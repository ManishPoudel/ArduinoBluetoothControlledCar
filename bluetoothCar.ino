/*
  Bluetooth Control Car.

  The pin configuration for the car is as follows:-

      For Motor Controller (l298N):-

          Arduino pin 8,9,10,11 pins to l298N Driver pins respectively.

                          |
                          V

          ( put the l298n Driver Such that the ic behind the heat sink is visible to you
          so the control pins of l298N is closet to you then attacht 8,9,10,11 sequentially
          from left to right int the l298N motor driver)

          Arduino pin 5,6    =>      To the enable A and enable B pin (corner header pins of l298n Driver)


          Bluetooth Module connections :-

                      Gnd                       =>  Gnd arduino

                      5v OR vcc                 =>  5v arduino

                      Tx of bluetooth module    => Rx of arduino.


  The Software you need to Run this program is "Arduino car" android application made by "one day of Code".
*/


#define SERIAL_RATE 9600
// Direction constants.
#define FRONT 'F'
#define BACK 'B'
#define RIGHT 'R'
#define LEFT 'L'
#define STOP 'S'
#define ENABLE_A 5  // Enable motor A for pwm speed control.(pin 5).
#define ENABLE_B 6  // Enable motor B for pwm speed control.(pin 6).

// Motors control pins.
int m1a = 8;
int m1b = 9;
int m2a = 10;
int m2b = 11;

// Initial speed of Motors.
int speed = 100;
// The force at with brake is applied.
int brakeForce = 4000;

/*
  If one motor runs slower OR the car is tilting in
  certain direcction while going forward then change these
  offsets to change the speed of each motor so that
  the path is straight of the car.
*/
int m1SpeedOffset = 0;
int m2SpeedOffset = 0;

void setup(){
  // First we Start with 100 speed.
  analogWrite(ENABLE_A, speed + m1SpeedOffset);
  analogWrite(ENABLE_B, speed + m2SpeedOffset);
  pinMode(m1a, OUTPUT);  // Digital pin 10 set as output Pin
  pinMode(m1b, OUTPUT);  // Digital pin 11 set as output Pin
  pinMode(m2a, OUTPUT);  // Digital pin 12 set as output Pin
  pinMode(m2b, OUTPUT);  // Digital pin 13 set as output Pin
  Serial.begin(SERIAL_RATE);
}

// Move motor in desired driection.
void moveMotor(char dir){

  if(dir == BACK)  // Reverse
  {
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, HIGH);
    return;
  }
  if(dir == FRONT)  // Forward
  {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, HIGH);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW);
    return;
  }
  if(dir == RIGHT)  //Right
  {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, HIGH);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, HIGH);
    return;
  }
  if(dir == LEFT)  //Left
  {
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW);
    return;
  }
  if(dir == STOP)  //Stop
  {
    brakeMotor();
    return;
  }

  // For Extra and more controll.
  if(dir == 'I')  //Backward Left
  {
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
    return;
  }
  if(dir == 'J')  //Forward Left
  {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, HIGH);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);
    return;
  }
  if(dir == 'G')  //Backward Right
  {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, HIGH);
    return;
  }
  if(dir == 'H')  //Forward Right
  {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW);
    return;
  }

  //SPEED increment and decrement using X and Y keys.
  if(dir == 'X' && (speed + m2SpeedOffset) <= 240 && (speed + m1SpeedOffset) <= 240){
    speed += 10;
    //Serial.println(speed); // UnComment if you want serial output on screen.
    return;
  }

  if(dir == 'Y' && speed >= 15){
    speed -= 10;
    //Serial.println(speed); // UnComment if you want serial output on screen.
    return;
  }

  /*
// Code for slider to work and change speed.
// Work in progress.

  if (dir == "M") {
    speed=180;
    int temp = 0;
    int val=1;
    for (int x = 0; val > 0; x++) {
      temp = val + 10 *temp;
      val = Serial.read();
    }
    speed = temp;
    Serial.println(speed);
    Serial.println(speed);
    return;
  }
  */

  return;
}

void brakeMotor(){
  // Electronic Braking System implemented.
  // Each time, in the Arduino car app the button is released after pressing 'S' is sent and break is applied.
  for(int i = 0; i < brakeForce; i++){
    moveMotor(BACK);
  }
  //Stop Motors.
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, LOW);
  return;
}

void loop(){
  char val; // Value Varaible for recieved value.
  while(Serial.available() > 0){
    // Changing speed.
    analogWrite(ENABLE_A, speed + m1SpeedOffset);
    analogWrite(ENABLE_B, speed + m2SpeedOffset);
    val = Serial.read();
    moveMotor(val);
    //Serial.println(val); // UnComment if you want serial output on screen.
  }
}
