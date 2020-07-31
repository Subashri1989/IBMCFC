#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,4);

void setup()
{
  mySerial.begin(9600);
  pinMode(A1, OUTPUT);
}

void loop()
{
  mySerial.println(map(analogRead(A1), 332, 265, 0, 90));
  delay(700);
} // end loop

