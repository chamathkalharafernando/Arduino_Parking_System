#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize Servo
Servo myservo;

// Define IR sensor pins
int IR1 = 3; // Entry sensor
int IR2 = 2; // Exit sensor

// Flags for sensor status
int flag1 = 0;
int flag2 = 0;

// Car count variable
int carCount = 0; // Number of cars entered

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set IR sensors as inputs
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Initialize servo motor
  myservo.attach(4);
  myservo.write(100); // Gate closed position

  // Welcome message on LCD
  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check entry sensor
  if (digitalRead(IR1) == LOW && flag1 == 0) {
    flag1 = 1;
    if (carCount < 4) { // Check for available parking slots (assuming 4 slots)
      openGate();       // Immediately open the gate when car enters
      carCount++;       // Increment the car count when a car enters
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
      delay(3000);
      lcd.clear();
    }
  }

  // Check exit sensor
  if (digitalRead(IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    openGate();        // Open gate immediately when a car exits
    carCount--;        // Decrement car count when a car exits
    closeGate();       // Close the gate immediately after the car exits
    displayCarCount(); // Display the car count
  }

  // Reset flags after closing the gate
  if (flag1 == 1 && flag2 == 1) {
    flag1 = 0;
    flag2 = 0;
  }
}

// Function to open the gate
void openGate() {
  myservo.write(0); // Open gate position
  lcd.setCursor(0, 1);
  lcd.print(" Gate Opening  ");
  delay(500);  // Short delay to ensure gate opens, no need to wait for 2 seconds
  lcd.clear();
}

// Function to close the gate
void closeGate() {
  myservo.write(100); // Close gate position
  lcd.setCursor(0, 0);
  lcd.print("  Gate Closing  ");
  lcd.setCursor(0, 1);
  lcd.print(" Thank You!     ");
  delay(2000); // Keep the message for 2 seconds
  lcd.clear();
}

// Function to display the car count
void displayCarCount() {
  lcd.setCursor(0, 0);
  lcd.print("Cars Entered: ");
  lcd.print(carCount);  // Display the number of cars that have entered
  delay(3000);  // Display for 3 seconds before clearing
  lcd.clear();
}
