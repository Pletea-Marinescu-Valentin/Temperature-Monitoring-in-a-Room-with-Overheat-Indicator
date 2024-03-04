#include <TimerOne.h>

const int Data = 7;
const int Clock = 8;
const int Latch = 6;
const int LEDPin = 9;

const int SEG0 = 5;
const int SEG1 = 4;
const int SEG2 = 3;
const int SEG3 = 2;

int cc = 0;
char Value[4];

const char SegData[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
class SevenSegmentDisplay {
private:
  int Data = 7;
  int Clock = 8;
  int Latch = 6;
  int LEDPin = 9;
  int SEG0 = 5;
  int SEG1 = 4;
  int SEG2 = 3;
  int SEG3 = 2;
public:
  SevenSegmentDisplay(int Data, int Clock, int Latch, int LEDPin, int SEG0, int SEG1, int SEG2, int SEG3) : Data(Data), Clock(Clock), Latch(Latch), LEDPin(LEDPin), SEG0(SEG0), SEG1(SEG1), SEG2(SEG2), SEG3(SEG3) {}
  void initialize() {
    pinMode(Data, OUTPUT);
    pinMode(Clock, OUTPUT);
    pinMode(Latch, OUTPUT);
    pinMode(SEG0, OUTPUT);
    pinMode(SEG1, OUTPUT);
    pinMode(SEG2, OUTPUT);
    pinMode(SEG3, OUTPUT);
  }

  void displayDigit(char d) {
    int i;

    for (i = 0; i < 8; i++) {
      if ((d & 0x80) == 0x80) {
        digitalWrite(Data, HIGH);
      } else {
        digitalWrite(Data, LOW);
      }
      d = d << 1;

      digitalWrite(Clock, LOW);
      digitalWrite(Clock, HIGH);
    }
    digitalWrite(Latch, LOW);
    digitalWrite(Latch, HIGH);
  }
};

class TemperatureSensor {
public:
  float getTemperature() {
    int tempReading = analogRead(A0);
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK);

    float tempC = tempK - 273.15;
    return tempC;
  }
};

SevenSegmentDisplay display(Data, Clock, Latch, LEDPin, SEG0, SEG1, SEG2, SEG3);
TemperatureSensor sensor;

void setup() {
  Serial.begin(9600);
  display.initialize();

  cc = 0;
  Timer1.initialize(5000);
  Timer1.attachInterrupt(timerIsr);

  pinMode(LEDPin, OUTPUT);

}

void loop() {
  float tempC = sensor.getTemperature();
  float tempF = (tempC * 9.0) / 5.0 + 32.0;

  char displayValue[5];
  sprintf(displayValue, "%04d", (int)(tempC * 100));
  Serial.println(displayValue);

  Value[0] = displayValue[0] & 0x0F;
  Value[1] = displayValue[1] & 0x0F;
  Value[2] = displayValue[2] & 0x0F;
  Value[3] = displayValue[3] & 0x0F;
  delay(200);

  if (tempC > 25.0) {
    Serial.println("Prea cald!");
    digitalWrite(LEDPin, HIGH);
  } else {
  digitalWrite(LEDPin, LOW);
  }
}

void timerIsr() {
  cc++;
  if (cc == 5) {
    cc = 1;
  }
  Scanner();
  TCNT0 = 0xCC;
}

void Scanner() {
  switch (cc) {
    case 1:
      digitalWrite(SEG3, HIGH);
      display.displayDigit(SegData[Value[0]]);
      digitalWrite(SEG0, LOW);
      break;
    case 2:
      digitalWrite(SEG0, HIGH);
      display.displayDigit(SegData[Value[1]] | 0x80);
      digitalWrite(SEG1, LOW);
      break;
    case 3:
      digitalWrite(SEG1, HIGH);
      display.displayDigit(SegData[Value[2]]);
      digitalWrite(SEG2, LOW);
      break;
    case 4:
      digitalWrite(SEG2, HIGH);
      display.displayDigit(SegData[Value[3]]);
      digitalWrite(SEG3, LOW);
      break;
  }
}
