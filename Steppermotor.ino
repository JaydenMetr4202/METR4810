
 // Arduino Uno R3 + Adafruit TMC2209
// Move forward: 1 second ON, 1 second OFF
// Stop and reverse when limit switch on pin 5 is hit.
// Move backward: 1 second ON, 1 second OFF
// Stop permanently when limit switch on pin 4 is hit.

const int STEP_PIN = 2;
const int DIR_PIN  = 3;

const int FORWARD_LIMIT_PIN = 5;
const int REVERSE_LIMIT_PIN = 4;

// Motor speed
const int STEP_DELAY_US = 10000;

// Movement timing (milliseconds)
const unsigned long MOVE_TIME = 500;
const unsigned long PAUSE_TIME = 1000;

// Move one microstep
void stepMotor()
{
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(STEP_DELAY_US);

  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(STEP_DELAY_US);
}

// Move with 1 second ON / 1 second OFF
// until the specified limit switch is pressed
void moveUntilLimit(bool direction, int limitPin)
{
  digitalWrite(DIR_PIN, direction);
  delay(10);

  while (digitalRead(limitPin) == HIGH)
  {
    // Move for 1 second
    unsigned long startTime = millis();

    while (millis() - startTime < MOVE_TIME)
    {
      // Stop immediately if limit switch is pressed
      if (digitalRead(limitPin) == LOW)
      {
        return;
      }

      stepMotor();
    }

    // Stop if limit switch was triggered
    if (digitalRead(limitPin) == LOW)
    {
      return;
    }

    // Pause for 1 second
    delay(PAUSE_TIME);
  }
}

void setup()
{
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(FORWARD_LIMIT_PIN, INPUT_PULLUP);
  pinMode(REVERSE_LIMIT_PIN, INPUT_PULLUP);

  digitalWrite(STEP_PIN, LOW);

  delay(1000);

  // 1. Move forward in 1-second intervals
  // Until limit switch on pin 5 is pressed
  moveUntilLimit(HIGH, FORWARD_LIMIT_PIN);

  // 2. Reverse in 1-second intervals
  // Until limit switch on pin 4 is pressed
  moveUntilLimit(LOW, REVERSE_LIMIT_PIN);

  // 3. Finished
}

void loop()
{
  // Motor remains stopped permanently
}
