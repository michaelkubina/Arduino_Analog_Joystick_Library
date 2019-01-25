#include <Joystick.h>

#define HORIZONTAL_PIN A0
#define VERTICAL_PIN A1
#define SELECT_PIN 2

// create joystick object with specified pin-layout
Joystick joystick(HORIZONTAL_PIN, VERTICAL_PIN, SELECT_PIN);

void setup() {
  // start serial-monitor
  Serial.begin(9600);

  // dont touch your joystick here, since it is required to be in its resting position
  joystick.initialize();
  
  // check serial-monitor for command: move to all outer directions and press button
  joystick.calibrate();

  // sets the threshold for registering movement of the joystick
  joystick.setSensivity(5);
}

void loop() {
  // press button and get it confirmed
  if(joystick.isPressed())
  {
    Serial.println("Button is pressed!");
  }

  // output the direction if the button is not pressed
  if(joystick.isReleased())
  {
    // left
    if(joystick.isLeft())
    {
      Serial.println("Joystick is left");
      Serial.println(joystick.readX());
    }

    // right
    if(joystick.isRight())
    {
      Serial.println("Joystick is right");
      Serial.println(joystick.readX());
    }

    // up
    if(joystick.isUp())
    {
      Serial.println("Joystick is up");
      Serial.println(joystick.readY());
    }

    // down
    if(joystick.isDown())
    {
      Serial.println("Joystick is down");
      Serial.println(joystick.readY());
    }
  }

  // slow down a bit
  delay(100);
}
