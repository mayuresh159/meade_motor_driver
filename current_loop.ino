// general parameters
// pwm parameters
const unsigned int max_count = 2000; // ICR1 value to decide the operating frequency of the pwm
const unsigned int up_sat = 1975; // upper saturation for pwm dutycycles
const unsigned int low_sat = 10; // lower saturation for pwm dutycycles
// current loop parameters
const float c_kp = 2.5;
const float c_ki = 2.5;
const float c_kd = 0.0;
const unsigned int c_loop_time = 10; // current loop time in ms
const int c_up_sat = 1000;
const int c_low_sat = 20;
// speed loop parameters 
const float v_kp = 4;
const float v_ki = 0.5;
const float v_kd = 0.0;
const unsigned int v_command = 15;

// pin placements
int motorpin_a = 9;
int motorpin_b = 10;
int csense_in = 0;
int up_count = 2;
int down_count = 3;

// global variables assignment
unsigned int count = 0;
unsigned int count_samp;
unsigned int pwm = 700;
unsigned int c_command = 400;
signed int c_error[3] = {0,0,0};
signed int v_error[3] = {0,0,0};
unsigned int c_loop_repeat = 0;

void setup()
{
  // initialize input output pins
  pinMode(motorpin_a,OUTPUT); digitalWrite(motorpin_a,LOW);
  pinMode(motorpin_b,OUTPUT); digitalWrite(motorpin_b,LOW);
  pinMode(up_count,INPUT);
  pinMode(down_count,INPUT);
  // initialize serial port
  Serial.begin(115200);
  // initialize external interrupt
  attachInterrupt(0,update_count_up,FALLING); 
  attachInterrupt(1,update_count_down,FALLING);
  // initialize pwm engine
  init_pwm_engine(); 
  delay(8000);
}

void loop()
{
  c_loop_repeat ++;
  if (c_loop_repeat = 10) {speed_loop(); c_loop_repeat = 0;}
  current_loop();
  set_pwm(pwm);
  delay(c_loop_time-5);
}

// external interrupt service routine
void update_count_up() { count++; }
void update_count_down() { count--; }

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

// current loop routine
void current_loop()
{
  unsigned int c_read;
  c_read = analogRead(csense_in);
  c_error[0] = c_command - c_read;
  c_pid(); 
}

void c_pid()
{
  float new_pwm;  
  new_pwm = (float)pwm + c_kp*(c_error[0] - c_error[1]) + c_ki*c_error[1] + c_kd*(c_error[0] - c_error[2]);   
  if (new_pwm >= up_sat) new_pwm = up_sat;
  if (new_pwm <= low_sat) new_pwm = low_sat;
  c_error[2] = c_error[1];
  c_error[1] = c_error[0];
  pwm = (int) new_pwm;
}


// speed loop routine
void speed_loop()
{
  count_samp = count;
  if (count_samp>v_command+200) count_samp = v_command;
  v_error[0] = v_command - count_samp;
  v_pid();
  purge_data();
  count = 0;
}

void v_pid()
{
  float new_c_command;  
  new_c_command = (float)c_command + v_kp*(v_error[0] - v_error[1]) + v_ki*v_error[1] + v_kd*(v_error[0] - v_error[2]);   
  if (new_c_command >= c_up_sat) new_c_command = c_up_sat;
  if (new_c_command <= c_low_sat) new_c_command = c_low_sat;
  v_error[2] = v_error[1];
  v_error[1] = v_error[0];
  c_command = (int) new_c_command;
}


// print data on serial port
void purge_data()
{
  //Serial.print("{1,T,"); Serial.print(count_samp,DEC); Serial.print("}"); Serial.print("\t");
  //Serial.print("{2,T,"); Serial.print(c_command,DEC); Serial.print("}"); Serial.print("\t");
  //Serial.print("{3,T,"); Serial.print(pwm,DEC); Serial.println("}");
  Serial.print(count_samp,DEC); Serial.print("\t");
  Serial.print(c_command,DEC); Serial.print("\t");
  Serial.println(pwm,DEC);
}



