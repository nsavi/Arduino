#include <LiquidCrystal.h>
#include <Keypad.h>

// Display
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Keypad setup 
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'.', '0', '=', '/'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

const int prices[10] = {0, 100, 150, 50, 70, 30, 40, 60, 50, 80}; // Prices
int totalAmount = 0;
int tableNumber = 0;
bool waitingForTableNumber = true;
bool serviceFinished = false;
bool motionDetected = false; // Motion detection flag

const int motionSensorPin = 6;

void showSplashScreen() {
  lcd.clear();
  lcd.print("WELCOME TO CAFE");
  lcd.setCursor(0, 1); 
  lcd.print("Enter table no.");
}

void showMainMenu() {
  lcd.clear();
  lcd.print("1: Call waiter");
  lcd.setCursor(0, 1);
  lcd.print("2: Menu order");
  delay(2000);
  lcd.clear();
  lcd.print("3: View menu");
  delay(2000);
}

void showMenu() {
  lcd.clear();
  lcd.print("Enter menu items");
}

void showOrderOptions() {
  lcd.clear();
  lcd.print("1: Finish service");
  lcd.setCursor(0, 1);
  lcd.print("2: Order more");
}

void processMainMenu(char key) {
  if (key == '1') {
    lcd.clear();
    lcd.print("Calling waiter...");
    delay(3000);
    showMainMenu();
  } else if (key == '2') {
    showMenu();
  } else if (key == '3') {
    lcd.clear();
    lcd.print("1: Burger");
    lcd.setCursor(0, 1);
    lcd.print("2: Pizza");
    delay(2000);
    lcd.clear();
    lcd.print("3: Sandwich");
    lcd.setCursor(0, 1);
    lcd.print("4: Wrap");
    delay(2000);
    lcd.clear();
    lcd.print("5: Drinks");
    lcd.setCursor(0, 1);
    lcd.print("6: Ice cream");
    delay(2000);
    lcd.clear();
    lcd.print("7: Fries");
    lcd.setCursor(0, 1);
    lcd.print("8: Ice cream");
    delay(2000);
    lcd.clear();
    lcd.print("9: Noodles");
    delay(2000);
    showMainMenu();
  }
}

void processMenuOrder(char key) {
  lcd.clear();
  switch (key) {
    case '1':
      lcd.print("Burger ordered");
      totalAmount += prices[1];
      break;
    case '2':
      lcd.print("Pizza ordered");
      totalAmount += prices[2];
      break;
    case '3':
      lcd.print("Sandwich ordered");
      totalAmount += prices[3];
      break;
    case '4':
      lcd.print("Wrap ordered");
      totalAmount += prices[4];
      break;
    case '5':
      lcd.print("Drinks ordered");
      totalAmount += prices[5];
      break;
    case '6':
      lcd.print("Ice cream ordered");
      totalAmount += prices[6];
      break;
    case '7':
      lcd.print("Fries ordered");
      totalAmount += prices[7];
      break;
    case '8':
      lcd.print("Ice cream ordered");
      totalAmount += prices[8];
      break;
    case '9':
      lcd.print("Noodles ordered");
      totalAmount += prices[9];
      break;
    case '0':
      lcd.clear();
      lcd.print("Total amount = ");
      lcd.print(totalAmount);
      lcd.setCursor(0, 1);
      lcd.print("Table number = ");
      lcd.print(tableNumber);
      delay(3000);
      lcd.clear();
      lcd.print("Waiter will come to table");
      delay(3000);
      lcd.clear();
      lcd.print("1: Finish service");
      lcd.setCursor(0, 1);
      lcd.print("2: Order more");
      serviceFinished = true;
      break;
  }
  delay(1000);
  showMenu();
}

void processOrderOptions(char key) {
  if (key == '1') {
    totalAmount = 0;
    tableNumber = 0;
    waitingForTableNumber = true;
    showSplashScreen();
    serviceFinished = false;
  } else if (key == '2') {
    showMainMenu();
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear(); 
  pinMode(motionSensorPin, INPUT); 
}

void waitForMotion() {
  delay(100); 
  while (!motionDetected) {
    if (digitalRead(motionSensorPin) == HIGH) {
      motionDetected = true;
    }
  }
}


void loop() {
  waitForMotion();
  
  if (waitingForTableNumber) {
    showSplashScreen(); // Show the splash screen after motion is detected
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      tableNumber = key - '0';
      waitingForTableNumber = false;
      showMainMenu();
    }
  } else if (!serviceFinished) {
    char key = keypad.getKey();
    if (key) {
      static bool inMenu = false;
      if (!inMenu) {
        if (key == '1' || key == '2' || key == '3') {
          processMainMenu(key);
          if (key == '2') {
            inMenu = true;
          }
        }
      } else {
        processMenuOrder(key);
        if (key == '0') {
          inMenu = false;
          showOrderOptions();
        }
      }
    }
  } else {
    char key = keypad.getKey();
    if (key) {
      processOrderOptions(key);
    }
  }
}

