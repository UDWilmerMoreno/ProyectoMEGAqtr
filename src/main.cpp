#include <Arduino.h>
#include <Servo.h>
#include <QTRSensors.h>
#define Sensor1 A15
#define Sensor2 A14
#define Sensor3 A13
#define Sensor4 A12
#define Sensor5 A11
#define Sensor6 A10
#define LedRojo 33
#define LedVerde 35
#define LedAzul 37
#define SensorOUT 50
#define M1Adelante 45
#define M1Atras 47
#define M2Adelante 49
#define M2Atras 51
#define PWM1 10
#define PWM2 9
const int s0 = 22;
const int s1 = 24;
const int s2 = 34;
const int s3 = 36;
const int out = 32;
int velocidad = 90;
int Trig = 30;
int Echo = 31;
int pinservo = 44;
int pulsomin = 520;  //520
int pulsomax = 2500; //2500

Servo servo1;
QTRSensors qtr;
byte countRed = 0;
byte countGreen = 0;
byte countBlue = 0;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

void setup()
{
  Serial.begin(9600);
  servo1.attach(pinservo, pulsomin, pulsomax);
  pinMode(M1Adelante, OUTPUT);
  pinMode(M1Atras, OUTPUT);
  pinMode(M2Adelante, OUTPUT);
  pinMode(M2Atras, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(LedRojo, OUTPUT);
  pinMode(LedVerde, OUTPUT);
  pinMode(LedAzul, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){Sensor1, Sensor2, Sensor3, Sensor4, Sensor5, Sensor6}, SensorCount);
  qtr.setEmitterPin(SensorOUT);

  delay(500);

  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }

  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }

  Serial.println();
  Serial.println();
  //delay(1000);
}

void getColor()
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  countRed = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  countBlue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  countGreen = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}

void Alto()
{
  //digitalWrite(LedRojo, HIGH);
  //digitalWrite(LedVerde, HIGH);
  //digitalWrite(LedAzul, HIGH);
  digitalWrite(M1Adelante, LOW);
  digitalWrite(M1Atras, LOW);
  digitalWrite(M2Adelante, LOW);
  digitalWrite(M2Atras, LOW);
}

void Adelante()
{
  //digitalWrite(LedRojo, HIGH);
  //digitalWrite(LedVerde, LOW);
  //digitalWrite(LedAzul, HIGH);
  digitalWrite(M1Adelante, HIGH);
  digitalWrite(M1Atras, LOW);
  digitalWrite(M2Adelante, HIGH);
  digitalWrite(M2Atras, LOW);
  delay(200);
  Alto();
}
void Atras()
{
  //digitalWrite(LedRojo, LOW);
  //digitalWrite(LedVerde, LOW);
  //digitalWrite(LedAzul, LOW);
  digitalWrite(M1Adelante, LOW);
  digitalWrite(M1Atras, HIGH);
  digitalWrite(M2Adelante, LOW);
  digitalWrite(M2Atras, HIGH);
}
void Derecha()
{
  //digitalWrite(LedRojo, LOW);
  //digitalWrite(LedVerde, HIGH);
  //digitalWrite(LedAzul, HIGH);
  digitalWrite(M1Adelante, HIGH);
  digitalWrite(M1Atras, LOW);
  digitalWrite(M2Adelante, LOW);
  digitalWrite(M2Atras, HIGH);
}
void Izquierda()
{
  //digitalWrite(LedRojo, HIGH);
  //digitalWrite(LedVerde, HIGH);
  //digitalWrite(LedAzul, LOW);
  digitalWrite(M1Adelante, LOW);
  digitalWrite(M1Atras, HIGH);
  digitalWrite(M2Adelante, HIGH);
  digitalWrite(M2Atras, LOW);
}

void GiroDerecha()
{
  Alto();
  Derecha();
  delay(200);
  Alto();
}

void GiroIzquierda()
{
  Alto();
  Izquierda();
  delay(200);
  Alto();
}

void GiroTotal()
{
  Alto();
  Derecha();
  delay(400);
  Alto();
}

void loop()
{
  //Adelante();
  servo1.write(0);
  analogWrite(PWM1, velocidad);
  analogWrite(PWM2, velocidad);
  uint16_t position = qtr.readLineBlack(sensorValues);

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }

  Serial.println(position);
  delay(50); // TENER EN CUENTA EL TEMPO PARA EL DISPARO DE LOS MOTORES

  if (sensorValues[0] < 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
      sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] < 500)
  {
    Adelante();
  }
  else if (sensorValues[0] < 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] > 500)
  {
    GiroIzquierda();
  }
  else if (sensorValues[0] < 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroIzquierda();
  }
  else if (sensorValues[0] < 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroIzquierda();
  }
  else if (sensorValues[0] < 500 && sensorValues[1] < 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroIzquierda();
  }
  else if (sensorValues[0] < 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroIzquierda();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] < 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] < 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] < 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] < 500 && sensorValues[5] < 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] < 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] > 500 &&
           sensorValues[3] > 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] < 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] < 500 && sensorValues[5] > 500)
  {
    GiroDerecha();
  }
  else if (sensorValues[0] > 500 && sensorValues[1] > 500 && sensorValues[2] < 500 &&
           sensorValues[3] < 500 && sensorValues[4] > 500 && sensorValues[5] > 500)
  {
    GiroDerecha();
  }
  else
  {
    Adelante();
  }
  long duracion;
  long distancia;
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  duracion = pulseIn(Echo, HIGH);
  duracion = duracion / 2;
  distancia = duracion / 29;
  //Serial.println(distancia);
  if (distancia <= 4)
  {
    Alto();
    servo1.write(0);
    delay(1000);
    servo1.write(33);
    getColor();
    if (countRed < countBlue && countGreen > 100 && countRed < 80)
    {
      //Serial.println("Rojo");
      digitalWrite(LedRojo, LOW);
      digitalWrite(LedVerde, HIGH);
      digitalWrite(LedAzul, HIGH);
    }
    else if (countBlue < countRed && countBlue < countGreen)
    {
      //Serial.println("Azul");
      digitalWrite(LedRojo, HIGH);
      digitalWrite(LedVerde, HIGH);
      digitalWrite(LedAzul, LOW);
    }
    else if (countGreen < countRed && countGreen < countBlue)
    {
      //Serial.println("Verde");
      digitalWrite(LedRojo, HIGH);
      digitalWrite(LedVerde, LOW);
      digitalWrite(LedAzul, HIGH);
    }
    else
    {
      //Serial.println("Sin color");
      digitalWrite(LedRojo, HIGH);
      digitalWrite(LedVerde, HIGH);
      digitalWrite(LedAzul, HIGH);
    }
    delay(3000);
    servo1.write(0);
    digitalWrite(LedRojo, HIGH);
    digitalWrite(LedVerde, HIGH);
    digitalWrite(LedAzul, HIGH);
    Atras();
    delay(200);
  }
}