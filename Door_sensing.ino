#include <Servo.h>
Servo servo;

const int TRIG1 = 9;
const int ECHO1 = 8;
const int TRIG2 = 11;
const int ECHO2 = 10;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  servo.attach(6);
}                                         
int min = 10000;

int max = 0;
int angle = 0;
int peopleCount = 0; // 건물 안에 있는 사람 수


bool standIn = false;
bool standOut = false;

bool checkIn = false;
bool checkOut = false;

// unsigned long currentTime = millis();
unsigned long eventTime = 0; // 이벤트 발생 시간
unsigned long eventIn = 0; // 이벤트 발생 시간
unsigned long eventOut = 0; // 이벤트 발생 시간
unsigned long currentTime;
unsigned long checkTime;
unsigned int flag = 0;

void loop() {
  // if ((checkIn == true || checkOut == true) && (millis() - checkTime > 1000)){
  //   checkIn = false; 
  //   checkOut = false;
  // }

  checkIn = false; 
  checkOut = false;
  
  long duration1, distance1;
  long duration2, distance2;

  digitalWrite(TRIG1, LOW);                                                                                                                                                                                                                                                                      
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  duration1 = pulseIn(ECHO1, HIGH);

  digitalWrite(TRIG2, LOW);                                                                                                                                                                                                                                                                      
  delayMicroseconds(2);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);

  distance1 = duration1 * 17 / 1000;
  distance2 = duration2 * 17 / 1000;
 

 if (distance1 < 7 && distance2 >= 7 && standIn == false && standOut == false && millis() - eventOut >= 5000) {
    // 사람이 건물 안으로 들어온 경우
    servo.write(90);
    eventIn = millis();
    standIn = true;
    standOut = false;
    flag = 1;
    // delay(1000);
  }
  if (distance1 >= 7 && distance2 < 7 && standIn == true && standOut == false) {
    // 사람이 건물 안으로 들어오는 경우
      servo.write(90);
      peopleCount++;
      Serial.println("\n 사람이 들어왔습니다.");
      Serial.print("건물 안에 있는 사람 수: ");
      Serial.println(peopleCount);
      standIn = false;
      standOut = false;
      
      checkIn = true; // 건물 안으로 들어 옴!
      checkTime = millis();
      flag = 0;
      // delay(2000);
  }
  
  if (distance1 >= 7 && distance2 < 7 && standIn == false && standOut == false && millis() - eventIn >= 5000) {
    // 사람이 건물 밖으로 나가는 경우
    eventOut = millis();
    servo.write(90);
    standIn = false;
    standOut = true;
    flag = 1;
    // delay(1000);

    // Serial.println("사람이 들어왔습니다.");
  }
  if (distance1 < 7 && distance2 >= 7 && standIn == false && standOut == true) {
    // 사람이 건물 밖으로 나간 경우
      servo.write(90);
      if (peopleCount >= 1){
        peopleCount--;
      }
      Serial.println("사람이 나갔습니다.");
      Serial.print("건물 안에 있는 사람 수: ");
      Serial.println(peopleCount);
    
      standIn = false;
      standOut = false;

      checkOut = true;
      checkTime = millis();
      flag = 0;
      // delay(3000);
  }


  if (distance1 >= 7 && distance2 >= 7 && standIn == false && standOut == false){
    servo.write(0);
  }

  // StandIn 또는 StandOut 후 10초 지나면 reset
  if (distance1 >= 7 && distance2 >= 7 && standIn == true || standOut == true){
    // Serial.print(millis());
    if (((millis() - eventIn > 5000 && millis() - eventIn < 7000)|| (millis() - eventOut > 5000 && millis() - eventOut < 7000)) && flag == 1){
        standIn = false;
        standOut = false;
      }
  }
  
  // Serial.print("\nDIstance 1: ");
  // Serial.print(distance1);
  // Serial.print(" cm");

  // Serial.print("         DIstance 2: ");
  // Serial.print(distance2);
  // Serial.print(" cm");

  // Serial.print("       Number of people: ");
  // Serial.print(peopleCount);


  // Serial.print("    stand in:");
  // Serial.print(standIn);
  
  // Serial.print("    stand Out:");
  // Serial.print(standOut);

  // Serial.print("    check In:");
  // Serial.print(checkIn);

  // Serial.print("    check Out:");
  // Serial.print(checkOut);


  // Serial.print("       Number of people: ");

  Serial.print(peopleCount);
  Serial.print(' ');
  Serial.print(checkIn);
  Serial.print(' ');
  Serial.print(checkOut);
  Serial.println();
}