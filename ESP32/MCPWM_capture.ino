#include <Arduino.h>
#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"

// Define the input pin connected to your astable multivibrator
#define CAPTURE_PIN GPIO_NUM_4

// Volatile variables for safe ISR-to-main communication
volatile uint32_t last_rising_edge = 0;
volatile uint32_t latest_period_ticks = 0;
volatile uint32_t latest_high_ticks = 0;
volatile bool first_edge_captured = false;

uint32_t cap_resolution = 0;
mcpwm_cap_timer_handle_t cap_timer = NULL;
mcpwm_cap_channel_handle_t cap_channel = NULL;

// Hardware ISR: Keep it ultra-fast (Integer math only, no float operations)
static bool IRAM_ATTR capture_callback(
        mcpwm_cap_channel_handle_t chan,
        const mcpwm_capture_event_data_t *edata,
        void *user_data)
{
    uint32_t current_tick = edata->cap_value;

    if (edata->cap_edge == MCPWM_CAP_EDGE_POS)
    {
        if (first_edge_captured)
        {
            // Period is the duration between two consecutive rising edges
            latest_period_ticks = current_tick - last_rising_edge;
        }
        else
        {
            first_edge_captured = true;
        }
        last_rising_edge = current_tick;
    }
    else // MCPWM_CAP_EDGE_NEG
    {
        if (first_edge_captured)
        {
            // High ticks is the duration from the last rising edge to this falling edge
            latest_high_ticks = current_tick - last_rising_edge;
        }
    }

    return false;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // 1. Configure the Input Pin
    gpio_set_direction(CAPTURE_PIN, GPIO_MODE_INPUT);

    // 2. Configure and Initialize the Capture Timer (Uses APB clock @ 80MHz)
    mcpwm_capture_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_CAPTURE_CLK_SRC_APB,
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_timer(&timer_config, &cap_timer));

    // Fetch the actual hardware resolution (ticks per second, usually 80,000,000)
    ESP_ERROR_CHECK(mcpwm_capture_timer_get_resolution(cap_timer, &cap_resolution));

    // 3. Configure Channel to capture BOTH positive and negative edges
    mcpwm_capture_channel_config_t channel_config = {
        .gpio_num = CAPTURE_PIN,
        .prescale = 1, // No prescaler for maximum timing resolution
        .flags = {
            .pos_edge = true,
            .neg_edge = true,
            .pull_up = false,
            .pull_down = false,
        },
    };
    ESP_ERROR_CHECK(mcpwm_new_capture_channel(cap_timer, &channel_config, &cap_channel));

    // 4. Register the Callback and Enable Hardware
    mcpwm_capture_event_callbacks_t callbacks = {
        .on_cap = capture_callback,
    };
    ESP_ERROR_CHECK(mcpwm_capture_channel_register_event_callbacks(cap_channel, &callbacks, NULL));
    ESP_ERROR_CHECK(mcpwm_capture_channel_enable(cap_channel));
    ESP_ERROR_CHECK(mcpwm_capture_timer_enable(cap_timer));
    ESP_ERROR_CHECK(mcpwm_capture_timer_start(cap_timer));

    Serial.println("--- Astable Multivibrator Monitor Ready ---");
}

void loop()
{
    // Read and display data every 500ms
    delay(500);

    uint32_t period = 0;
    uint32_t high = 0;

    // Critical Section: Temporarily disable interrupts to copy volatile data safely
    noInterrupts();
    period = latest_period_ticks;
    high = latest_high_ticks;
    interrupts();

    // Prevent Division by Zero if no signal is connected
    if (period == 0)
    {
        Serial.println("Waiting for signal...");
        return;
    }

    // High accuracy floating-point math done safely in the main loop
    float frequency = (float)cap_resolution / (float)period;
    float duty_cycle = ((float)high / (float)period) * 100.0f;

    // Print results
    Serial.print("Freq: ");
    Serial.print(frequency, 2); // 2 decimal places for accuracy
    Serial.print(" Hz  |  Duty Cycle: ");
    Serial.print(duty_cycle, 3); // 3 decimal places for precise duty tracking
    Serial.println(" %");
}