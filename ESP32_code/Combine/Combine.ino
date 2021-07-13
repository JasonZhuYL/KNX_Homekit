#define RXD2 22
#define TXD2 23

#include <WiFi.h>
const char* ssid = "3102";
const char* password = "HXMZJT888";
WiFiServer server(8000);

int message[7];
void setup() {
  message[0] = 1;
  message[1] = 5;
  message[2] = 1;
  message[3] = 1;
  message[4] = 10;//A for main lamp
  message[5] = 1; //length of value (bit);
  message[6] = 1; //1 for on 0 for off
  message[7] = 8; //the XOR8 verification 
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,RXD2, TXD2);

  //Start to connect to WIFI 
    Serial.print("Connecting to ");Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");Serial.println("WiFi connected.");
    Serial.println("IP address: ");Serial.println(WiFi.localIP());
    server.begin();

}

int rx_byte;
String input;
char input_byte1;
char input_byte2;
byte veri;


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click to turn main light <a href=\"/a1\">on</a>/<a href=\"/a0\">off</a><br>");
            client.print("Click to turn led strip <a href=\"/b1\">on</a>/<a href=\"/b0\">off</a><br>");
            client.print("Click to turn spotlight <a href=\"/c1\">on</a>/<a href=\"/c0\">off</a><br>");
//            client.print("Click to turn led strip <a href=\"/b1\">on</a>/<a href=\"/b0\">off</a><br>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
//        Serial.println(currentLine);
        if (currentLine.endsWith("GET /a1")) {
          operation('a','1');
        }
        if (currentLine.endsWith("GET /a0")) {
          operation('a','0');
        }
        if (currentLine.endsWith("GET /b1")) {
          operation('b','1');
        }
        if (currentLine.endsWith("GET /b0")) {
          operation('b','0');
        }
        if (currentLine.endsWith("GET /c1")) {
          operation('c','1');
        }
        if (currentLine.endsWith("GET /c0")) {
          operation('c','0');
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

/*     Using Serial moniter to test
//  if(Serial.available()){
//    Serial.println();
//    input = Serial.readString();
//    Serial.print("Received: ");
//    Serial.println(input);
//    input_byte1 = input[0];
//    input_byte2 = input[1];
//    
//    operation(input_byte1,input_byte2);
//  }
*/
  if(Serial2.available()){
    rx_byte = Serial2.read();
    Serial.print(rx_byte, HEX);
    Serial.print(" ");
  }
}


void operation (int pos,int val){
  //pos is the ascii value of char 
  //pos value should be 97-107 (a-k)
  //val should be ascii value of 0/1 48-49
    beginning();
    veri = 0100;
    //Verification code until here is 00000100
    Serial2.write(pos-87);
    veri = B00000100 ^ pos-87;
    Serial2.write(1);
    veri = veri^1;
    Serial2.write(val-48);
    veri = veri^(val-48);
    //Verification code until here is 
    Serial2.write(veri);
}

void beginning(){
  for(int i = 0; i<4;i++){
      Serial2.write(message[i]);
  }
}
