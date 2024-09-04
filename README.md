# LX-224
Struggling hard with LX-224 & Arduino Uno

Using: 
- Arduino Uno R3, Arduino Mega R3
- LX-224
- Arduino IDE 2.3.2
- [LX16a-Servo](https://github.com/madhephaestus/lx16a-servo#electrical) 
- Buffer chip 74HC126 and wiring as defined in [madhephaestus-arm](https://github.com/Hephaestus-Arm/HephaestusArm2/blob/0.1.1/electronics.md#2-setting-up-the-board)
- Separate power supply, tried between 8-16VDC 

## VSCODE CONFIGURATION
If using vscode, make sure to have the C++/CLang formatter installed. 
If getting an error on the Serial1, Serial2:
- Make sure you have the Arduino Mega selected
- Make sure the following lines are in your `c_cpp_properties.json` file: 
```json
  "defines": [
    "ARDUINO=10607",
    "__AVR_ATmega2560__",
    "ARDUINO=10800",
    "ARDUINO_AVR_MEGA2560",
    "ARDUINO_ARCH_AVR",
  ]
```
- put the following in your settings.json:
`"clang-format.executable": ".clang-format"`
- In the Arduino extension settings, turn off `Arduino: Disable Intelli Sense Auto Gen`