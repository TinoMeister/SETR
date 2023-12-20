# Smart Housing - Embedded Systems Project

This collaborative project, designed for groups of 3 or 4 students, focuses on the development of integrated real-time embedded systems for Smart Housing (SH). The goal is to implement and showcase various subsystems in a prototype setup. The project should be documented in a structured report, describing the design and development of each system.

## Project Overview

The project involves the creation of a Home Automation pilot project, incorporating multiple embedded systems, each with specific functionalities such as lighting control, climate regulation, security, and additional automation features. The development will utilize co-designed hardware and software, implemented with commented code, and demonstrated using Arduino and the Arduino TinkerCad simulator.

### System A - Interior Lighting Control

Develop a lighting control system for indoor spaces. The system adjusts lighting based on sunlight intensity, ensuring consistent illumination for enhanced energy efficiency. Five light intensity scales are considered, each corresponding to a specific LED output:

- >= 800: LED 255
- >= 600 and < 800: LED 192
- >= 400 and < 600: LED 128
- >= 200 and < 400: LED 64
- < 200: LED 0

Simulate the system using LEDs and an LDR sensor. Display the sensor input and LED output on the serial monitor.

### System B - Climate Control

Develop a climate control system based on ambient temperature. A fan is activated for space cooling depending on temperature values from a sensor. The fan should turn on when the temperature exceeds 24 degrees Celsius and turn off when it drops below 20 degrees Celsius. Two LEDs provide indications:

- Red LED: Indicates cooling is active.
- Green LED: Indicates a stable temperature.

Display the fan status ("Fan ON/Fan OFF") and current temperature ("Temp: xxxºC") on an LCD display. Control the display brightness using a potentiometer.

### System C - Security System (Alarm)

Create a security system for intrusion detection using a PIR motion sensor. When motion is detected, trigger:

- Flashing red LED.
- Characteristic alarm sound for 10 seconds.
- Consider a button to disarm the alarm.
- Repeat the alarm sound (10 seconds) with 5-second intervals until disarmed.

### System D

Develop an additional embedded system for integration into Smart Housing, automating various tasks such as parking, irrigation, or access control. Use at least three distinct components between sensors and actuators, with monitoring displayed on the serial monitor.

### Additional Requirements

- Use of Interrupts in one of the developed systems (2 points).
- Creation of a graphical interface for real-time remote monitoring of one of the developed systems (2 points).
- Multitasking (with Timer or RTOS) to manage the concurrent execution of 2 or more components in one of the systems (2 points).
- Performance analysis of the program using path profiling, flags, and timers on the serial monitor. Compare the energy efficiency of two program versions for one of the systems (2 points).