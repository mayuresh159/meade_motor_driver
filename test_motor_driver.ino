int motorpin_a = 9;
int motorpin_b = 10;
int ledpin = 13;

void setup()
{
  pinMode(ledpin,OUTPUT); digitalWrite(motorpin_a,LOW);
  pinMode(motorpin_a,OUTPUT); digitalWrite(motorpin_a,LOW);
  pinMode(motorpin_b,OUTPUT); digitalWrite(motorpin_b,LOW);
}

void loop()
{
  char i;
 for (i=0;i<2;i++)
  {
    digitalWrite(motorpin_a,!digitalRead(motorpin_a));
    digitalWrite(ledpin,!digitalRead(ledpin));
    delay(2000);
  } 
  digitalWrite(motorpin_b,!digitalRead(motorpin_b));
  digitalWrite(ledpin,!digitalRead(ledpin));
  delay (2000);
}
