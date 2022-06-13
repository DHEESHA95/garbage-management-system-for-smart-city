#include<SoftwareSerial.h>//gsm

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };//ethernet mac address
IPAddress ip(192,168,0,126); //<<< ENTER YOUR IP ADDRESS HERE!!!
EthernetServer server(80);

SoftwareSerial mySerial(7,8);//rx,tx (arduino receiver from 7th pin,transmitter 8th pin) 
 
String num ="000000000";
float gas;
int gaspin=A0;//gas sensor pin
const int trigPin = 2;//uv pin
const int echoPin = 3;//uv pin
int i=0;
long duration;
int distance;


void setup()//ethernet
{
  Ethernet.begin(mac, ip);
  server.begin();
pinMode(4, INPUT); //ir sensor
mySerial.begin(9600);//gsm boud rate
pinMode(trigPin, OUTPUT); //uv output
pinMode(echoPin, INPUT); // uv input
Serial.begin(9600);//gsm boud rate

}
void loop()//uv sensor
{

             digitalWrite(trigPin, LOW);
              delay(2);
              digitalWrite(trigPin, HIGH);
              delay(10);
              digitalWrite(trigPin, LOW);
              duration = pulseIn(echoPin, HIGH);//calculating duration
              distance= duration*0.034/2;//calculating distance
              Serial.print("Distance: ");
              Serial.println(distance);
             gas=analogRead(gaspin);//read the value of gas
            Serial.print("Gas Level= ");
            Serial.print(gas);
            Serial.println("ppm");//gas sensor unit
            delay(1000);
if(digitalRead(4)==1)//ir sensor (1 means high)
{
  i++;
  if(i>5)
  {
    Serial.println("ALERT! WASTE_BIN MIGHT BE FULL BECAUSE IR SENSOR HAS SENSED THE GIVEN LIMIT");
    mySerial.println("AT\r");
delay(500);
mySerial.println("AT+CMGF=1\r");//text mode for message
delay(500);
mySerial.print("AT+CMGS=\"");//number
mySerial.print(num);//number
mySerial.println("\"\r");//number
delay(500);
mySerial.println("ALERT! WASTE BIN MIGHT BE FULL BECAUSE IR SENSOR HAS SENSED THE GIVEN LIMIT");
delay(500);
mySerial.println((char)26);//send button
delay(500);
  }
}
            
           L1: if(distance<30)//ultrasonic sensor
            {
           Serial.println("THE WASTE BIN AT THE LOCATION XYZ IS FULL !!");
  mySerial.println("AT\r");
delay(500);
mySerial.println("AT+CMGF=1\r");
delay(500);
mySerial.print("AT+CMGS=\"");
mySerial.print(num);
mySerial.println("\"\r");
delay(500);
mySerial.println("THE WASTE BIN AT THE LOCATION XYZ IS FULL !!");
delay(500);
mySerial.println((char)26);
delay(500);
            }

             if(gas<350)//gas sensor
            {
           Serial.println("THE LOCATION XYZ HAS HAZARDOUS SMELL !!");
  mySerial.println("AT\r");
delay(500);
mySerial.println("AT+CMGF=1\r");
delay(500);
mySerial.print("AT+CMGS=\"");
mySerial.print(num);
mySerial.println("\"\r");
delay(500);
mySerial.println("THE LOCATION XYZ HAS HAZARDOUS SMELL !!");
delay(500);
mySerial.println((char)26);
delay(500);
            }
           
     EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/192.168.1.48 200 OK");
          client.println("Content-Type: text/html");
           client.println("Refresh: 1");
          client.println();
            // client.println("<cke:html><cke:head><cke:meta http-equiv='refresh' content='30'></cke:head><cke:body bgcolor=#FFFFFF>gas");
             client.println("<cke:html><cke:head><cke:meta http-equiv='refresh' content='30'></cke:head><cke:body bgcolor=#FFFFFF>   gas=");
             client.println(gas);
             client.println("<cke:html><cke:head><cke:meta http-equiv='refresh' content='30'></cke:head><cke:body bgcolor=#FFFFFF> basket level=");
            client.println(distance);
             client.println("</cke:body></cke:html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
  

}
