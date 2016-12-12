int readyy = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char inByte = Serial.read();
    if(inByte == 'S')
    { 
      Serial.write("R");
      readyy = 1;
    }
    else if(inByte == 'F')
      readyy = 0;
    else if(inByte == 'N')
      readyy = 2;

     sendTestData();
  }
}
void sendTestData(){

  if(readyy == 2 ){
    Serial.write("{X56.56Y87.87x541y661}");
    Serial.write("X22.56Y45.87x512y512!}");
    Serial.write("X21.56Y123.87x441y511!}");
    Serial.write("X78.56Y843.87x312y722!}");
    Serial.write("X99.56Y876.87x141y412!}");
    Serial.write("X234.56Y456.87x251y332!}");
  }
}
