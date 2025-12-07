#include <Keypad.h>
#include <Servo.h>

#define BUZZER_PIN 6
#define LED_GREEN 7
#define LED_RED 8
#define SERVO_PIN 9

Servo lockServo;

String password = "1234";
String inputPass = "";

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {10, 11, 12, 13};
byte colPins[COLS] = {A0, A1, A2, A3};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void beepWrong() {
  tone(BUZZER_PIN, 400, 200);
  delay(200);
  tone(BUZZER_PIN, 200, 200);
  delay(200);
}

void melodyOpen() {
  tone(BUZZER_PIN, 500, 150);
  delay(200);
  tone(BUZZER_PIN, 800, 150);
  delay(200);
  tone(BUZZER_PIN, 1200, 150);
  delay(200);
}

void openDoor() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  lockServo.write(90);
  melodyOpen();
  delay(2000);
  lockServo.write(0);
  digitalWrite(LED_GREEN, LOW);
}

void wrongPass() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  beepWrong();
  delay(1500);
  digitalWrite(LED_RED, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  lockServo.attach(SERVO_PIN);
  lockServo.write(0);

  Serial.println("Enter Password:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print(key);

    if (key == '#') {
      if (inputPass == password) {
        Serial.println(" -> Correct!");
        openDoor();
      } else {
        Serial.println(" -> Wrong!");
        wrongPass();
      }
      inputPass = "";
    } 
    else if (key == '*') {
      inputPass = "";
      Serial.println("\nCleared");
    } 
    else {
      inputPass += key;
    }
  }
}
