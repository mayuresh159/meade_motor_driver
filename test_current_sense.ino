const int max_count = 2000;

int csense_in = 0;
int motorpin_a = 9;
int motorpin_b = 10;

void setup()
{
  pinMode(motorpin_a,OUTPUT); digitalWrite(motorpin_a,HIGH);
  pinMode(motorpin_b,OUTPUT); digitalWrite(motorpin_b,LOW);
  init_pwm_engine();
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  unsigned int csense_read;
  set_pwm(1200);
  delay(500);
  csense_read = analogRead(csense_in);
  Serial.println(csense_read,DEC);
}

// pwm engine 
void init_pwm_engine() // phase correct pwm mode with a frequency of 1kHz
{
  //OCR1A = 0;
  OCR1A = 0;
  ICR1 = max_count; // 1000
  TCCR1A = _BV(COM1A1)  | _BV(WGM11); //_BV(COM1A1)
  TCCR1B = _BV(WGM13) | _BV(CS10);
}
void set_pwm(unsigned int val){ OCR1A = val;} // updating the value of OCR to change the pwm duty cycle
