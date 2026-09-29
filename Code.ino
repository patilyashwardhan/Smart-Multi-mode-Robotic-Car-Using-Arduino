#include<Servo.h>
Servo myservo ;

int IN1 = 2 ;//Motor 1
int IN2 = 4 ;
int EN1 = 3 ;

int IN3 = 5 ;//Motor 2
int IN4 = 7 ;
int EN2 = 6 ;

int IR1 = 8 ; //Left
int IR2 = 9 ; //Right

int trig = 11 ;//Ultrasonic
int echo = 12 ;
float duration , distance ;

int mode = 1 ;

void setup() 
{
  myservo.attach(10);
  
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR1 , INPUT);
  pinMode(IR2 , INPUT);

  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT); 
  Serial.begin(9600);
  myservo.write(90); 
}

void loop() 
{
  if (Serial.available() > 0)
  {
    char command = Serial.read();
    Serial.println(command);
    if (command == 'X' )
    {
      mode = 1 ;
      MoveStop();
    }
    else if (command == 'Y')
    {
      mode = 2 ;
      MoveStop();
    }
    else if(command =='Z')
    {
      mode = 3 ;
      MoveStop();
    }
  

    if (mode == 1)
    {
      if (command == 'A') 
      {
        MoveStop();
      }
      else if (command == 'B')
      {
         setSpeed(130 , 130);
         MoveForward();
      }
      else if (command == 'C') 
      {
        setSpeed(130 , 130);
        MoveBackward();
      }
      else if (command == 'D') 
      {
        setSpeed(150 ,150);
        MoveRight();
      }
      else if (command == 'E') 
      {
        setSpeed(150 , 150);
        MoveLeft();
      }
    }
  }

  if (mode == 2)
  {
    obstacleAvoid();
  }

  else if (mode == 3)
  {
    LineFollowing();
  }
}

float ultrasonic()
{
  float sum = 0;

  for (int j = 0; j < 3; j++)
  {
    digitalWrite(trig, LOW);
    delayMicroseconds(5);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    sum += 0.0343 * (duration / 2);

    delay(10);
  }

  distance = sum / 3;
  return distance;
}

void setSpeed(int speedVal1 , int speedVal2)
{
  analogWrite(EN1, speedVal1);
  analogWrite(EN2, speedVal2);  
}

void MoveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void MoveBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void MoveStop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MoveRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void MoveLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void obstacleAvoid()
{
  distance = ultrasonic();

  if (distance > 60)
  {
    setSpeed(130 , 130);
    MoveForward();
  }
  else if (distance > 35)
  {
    setSpeed(90 , 90);
    MoveForward();
  }
  else
  {
    MoveStop();
    delay(300);

    int maxdistance = 0;
    int Angle = 90;

    for (int i = 0; i <= 180; i += 30)
    {
      myservo.write(i);
      delay(300);

      float zdistance = ultrasonic();

      if (zdistance > maxdistance)
      {
        maxdistance = zdistance;
        Angle = i;
      }
    }

    myservo.write(90);
    delay(200);

    if (Angle < 70)
    {
      setSpeed(130 , 130);
      MoveRight();
      delay(500);
    }
    else if (Angle > 110)
    {
      setSpeed(130 , 130);
      MoveLeft();
      delay(500);
    }
    else
    {
      setSpeed(130 , 130);
      MoveForward();
    }
  }
}

void LineFollowing()
{
  int left = digitalRead(IR1);
  int right = digitalRead(IR2);

  if(left == 0 && right == 0)
  {
    setSpeed(90 , 90);
    MoveForward();
  }

  else if(left == 0 && right == 1)
  {
    setSpeed(90 , 110);
     MoveLeft();
  }
  else if(left == 1 && right == 0)
  {
   setSpeed(110 , 90);
   MoveRight();
  }

  else
  {
    MoveStop();
  }
}
