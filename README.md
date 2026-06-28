# Open-Loop Boost Converter using a Discrete Astable Multivibrator

## Overview

This project presents the design, simulation, implementation, and experimental characterization of an **open-loop DC-DC boost converter** capable of stepping up a **1.5 V DC input** to approximately **7.8 V** under no-load conditions.

Unlike conventional boost converters that employ dedicated PWM controller ICs or microcontrollers, the switching signal is generated using a **fully discrete two-transistor astable multivibrator** followed by a **CD40106 Schmitt Trigger inverter**.

The project was undertaken to gain practical understanding of switching power electronics, boost converter operation, component selection, and the differences between theoretical analysis, simulation, and real-world hardware implementation.

---

# Features

- Fully discrete PWM generation
- Open-loop boost converter
- LTspice simulation before hardware implementation
- Hardware validation using ESP32 MCPWM Capture Peripheral
- Experimental optimization of transistor base drive
- Load regulation characterization
- Comparison between theoretical and practical performance

---

# Hardware Used

## Power Stage

- 2N2222A NPN Transistor
- 100 μH Inductor
- 1N5819 Schottky Diode
- 220 μF Electrolytic Capacitor
- 1.5 V AA Cell

## PWM Generation

- Two-Transistor Astable Multivibrator
- CD40106 Schmitt Trigger Inverter

## Measurement Equipment

- ESP32 Development Board
- Voltmeter

---

# Working Principle

The astable multivibrator continuously generates a square-wave switching signal. The CD40106 Schmitt Trigger cleans the waveform and drives the switching transistor.

When the transistor is ON, energy is stored in the magnetic field of the inductor.

When the transistor switches OFF, the inductor releases its stored energy through the Schottky diode into the output capacitor and load, thereby producing an output voltage greater than the input voltage.

---

# Simulation

The converter was first designed and analyzed in **LTspice** before hardware implementation.

Simulation was used to study:

- Output voltage
- Inductor current
- Switching waveforms
- Load variation
- Component selection

The simulated results were later compared with practical hardware measurements.

---

# Experimental Results

## Switching Characteristics

| Parameter | Measured Value |
|-----------|---------------:|
| Input Voltage | 1.5 V |
| Switching Frequency | 44.45 kHz |
| Duty Cycle | 82.3 % |

---

## Load Regulation

| Load Resistance | Output Voltage |
|---------------:|---------------:|
| No Load | 7.8 V |
| 2.2 kΩ | 7.0 V |
| 1 kΩ | 6.4 V |
| 470 Ω | 5.1 V |
| 330 Ω | 4.4 V |
| 180 Ω | 3.0–3.1 V |
| 90 Ω | 2.0 V |

---

## Effect of Base Resistance (330 Ω Load)

| Base Resistance | Output Voltage |
|---------------:|---------------:|
| 2.2 kΩ | 2.0 V |
| 1 kΩ | 3.0 V |
| 470 Ω | 3.8 V |
| 180 Ω | 4.4 V |
| 90 Ω | 4.5 V |

---

# Base Drive Analysis

The voltage measured across the **180 Ω base resistor** was approximately **2.2 V**, corresponding to an average base current of approximately **12.2 mA**.

The power dissipated in the base resistor was approximately **26.9 mW**, while the estimated power dissipated across the transistor base-emitter junction was approximately **8.5 mW**, resulting in a total base-drive power of approximately **35.4 mW**.

This experimental analysis demonstrates one of the primary limitations of BJT-based switching converters, motivating the use of MOSFETs in modern high-efficiency DC-DC converters.

---

# Key Observations

- The practical no-load output voltage closely matched the theoretical prediction.
- Output voltage decreased as the load current increased.
- Base-drive resistance significantly influenced converter performance.
- Practical performance differed from simulation due to transistor saturation, diode forward voltage, battery internal resistance, and parasitic losses.

---

# Skills Demonstrated

- Power Electronics
- Boost Converter Design
- LTspice Simulation
- Hardware Prototyping
- PWM Generation using Discrete Electronics
- ESP32 MCPWM Capture
- Experimental Characterization
- Hardware Debugging
- Component Selection
- Circuit Optimization

---

# Future Improvements

- Closed-loop voltage regulation
- MOSFET-based switching stage
- PCB implementation
- Current limiting and soft-start
- Efficiency optimization
- Higher power capability

---

# Repository Structure

```
├── LTSpice/
│   ├── BoostConverter.asc
│
├── Hardware/
│   ├── Circuit_Diagram.png
│   ├── circuit_picture.jpeg
│
├── ESP32/
│   └── MCPWM_capture.ino
│
├── Project Report.pdf
│
├── images/
│   ├── PWM.png
│   ├── Inductor_Current.png
│   └── Output.png
│
└── README.md
```

---

# Future Scope

- Replace the BJT with a logic-level MOSFET for higher efficiency.
- Implement closed-loop feedback for output voltage regulation.
- Investigate the effect of switching frequency and inductor value on efficiency.
- Design a PCB to reduce parasitic effects and improve reliability.

---

# License

This project is intended for educational and learning purposes.

---

## Author

**Chirag**

B.Tech, Electrical Engineering

Indian Institute of Technology (ISM) Dhanbad
