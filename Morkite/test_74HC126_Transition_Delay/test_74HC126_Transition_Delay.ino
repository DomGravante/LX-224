#define TX_ENABLE 2
#define TX_READ 3

void setup() {

  Serial.begin(115200);  // Computer Serial

  Serial.println("Starting");
  // put your setup code here, to run once:
  pinMode(TX_ENABLE, OUTPUT);
  pinMode(TX_READ, INPUT);
  digitalWrite(TX_ENABLE, HIGH);
}

int loopCount = 0;

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(TX_READ) == 1) {
    Serial.println("Took " + String(loopCount) + " loops");
    Serial.print(digitalRead(TX_READ));
    loopCount = -1;
    digitalWrite(TX_ENABLE, LOW);

    delay(10001);
    digitalWrite(TX_ENABLE, HIGH);
  }

  loopCount += 1;
  delayMicroseconds(1);
}
