void setup() {
  Serial.begin(9600);
  
  while (!Serial);
  // wait for serial port to connect
}

void loop() {
        char incomingByte;
        // send data only when you receive data:
        if (Serial.available() > 0) {
                incomingByte = Serial.read();
                Serial.print("Got: ");
                Serial.println(incomingByte);
        }
        
}
