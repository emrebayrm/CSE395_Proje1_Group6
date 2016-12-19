int readyy = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char inByte = Serial.read();
    //Serial.print("Byte:");Serial.println(inByte);
    if(inByte == 'S') // check hand shake
    { 
      Serial.write("R");
      readyy = 1;
    }
    else if(inByte == 'F'){ // no connection
      Serial.println("Connection closed");
      readyy = 0;
      Serial.flush();
     }
    else if(inByte == 'N'){ // con opened
      readyy = 2;
      sendTestData();
      }
  }
}
void sendTestData(){

  if(readyy == 2 ){
    Serial.write("{X125Y50x150y250}");
    /*Serial.write("{X90Y29x50y320}");
    Serial.write("{X60Y45x100y115}");
    Serial.write("{X40Y72x150y210}");
    Serial.write("{X20Y92x200y300}");
    Serial.write("{X5Y120x375y340}");*/
  }
}
