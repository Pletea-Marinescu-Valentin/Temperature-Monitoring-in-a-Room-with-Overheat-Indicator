#include <TimerOne.h>

// Define pin numbers for various components
const int Data = 7;
const int Clock = 8;
const int Latch = 6;
const int LEDPin = 9;

// Define pin numbers for 7-segment display segments
const int SEG0 = 5;
const int SEG1 = 4;
const int SEG2 = 3;
const int SEG3 = 2;

// Counter variable for segment scanning
int cc = 0;
// Array to hold the value to be displayed on the 7-segment display
char Value[4];

// 7-segment display segment patterns
const char SegData[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Class for controlling the 7-segment display
class SevenSegmentDisplay {
private:
  // Pin variables
  int Data = 7;
  int Clock = 8;
  int Latch = 6;
  int LEDPin = 9;
  int SEG0 = 5;
  int SEG1 = 4;
  int SEG2 = 3;
  int SEG3 = 2;
public:
  // Constructor
  SevenSegmentDisplay(int Data, int Clock, int Latch, int LEDPin, int SEG0, int SEG1, int SEG2, int SEG3) : Data(Data), Clock(Clock), Latch(Latch), LEDPin(LEDPin), SEG0(SEG0), SEG1(SEG1), SEG2(SEG2), SEG3(SEG3) {}
  // Initialization function
  void initialize() {
    pinMode(Data, OUTPUT);
    pinMode(Clock, OUTPUT);
    pinMode(Latch, OUTPUT);
    pinMode(SEG0, OUTPUT);
    pinMode(SEG1, OUTPUT);
    pinMode(SEG2, OUTPUT);
    pinMode(SEG3, OUTPUT);
  }
  // Function to display a single digit on the 7-segment display
  void displayDigit(char d) {
    int i;
    // Loop through each bit of the digit data
    for (i = 0; i < 8; i++) {
      // Set Data pin based on the current bit
      if ((d & 0x80) == 0x80) {
        digitalWrite(Data, HIGH);
      } else {
        digitalWrite(Data, LOW);
      }
      d = d << 1; // Shift to the next bit
      // Clock pulse
      digitalWrite(Clock, LOW);
      digitalWrite(Clock, HIGH);
    }
    // Latch pulse
    digitalWrite(Latch, LOW);
    digitalWrite(Latch, HIGH);
  }
};

// Class for reading temperature from a sensor
class TemperatureSensor {
public:
  // Function to get the temperature in Celsius
  float getTemperature() {
    int tempReading = analogRead(A0);
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK);
    float tempC = tempK - 273.15;
    return tempC;
  }
};

// Initialize instances of SevenSegmentDisplay and TemperatureSensor
SevenSegmentDisplay display(Data, Clock, Latch, LEDPin, SEG0, SEG1, SEG2, SEG3);
TemperatureSensor sensor;

void setup() {
  Serial.begin(9600);
  display.initialize();

  cc = 0;
  Timer1.initialize(5000); // Initialize Timer1 with a 5-second interval
  Timer1.attachInterrupt(timerIsr); // Attach the timer ISR (interrupt service routine)
  
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // Read temperature from the sensor
  float tempC = sensor.getTemperature();
  // Convert temperature to Fahrenheit
  float tempF = (tempC * 9.0) / 5.0 + 32.0;

  // Format temperature value for display
  char displayValue[5];
  sprintf(displayValue, "%04d", (int)(tempC * 100)); // Format temperature value with 4 digits
  Serial.println(displayValue); // Print temperature value to serial monitor

  // Extract individual digits from the formatted value
  Value[0] = displayValue[0] & 0x0F;
  Value[1] = displayValue[1] & 0x0F;
  Value[2] = displayValue[2] & 0x0F;
  Value[3] = displayValue[3] & 0x0F;
  delay(200); // Delay for stability

  // Check if temperature is too high and turn on LED accordingly
  if (tempC > 25.0) {
    Serial.println("Too Hot!"); // Print a message to serial monitor
    digitalWrite(LEDPin, HIGH); // Turn on LED
  } else {
    digitalWrite(LEDPin, LOW); // Turn off LED
  }
}

// Interrupt service routine for timer
void timerIsr() {
  cc++; // Increment the counter
  if (cc == 5) {
    cc = 1; // Reset the counter if it reaches 5
  }
  Scanner(); // Call the scanning function
  TCNT0 = 0xCC; // Reset timer counter
}

// Function to scan and display digits on the 7-segment display
void Scanner() {
  switch (cc) {
    case 1:
      digitalWrite(SEG3, HIGH); // Enable the first digit
      display.displayDigit(SegData[Value[0]]); // Display the first digit
      digitalWrite(SEG0, LOW); // Disable other digits
      break;
    case 2:
      digitalWrite(SEG0, HIGH); // Enable the second digit
      display.displayDigit(SegData[Value[1]] | 0x80); // Display the second digit
      digitalWrite(SEG1, LOW); // Disable other digits
      break;
    case 3:
      digitalWrite(SEG1, HIGH); // Enable the third digit
      display.displayDigit(SegData[Value[2]]); // Display the third digit
      digitalWrite(SEG2, LOW); // Disable other digits
      break;
    case 4:
      digitalWrite(SEG2, HIGH); // Enable the fourth digit
      display.displayDigit(SegData[Value[3]]); // Display the fourth digit
      digitalWrite(SEG3, LOW); // Disable other digits
      break;
  }
}
