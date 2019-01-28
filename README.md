# Arduino_Analog_Joystick_Library
A simple Library for a Arduino 2-axis analog joystick with select button.

## Requirements
  
1x Arduino Uno R3  
1x 2-Axis Analog Joystick with Switch-Button(KY-023 or similar)
6x Jumper Wire  
  
## Diagram
  
![alt text](https://github.com/Mimaku/Arduino_Analog_Joystick_Library/blob/master/examples/Joystick/Joystick_Steckplatine.svg "Diagram / Breadboard layout")
  
## Usage

```c
// create joystick object with specified pin-layout
Joystick joystick(HORIZONTAL_PIN, VERTICAL_PIN, SELECT_PIN);
  
void setup() {  
    // dont touch your joystick here, since it is required to be in its resting position
    joystick.initialize();  
}  
  
...  
  
void loop() {  
    // left  
    if(joystick.isLeft())  
    {  
      Serial.println("Joystick is left");  
      Serial.println(joystick.readX());  
    }  
}  
```  