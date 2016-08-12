// Written by: Jimmy LeBaron & Morgan Durocher
// Maze Navigating Robot Project 2016

#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  12 // Create the variables for the Sonar Sensor
#define ECHO_PIN     11
#define MAX_DISTANCE 50
#define DELAY_TIME   50

///////////////////////////T U R N   S E T T I N G S/////////////////////////////////

#define LEFT_TURN_DEGREES -90  // Use a negative number here
#define RIGHT_TURN_DEGREES 90 // Use a positive number here
#define LEFT_ADJUST -5 // Use a negative number here (Increase if the robot hits the wall a lot)
#define RIGHT_ADJUST 5  // Use a positive number here
#define HEAD_TURN_DEGREES 0 // This is to adjust if the head doesn't point directly right
#define LEFT_STOP 90 // Degree to stop the left servo (Adjust if the servo doesn't stop moving)
#define RIGHT_STOP 90 // Degree to stop the right servo (Adjust if the servo doesn't stop moving)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // Create the sonar sensor object
Servo head; // Define all the servos
Servo left;
Servo right;

void forward(int cm); // Initialize the methods we created
void turn(int deg);
void tank(int deg);
int count = 0;  // Count will keep track of how often we need to look forward

///////////////////////////////////////////////S E T U P///////////////////////////////////////////////

void setup() {
  head.attach(2); // Assign the servos to their pins
  left.attach(4);
  right.attach(3);
  left.write(LEFT_STOP);
  right.write(RIGHT_STOP);
  delay(2000);
}

////////////////////////////////////////////////// L O O P ////////////////////////////////////////////

void loop() {
  head.write(HEAD_TURN_DEGREES);  // Turn the head to the right
  delay(200);
  int distance = sonar.ping_cm();  // Check the distance from the right wall
  if (count < 6) {  // As long as count is less than 6, we can keep following the wall
    if (distance > 8 && distance < 12) { // If in between 7 and 15, go straight
      forward(8);
      count ++;
    } else if (distance <= 8 && distance > 0) {  // If to close to wall, turn away and scoot forward
      turn(LEFT_ADJUST);
      //delay(100);
      forward(2);
    } else if (distance >= 12 && distance < 25) {  // If to far away, turn towards the wall and scoot forward
      turn(RIGHT_ADJUST);
      //delay(100);
      forward(2);
    } else if (distance >= 25 || distance == 0) {  // If the wall is now gone, turn right to go around the corner
      forward(7);
      //delay(100);
      turn(RIGHT_TURN_DEGREES);
      //delay(100);
      forward(7);
      count = 5; // Make count 5 so we look forward after a turn to check for a wall
    }
    count ++;
  } else {  // If the count is more than six, we need to look forward
    head.write(90);  // Look forward
    delay(200);
    distance = sonar.ping_cm();  // Check the distance ahead of the robot
    if (distance <= 30 && distance > 0) { // If the wall is comming up...
      count = 4;                          // Make count 4 so we will check forward sooner next time
    }
    if (distance <= 15 && distance > 0) {  // If the wall is close, then turn to the left...
      if (distance <= 4 && distance > 0) { // If the wall is too close, back up before turning
      forward(-7);
      }
      turn(LEFT_TURN_DEGREES);
      //delay(200);
      forward(5);
      count = 5;
    } else {
      count = 0;
    }
  }
  //delay(50);
}

//////////////////////////////////////////////// M E T H O D S ////////////////////////////////////////

void forward(int cm) {
  double time = (316.5 * (abs((double)cm) / 10));

  if (cm > 0) {
    left.write(180);
    right.write(0);
    delay(time);
    left.write(90);
    right.write(90);
  } else {
    left.write(0);
    right.write(180);
    delay(time);
    left.write(90);
    right.write(90);
  }
}

void turn(int deg) {
  double time = ((abs((double)deg) / 360) * 2145); // 2145 ms is 360 degrees
  if (deg > 0) {
    right.write(90);
    left.write(180);
    delay(time);
    left.write(90);
  } else {
    right.write(0);
    left.write(90);
    delay(time);
    right.write(90);
  }
}

void tank(int deg) {
  double time = (abs((double)deg / 360) * 2200); // 2145 ms is 360 degrees 2400
  if (deg > 0) {
    right.write(0);
    left.write(0);
    delay(time / 2);
    right.write(90);
    left.write(90);
  } else {
    right.write(0);
    left.write(90);
    delay(time);
    right.write(90);
  }
}
