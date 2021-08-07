#include <arduino_homekit_server.h>
int message[7];
byte veri;

void KNXinit(){
  message[0] = 1;
  message[1] = 5;
  message[2] = 1;
  message[3] = 1;
  message[4] = 1;//A for main lamp
  message[5] = 1; //length of value (bit);
  message[6] = 1; //1 for on 0 for off
  message[7] = 8; //the XOR8 verification 
}
void operation (char pos,char val){
  //pos is the ascii value of char 
  //pos value should be 97-107 (a-k)
  //val should be ascii value of 0/1 48-49
    for(int i = 0; i<4;i++){
      Serial1.write(message[i]);
    } 
  veri = 0100;
    //Verification code until here is 00000100
    Serial1.write(pos-87);
    veri = B00000100 ^ (pos-87);
    Serial1.write(1);
    veri = veri^1;
    Serial1.write(val-48);
    veri = veri^(val-48);
    //Verification code until here is 
    Serial1.write(veri);
}

void setup() {
  // put your setup code here, to run once:
  homekit_storage_reset();
  Serial.begin(115200);
//  Serial.swap();
  Serial1.begin(115200);
  KNXinit();
}
String input;
char input_byte1;
char input_byte2;
void loop() {
  if(Serial.available()){
    input = Serial.readString();
    Serial.print("Received: ");
    Serial.println(input);
    input_byte1 = input[0];
    input_byte2 = input[1];
//    operation(input_byte1,input_byte2);
    Serial1.write(1);
  }
  delay(500);

}
