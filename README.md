# STM32 Quadcopter Flight Controller

A quadcopter flight controller project built around the STM32F446RE and Raspberry Pi.

The goal of this project is to develop a flight controller from the ground up using bare-metal STM32 programming. The STM32 handles sensor communication, motor control, and flight-control calculations, while the Raspberry Pi provides a web-based control dashboard.

## Hardware

- STM32 NUCLEO-F446RE
- Raspberry Pi
- MPU6050 IMU
- BMP280 barometric pressure sensor
- 4-in-1 ESC
- Brushless motors
- LiPo battery

## Current Features

- Bare-metal STM32 peripheral configuration
- I2C communication with MPU6050 and BMP280
- MPU6050 calibration and orientation measurements
- BMP280 pressure and altitude measurements
- PWM control for four ESCs
- Motor mixing for throttle, pitch, roll, and yaw
- Interrupt-driven UART communication between Raspberry Pi and STM32
- Flask web dashboard with virtual joysticks
- ARM, DISARM, LAND, and emergency stop command interface
- STM32 telemetry sent to Raspberry Pi

## Current Status

The project is currently in the bench-testing stage.

Motor control, sensor communication, UART communication, and the Raspberry Pi dashboard are working. An intermittent I2C STOP synchronization issue during BMP280 reads was identified during testing. Waiting for STOP completion after the final bytes of the read has remained stable during current testing, but additional testing is still being performed.

Propellers are removed during bench testing.

## In Progress

- PID stabilization
- Pitch and roll self-correction
- Altitude hold
- Battery voltage monitoring
- Communication-loss failsafe
- Automatic landing
- Final arm/disarm safety logic
- Flight testing

## Communication

The Raspberry Pi communicates with the STM32 through UART at 115200 baud.

Joystick packets use:

`J,throttle,pitch,roll,yaw`

Special commands:

- `A` - Arm
- `D` - Disarm
- `L` - Land
- `E` - Emergency stop

## Project Goal

The final goal is a quadcopter capable of stable flight, self-correction, altitude hold, and automatic landing during low-battery or communication-loss conditions.