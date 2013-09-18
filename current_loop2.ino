// pwm limitations
const unsigned int max_count = 2000;  // defines a operating pwm frequency of 4kHz
const unsigned int pwm_up_sat = 1975; 
const unsigned int pwm_low_sat = 25;

// loop parameters
const unsigned int loop_time = 10;
// current loop parameters;
const float c_kp = 1,
            c_ki = 0.975,
            c_kd = 0.0;
            
// pin placements
int motorpin_a = 9;
int motorpin_b = 10;
int csense_in = 0;

// global variables
unsigned int pwm = 500;
unsigned int dir =  0; // motor direction and speed
unsigned int c_command = 700;
signed int c_error[3] = {0,0,0}; // current loop variables

void setup()
{
  pinMode(motorpin_a,OUTPUT); digitalWrite(motorpin_a,LOW);
  pinMode(motorpin_b,OUTPUT); digitalWrite(motorpin_b,LOW);
  init_pwm_engine();
  Serial.begin(115200);
  delay(8000);
}

void loop()
{
  set_pwm();
  delay(loop_time);
  c_loop();
  purge_data();
}

// pwm functions
void init_pwm_engine()
{
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(CS10);     // prescaler 1 for the system clock which generates base clock of 8MHz
  ICR1 = max_count;
}

void set_pwm()
{
  if (dir==0) {OCR1A = pwm; OCR1B = 0;}
  else {OCR1A = 0; OCR1B = pwm;}
}


// current loop correction
void c_loop()
{
  unsigned int c_read=0,i;
  for (i=0;i<5;i++)
  {
    c_read = c_read + analogRead(csense_in);
  }
  c_read /= 5;
  c_error[0] = c_command - c_read;
  c_pid();
  c_error[2] = c_error[1];
  c_error[1] = c_error[0];  
}

void c_pid()
{
  float new_pwm;  
  new_pwm = (float)pwm + c_kp*(c_error[0] - c_error[1]) + c_ki*c_error[1] + c_kd*(c_error[0] - c_error[2]);
  if (new_pwm >= pwm_up_sat) new_pwm = pwm_up_sat;
  if (new_pwm <= pwm_low_sat) new_pwm = pwm_low_sat;
  pwm = (int) new_pwm;
}

// print data on serial port
void purge_data()
{
  Serial.print(c_command,DEC); Serial.print("\t");
  Serial.print(c_error[0],DEC); Serial.print("\t");
  Serial.println(pwm,DEC);
}

