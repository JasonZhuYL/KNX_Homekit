#define RXD2 22
#define TXD2 23

void operation (int pos,int val);
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

 
}

int rx_byte;
String input;
char input_byte1;
char input_byte2;
byte veri;


void loop() {
  if(Serial.available()){
    Serial.println();
    input = Serial.readString();
    Serial.print("Received: ");
    Serial.println(input);
    input_byte1 = input[0];
    input_byte2 = input[1];
    
    operation(input_byte1,input_byte2);
  }
  if(Serial2.available()){
    rx_byte = Serial2.read();
    Serial.print(rx_byte, HEX);
    Serial.print(" ");
    
  }
}

/*     Using Serial moniter to test
//  
*/


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
