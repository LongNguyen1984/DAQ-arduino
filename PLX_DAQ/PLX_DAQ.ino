//set ups 3 ananlog channels as outputs.
unsigned long time_interval_ms = 10;
boolean PLX_flag = 1; // for makerPlot, set this false, set for normal output no headers
boolean print_flag = 1; //tells if we print to serial  port

int pin_led13 = 13;
int pin_led12 = 12;
int pin_led11 = 11;
unsigned long time0_us = 0;
unsigned long time1_us = 0;
unsigned long time2_us = 0;

int chan1_pin = A0;
int chan2_pin = A1;
int chan3_pin = A2;

float chan1_val = 0;
float chan2_val = 0;
float chan3_val = 0;
float time_data_sec;

unsigned long counter1 = 0;
unsigned long counter2 = 0;

unsigned long time_interval_us=time_interval_ms*1000;


void printData() {
  // this function will print the data to the serial port
  digitalWrite(pin_led11, HIGH);
  if (PLX_flag == true) {
    Serial.print("DATA, ");
    Serial.print(counter2);
    Serial.print(", ");

    Serial.print(time_data_sec, 3);
    Serial.print(", ");
  }

  Serial.print(chan1_val);
  Serial.print(", ");

  Serial.print(chan2_val);
  Serial.print(", ");

  Serial.println(chan3_val);
  digitalWrite(pin_led11, LOW);
}


void testFastestWrite() {
  digitalWrite(pin_led13, LOW);
  digitalWrite(pin_led13, HIGH);
}


void take3ChanAveData() {
  // take data over average interval
  digitalWrite(pin_led13, HIGH);  // start taking data
  counter2 = 0;

  chan1_val = 0;
  chan2_val = 0;
  chan3_val = 0;

  time1_us = micros();
  do {
    digitalWrite(pin_led12, HIGH);
    chan1_val = analogRead(chan1_pin) + chan1_val;
    chan2_val = analogRead(chan2_pin) + chan2_val;
    chan3_val = analogRead(chan3_pin) + chan3_val;
    counter2 = counter2 + 1;
    digitalWrite(pin_led12, LOW);

  } while (micros() < (time0_us + time_interval_us * counter1));

  time_data_sec = ((micros() - time1_us) * 0.5 + time1_us - time0_us) / 1e6;
  chan1_val = chan1_val / counter2;
  chan2_val = chan2_val / counter2;
  chan3_val = chan3_val / counter2;

  digitalWrite(pin_led13, LOW);
  //delayMicroseconds(100);
}

void setup(){
  // initialize serial communication
  Serial.begin(38400);
  pinMode(pin_led13, OUTPUT);
  pinMode(pin_led12, OUTPUT);
  pinMode(pin_led11, OUTPUT);
  //noInterrupts();
  counter1 = 0;
  
  if (PLX_flag == true) {
    Serial.println("CLEARDATA");
    Serial.println("LABEL,NptsAveraged,Time(sec),Chan1,Chan2,Chan3");
  }
  digitalWrite(pin_led12, LOW);
  digitalWrite(pin_led13, LOW);
  time0_us = micros(); // start the timer
}


void loop() {
  counter1=counter1+1;  // counts the number of data points taken
  take3ChanAveData();
  if (print_flag == true) printData();
}
