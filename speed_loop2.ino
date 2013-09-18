// pwm limitations
const unsigned int max_count = 400;  // defines a operating pwm frequency of 8kHz 1000
const unsigned int pwm_up_sat = 375; 
const unsigned int pwm_low_sat = 25;

const unsigned int c_up_sat = 1000;
const unsigned int c_low_sat = 10;

// loop parameters
const unsigned int loop_time = 20;
unsigned int loop_repeat = 0;

// current loop parameters;
const float c_kp = 1, // 1
            c_ki = 0.975, // 0.975
            c_kd = 0.0;
           
// speed loop parameters
const float v_kp = 3, // 
            v_ki = 0.5,
            v_kd = 0.0;
            

// pin placements
int motorpin_a = 9;
int motorpin_b = 10;
int csense_in = 0;
int encoder_in_up = 2;
int encoder_in_down = 3;


// global variables
unsigned int pwm = 500;
unsigned int dir =  0; // motor direction and speed

unsigned int c_command = 700; // in terms of adc value for the given circuit
signed int c_error[3] = {0,0,0}; // current loop variables

unsigned int v_command = 10; // in terms of count differences in an iteration
signed int v_error[3] = {0,0,0}; // speed loop variables

unsigned long int pos_count[2] = {0,0};


void setup()
{
  pinMode(motorpin_a,OUTPUT); digitalWrite(motorpin_a,LOW);
  pinMode(motorpin_b,OUTPUT); digitalWrite(motorpin_b,LOW);
  pinMode(encoder_in_up,INPUT);
  pinMode(encoder_in_down,INPUT);
  attachInterrupt(0,update_count_up,FALLING);
  attachInterrupt(1,update_count_down,FALLING);
  init_pwm_engine();
  Serial.begin(115200);
  delay(8000);
}

void loop()
{
  loop_repeat++;
  if (loop_repeat == 1){v_loop(); loop_repeat=0;}
  //v_loop();
  c_loop();
  set_pwm();
  purge_data();
  delay(loop_time);
  
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

// count interrupts
void update_count_up() {pos_count[0]++;}
void update_count_down() {pos_count[0]--;}

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

// speed loop correction
void v_loop()
{
  unsigned int v_read;
  v_read = pos_count[0] - pos_count[1];
  pos_count[1] = pos_count[0];
  v_error[0] = v_command - v_read;
  v_pid();
  v_error[2] = v_error[1];
  v_error[1] = v_error[0];
}

void v_pid()
{
  float new_c_command;
  new_c_command = (float)c_command + v_kp*(v_error[0] - v_error[1]) + v_ki*v_error[1] + v_kd*(v_error[0]-v_error[2]);
  if(new_c_command >= c_up_sat) new_c_command = c_up_sat;
  if(new_c_command <= c_low_sat) new_c_command = c_low_sat;
  c_command = (int)new_c_command;
}


// print data on serial port
void purge_data()
{
  
  Serial.print((unsigned int)pos_count%65536,DEC); Serial.print("\t");
  Serial.print(v_command,DEC); Serial.print("\t");
  Serial.print(v_error[0],DEC); Serial.print("\t");
  Serial.print(c_command,DEC); Serial.print("\t");
  Serial.print(c_error[0],DEC); Serial.print("\t");
  Serial.println(pwm,DEC);
}

