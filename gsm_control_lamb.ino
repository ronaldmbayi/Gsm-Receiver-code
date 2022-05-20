#include <Servo.h>
#include <SoftwareSerial.h>
//sender phone number with country code
const String PHONE = "+254704696287";

//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2
#define rxPin A5
#define txPin A4
SoftwareSerial sim800(rxPin,txPin);
Servo myservo;
#define servopin 6

String smsStatus,senderNumber,receivedDate,msg;
boolean isReply = false;

void setup() {
 
  Serial.begin(115200);
  Serial.println("Arduino serial initialize");
  
  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");

 myservo.attach(servopin);
  

  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";

  sim800.print("AT+CMGF=1\r"); //SMS text mode
  delay(1000);
}

void loop() {

while(sim800.available()){
  parseData(sim800.readString());
}

while(Serial.available())  {
  sim800.println(Serial.readString());
}

} //main loop ends

//***************************************************
void parseData(String buff){
  Serial.println(buff);

  unsigned int len, index;

  //Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index+2);
  buff.trim();

  if(buff != "OK"){
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    
    buff.remove(0, index+2);
    
    if(cmd == "+CMTI"){
      //get newly arrived memory location and store it in temp
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length()); 
      temp = "AT+CMGR=" + temp + "\r"; 
      //get the message stored at memory location "temp"
      sim800.println(temp); 
    }
    else if(cmd == "+CMGR"){
      extractSms(buff);
      
      if(senderNumber == PHONE){
        doAction();
      }
    }
  //////////////////////////////////////////////////
  }
  else{
  //The result of AT Command is "OK"
  }
}

//************************************************************
void extractSms(String buff){
   unsigned int index;
   
    index = buff.indexOf(",");
    smsStatus = buff.substring(1, index-1); 
    buff.remove(0, index+2);
    
    senderNumber = buff.substring(0, 13);
    buff.remove(0,19);
   
    receivedDate = buff.substring(0, 20);
    buff.remove(0,buff.indexOf("\r"));
    buff.trim();
    
    index =buff.indexOf("\n\r");
    buff = buff.substring(0, index);
    buff.trim();
    msg = buff;
    buff = "";
    msg.toLowerCase();
}

void doAction(){
  if(msg == "TURN OFF"){  
    myservo.write(0);
    /*
    Reply("Relay 1 has been OFF");
    */
  }
  
  else if(msg == "TURN ON"){
    myservo.write(180);
    /*
    Reply("Relay 1 has been ON");
    */
  }
 
 
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";  
}
/*
void Reply(String text)
{
    sim800.print("AT+CMGF=1\r");
    delay(1000);
    sim800.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800.print(text);
    delay(100);
    sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("SMS Sent Successfully.");
}
*/
