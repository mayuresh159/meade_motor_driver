int encoder_in_up = 2;
int encoder_in_down = 3;

unsigned long int count = 0;

void setup()
{
  pinMode(encoder_in_up,INPUT);
  pinMode(encoder_in_down,INPUT);
  attachInterrupt(0,update_count_up,FALLING);
  attachInterrupt(1,update_count_down,FALLING);
  Serial.begin(115200);
  delay(1000);
}

void loop()
{  
 unsigned long int count_samp = count;
 Serial.println(count_samp,DEC);
 delay(500);
}

void update_count_up() {count++;}
void update_count_down() {count--;}


