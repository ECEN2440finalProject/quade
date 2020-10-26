#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

//create instance of ASK driver
RH_ASK driver;

//variables for pins
int ver = A0;
int hor = A1;
int sw = A2;
uint16_t vertical = 0;
uint16_t horizontal = 0;
uint16_t Switch = 0;

void sendMessage(uint16_t msg){
    //parse to byte size
    uint8_t partA = static_cast<uint8_t>((msg & 0xFF00) >> 8);
    uint8_t partB = static_cast<uint8_t>(msg & 0x00FF);

    //send in bytes
    driver.send(&partA, sizeof(partA));
    driver.waitPacketSent();
    driver.send(&partB, sizeof(partA));
    driver.waitPacketSent();
}

void setup()
{
    Serial.begin(9600);    // Debugging only

    //initialize ASK driver
    if (!driver.init())
         Serial.println("init failed");    
}



void loop()
{
    //read anolog pins for vertical, horizontal and switch
    vertical = analogRead(ver);
    horizontal = analogRead(hor);
    Switch = analogRead(sw);
    
    //print values for debug
    Serial.print("Vertical = ");
    Serial.println(vertical);
    Serial.print("Horizontal = ");
    Serial.println(horizontal);
    Serial.print("Switch = ");
    Serial.println(Switch);
    
    //send each value
    sendMessage(vertical);
    sendMessage(horizontal);
    sendMessage(Switch);
    
    delay(1000);
}
