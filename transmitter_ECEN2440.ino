/*
 * --------------------Constants----------------------
 */

// baud rate
const long baud = 50;

// message length
const uint8_t msgLength = 3;

// start/ stop characters 
const uint16_t Start = 0xF000; //1111_0000_0000_0000
const uint16_t Stop = 0xFF00;  //1111_1111_0000_0000

// post message delay
const long postDelay = 1000;

// analog pins for reading joystick voltage
const int ver = A0;
const int hor = A1;
const int sw = A2;

/*
 * -------------------Structures--------------------
 */

//struct to hold data
struct data{
  uint16_t ver;
  uint16_t hor;
  uint16_t sw;
};

/*
 * -------------------Functions--------------------
 */

//takes in a data struct and fills the struct from the analog sources
//returns nothing
void readPins(data * D){
  D->ver = analogRead(ver);
  D->hor = analogRead(hor);
  D->sw = analogRead(sw);
}

//takes in a two byte variable
//returns void, updates array LSByte first
void parseData(uint16_t twoBytes, uint8_t * arr){
  //parse the 16 bit value into two 8 bit values LSB first
  arr[1] = static_cast<uint8_t>((twoBytes & 0xFF00) >> 8);
  arr[0] = static_cast<uint8_t>(twoBytes & 0x00FF);
}

/*
 * Least Significant Byte first, Least Significant Bit first
 * start   len msg1    msg2    msg3    stop
 * 00__0F__30__LS__MS__LS__MS__LS__MS__00__FF
 */
void sendData(data * D){
  //two byte arrays to send
  uint8_t st[2] = {0,0};
  uint8_t sp[2] = {0,0};
  uint8_t ver[2] = {0,0};
  uint8_t hor[2] = {0,0};
  uint8_t sw[2] = {0,0};

  //two byte ints parsed into bytes and placed in two byte arrays
  parseData(Start, st);
  parseData(Stop, sp);
  parseData(D->ver, ver);
  parseData(D->hor, hor);
  parseData(D->sw, sw);
  
  //send the start 
  Serial.write(st, sizeof(st));
  
  //send the length of the message
  Serial.write(msgLength);
  
  //send the message
  Serial.write(ver, sizeof(ver));
  Serial.write(hor, sizeof(hor));
  Serial.write(sw, sizeof(sw));

  //send the stop
  Serial.write(sp, sizeof(sp));
  
  delay(postDelay);
}
/*
 *--------------------Setup-------------------------
 */
void setup() {
  // open serial port for transmit, uses baud rate from above
  // 8 bit word length, no parity, one stop bit
  Serial.begin(baud, SERIAL_8N1);
}

/*
 *-------------------- Main-------------------------
 */

void loop() {
  // instantiate data struct
  data D = {0,0,0};
  
  //read analog pins
  readPins(&D);
  
  //send the data
  sendData(&D);
}
