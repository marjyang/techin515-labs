# Lab 4 Magic Wand

This magic wand lights up different colors, representing different spells by recognizing the gestures you make. To cast a fire bolt spell, wave "Z" (red) in the air. To cast a reflect shield spell, wave "O" (blue). To cast a healing spell, wave "V" (green).  

## Setup Instructions

### Hardware
- Seeed Studio XIAO ESP32C3
- MPU6050
- Common cathode RGB LED
- 3 x 220 Ohm resistors
- Push button
- 3.7V 1100mAh battery
- Switch (SPDT)
- Breadboard and jumper wires (for testing)
- Enclosure!

### Wiring
- Button - D2, GND
- RGB LED
    - R: 220 Ohm resistor, GPIO8 (D8)
    - G: 220 Ohm resistor, GPIO9 (D9)
    - B: 220 Ohm resistor, GPIO21 (D6)
    - GND
- MPU6050
    - SDA: GPIO6 (D4)
    - SCL: GPIO7 (D5)
    - 3V3, GND
- Switch (SPDT) - 5V, GND
- Battery - Switch, GND

### Software Setup
1. Plug MCU to computer with USB-C cable.
2. On PlatformIO, create a new project, selecting Seeed Studio XIAO ESP32C3 as the board.
3. Add libraries: Adafruit MPU6050 and Adafruit Unified Sensor.
4. Download and unzip `edge_impulse_export.zip`, and add folder into the PlatformIO project `lib` folder.
5. Upload the `main.cpp` file from `esp32_magic_wand` to the MCU.
6. Open serial monitor to ensure it's working properly.
7. Press the button and wave in the air - Z, O or V shapes!
8. Feel free to unplug from the computer and add the battery.
