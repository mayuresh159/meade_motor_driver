int encoder_in_a = 2;
int encoder_in_b = 3;

unsigned long int pos_count = 0;

void setup()
{
  pinMode(encoder_in_a,INPUT);
  pinMode(encoder_in_b,INPUT);
  attachInterrupt(0,phase_a,CHANGE);
  attachInterrupt(1,phase_b,CHANGE);
  Serial.begin(115200);
  delay(1000);
}

void loop()
{  
 unsigned long int count_samp = pos_count;
 Serial.println(count_samp,DEC);
 delay(500);
}

// count interrupts
void phase_a() 
{
  boolean a,b;
  a = digitalRead(encoder_in_a);
  b = digitalRead(encoder_in_b);
  if ((a==1&&b==0)||(a==0&&b==1)) {pos_count++;}
  if ((a==0&&b==0)||(a==1&&b==1)) {pos_count--;}
}
void phase_b() 
{
  boolean a,b;
  a = digitalRead(encoder_in_a);
  b = digitalRead(encoder_in_b);
  if ((a==1&&b==1)||(a==0&&b==0)) {pos_count++;}
  if ((a==0&&b==1)||(a==1&&b==0)) {pos_count--;}
}

