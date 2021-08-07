void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  Serial.println("LED On!");
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  Serial.println("LED Off!");
  Serial.write(1);
  Serial.write(2);
  Serial.write(49);
  Serial.print("hello");
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  
}
