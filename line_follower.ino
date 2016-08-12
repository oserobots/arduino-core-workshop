/* Line_Maze_Solver
 by Geoff Warren
 9 Aug 2016

 Specific servo orientation on "Herman" Arduino bot is:
            leftservo forward = 180
            rightservo forward = 0
*/

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//         L I B S   &   V A R S   &   I N I T I A L I Z E R S               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  9            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     8            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100          // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Servo myservo;                  // create servo object to control the front servo
Servo leftservo;                // create servo object on left of robot
Servo rightservo;               //create servo object on right of robot
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void rest();                //initiallizes rest function
void drive(int cm);         //allows forward function to compile in the void_setup
int pos = 0;                // variable to store the servo position
int dist = 550/15;          // variable to convert to centimeters
int twist = 540/180;        // variable to convert to degrees
unsigned int pingInches(); 
int bypassObject(); int avoid();
int lineSolver(); int lineMaze();
int loopCount = 0; int findGround();
unsigned int groundType; unsigned int initialGroundtype; int whatGround();

void tankLeft(unsigned int deg); void tankRight(unsigned int deg);    //allows swivel funtions to compile
void turnLeft(int deg); void turnRight(int deg);                      //allows turn functions to compile

//**************************************************************************************************************************
//                S E T U P   &   L O O P                                                                                ***
//**************************************************************************************************************************
void setup() 
{
  rest();                           //stops the bot before any other commands are given
  myservo.attach(3);                //attaches the servo on pin # to the myservo object
  rightservo.attach(5);             //attaches the servo on pin # to the servoleft object
  leftservo.attach(6);              //attaches the servo on pin # to the servoright object
  pinMode(2, INPUT);                //pin 2 is infrared input

  findGround();
}

//--------------------------------------------------------------------------------------------------------------------------
void loop() 
{
lineMaze();


  
}

//**************************************************************************************************************************
//**************************************************************************************************************************

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                 M O V E M E N T   F U N C T I O N S                       //
//                                                                           //
//     left wheel on #6    right wheel on #5     1/2 rotation servo on #3    // 
///////////////////////////////////////////////////////////////////////////////

void drive(int cm)            //drive function 
{
  if (cm > 0)                 //move forward by # called
  {
    rightservo.write(0);
    leftservo.write(180);
    delay(cm * dist);           //move forward by called centimeters
    rest();                        //stop after moving
  }
  else if (cm < 0)              //move backwards by # called
  {
    rightservo.write(180);
    leftservo.write(0);          //move backwards by called centimers
    delay(cm * -dist);            
    rest();                        //stop after moving
  }
  else                            //if 0 is called do nothing
  {
  }
}

void turnLeft(int deg)      //bot will turn one wheel to turn
                            //positive input will result in forward motion turning, negative in backwards motion turning
{
  if (deg >= 0)
    {
      leftservo.write(90); rightservo.write(0);   //right wheel forward, left wheel stopped
      delay(deg*twist);   
      rest();                        //stop after moving
    }
  else
    {
      leftservo.write(0); rightservo.write(90);   //right wheel backwards, left wheel stopped
      delay(deg*twist);   
      rest();                        //stop after moving
    }
}
void turnRight(int deg)     //bot will turn one wheel to turn
                            //positive input will result in forward motion turning, negative in backwards motion turning
{
  if (deg >= 0)
  {
    leftservo.write(180); rightservo.write(90);   //left wheel forward, right wheel stopped
    delay(deg*twist);   
    rest();                        //stop after moving  
  }
  else
  {
    leftservo.write(90); rightservo.write(180);   //right wheel backwards, left wheel stopped
    delay(deg*twist);   
    rest();                        //stop after moving         
  }
  
}
void tankLeft(unsigned int deg)     //bot will turn both wheels to turn
{
  leftservo.write(0); rightservo.write(0);   //left wheel forward, right wheel backwards
  delay(abs(deg)*twist);   
  rest();                        //stop after moving  
}
void tankRight(unsigned int deg)     //bot will turn both wheels forward to turn
{
  leftservo.write(180); rightservo.write(180);   //right wheel forward, left wheel backwards
  delay(abs(deg)*twist);   
  rest();                        //stop after moving  
}

void rest()                       //bot will stop moving (wheel servos only)
{
  leftservo.write(89); rightservo.write(90);
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                     S E N S O R   F U N C T I O N S                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int findGround();
{
  if (digitalRead(2) == HIGH)       //set ground type (hot or cool) and assign to initial condtion for later comparison
  {
    groundType = 1; //hot
  }
  else
  {
    groundType = 0; //cool
  }
  initialGroundtype = groundType;
}

int whatGround()    //calls whatGround() and initialGroundtype
{                                                         //return 1 for looking at initial groundtype, 0 for other
  if (whatGround() == initialGroundtype)                  
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//      L I N E    M A Z E    S O L V E R    F U N C T I O N S               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int lineSolver()  //calls avoid(), turnLeft(degrees, turnRight(degrees), increments loopCount
{                                                        //solve a line maze
  avoid();                                               //run avoid script
  if (whatGround() != initialGroundtype)                                    //while off line, turn left
  {
    turnLeft(5); loopCount += 5;
  }
  else                                                   //while on line, turn right
  {
    turnRight(5); loopCount -=5;
  }
}

int avoid()   //calls bypassObject()                      //avoid small object function
{ 
  myservo.write(90);                                      //set servo straight forward
  unsigned int look = sonar.ping_cm();                    //variable set to value of any object in front of bot
  if (look <= 15 && look != 0)                            //if object is between 0 and 15 cm away, run bypass function
  {
    bypassObject();
  }
}
int bypassObject()    //calls drive(distance), turnRight(degrees), tankLeft(degrees)
{                                                         //go around a small object on the left side
  tankLeft(95);                                           //turn left in place
  for (int i = 0; i < 2; i += 1)                          //2 times, go forward then turn right
  {
    drive(25);
    turnRight(85);
  }
  do                                                      //drive forward until bot finds the line
  {
    leftservo.write(180); rightservo.write(0);
  } while (whatGround() == initialGroundtype);
  tankLeft(90);                                           //reset bot orientation to left of line (bot will find line on the right)
}

int lineMaze()    //calls whatGround, initialGroundtype, lineSolver(), drive(dist in cm), turnRight(deg)
{                                                         //run on the line unless 1200 degrees of turn sequentially
  while (loopCount < 1200 && loopCount > -1200)
  {
    lineSolver();
  }
  
  loopCount=0;                                            //if 3.3 loops, reset count
  
  while (whatGround() == initialGroundtype)               //while sensor detects original ground type, drive in a broad ...
  {                                                       //...circle until you find the line
    drive(5); turnRight(15); drive(5);
  }
}


