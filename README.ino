# Gsm-Receiver-code
These code of lines can used to set gsm module in receiver mode
#include <SoftwareSerial.h>

SoftwareSerial mygsm(3,2);
String msg="";
int index=0;
void setup() {
  // put your setup code here, to run once:
mygsm.begin(9600);
Serial.begin(9600);
mygsm.print("AT+CMGF=1");
}

void loop() {
  // put your main code here, to run repeatedly:
if(mygsm.available())
{
  while(mygsm.available())
  {
    recieved_msg(mygsm.readString());
    }
  }
}
void recieved_msg(String buff)
{
    index=buff.indexOf('"');
    msg=buff.substring(index);
    msg.trim();
    Serial.println("message"+msg);
    msg.toUpperCase();
  }
