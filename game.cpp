const byte J1_X = A0;
const byte J1_Y = A1;
const byte J1_SW = 2;

const byte J2_X = A2;
const byte J2_Y = A3;
const byte J2_SW = 3;

void setup() {
  pinMode(J1_SW, INPUT_PULLUP);
  pinMode(J2_SW, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  int j1x = analogRead(J1_X);
  int j1y = analogRead(J1_Y);
  int j1sw = digitalRead(J1_SW) == LOW ? 1 : 0;

  int j2x = analogRead(J2_X);
  int j2y = analogRead(J2_Y);
  int j2sw = digitalRead(J2_SW) == LOW ? 1 : 0;

  Serial.print(j1x);
  Serial.print(",");
  Serial.print(j1y);
  Serial.print(",");
  Serial.print(j1sw);
  Serial.print(",");

  Serial.print(j2x);
  Serial.print(",");
  Serial.print(j2y);
  Serial.print(",");
  Serial.println(j2sw);

  delay(20);
}