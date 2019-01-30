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
  // automatic timeout after 15 seconds
  joystick.calibrate();

  // show all status-values
  joystick.getStats();

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

    // upper-left
    if(joystick.isUpperLeft())
    {
      Serial.println("Joystick is upper-left");
    }

    // upper-right
    if(joystick.isUpperRight())
    {
      Serial.println("Joystick is upper-right");
    }

    // lower-left
    if(joystick.isLowerLeft())
    {
      Serial.println("Joystick is lower-left");
    }

    // lower-right
    if(joystick.isLowerRight())
    {
      Serial.println("Joystick is lower-right");
    }
  }

  // slow down a bit
  delay(100);
}
