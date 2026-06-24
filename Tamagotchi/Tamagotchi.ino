
//Rotary Encoder (traced from schematics)
#define ROT_A 8 // Encoder A -> MCU Pin 9 -> GPIO8
#define ROT_B 20 // Encoder B -> MCU Pin 8 -> GPIO20
#define ROT_S1 21 // Encoder SW1 -> MCU Pin 7 -> GPIO21

//Buttons (traced from schematics)
#define BTN1 4 // SW4 -> MCU Pin 3 -> GPIO4 top
#define BTN2 3   // SW3 -> MCU Pin 2 -> GPIO3 middle
#define BTN3 2   // SW2 -> MCU Pin 1 -> GPIO2 bottom



// I2C OLED (traced from schematics)
#define SDA_PIN 6  // OLED SDA -> MCU Pin 5 -> GPIO6
#define SCL_PIN 7  // OLED SCL -> MCU Pin 6 -> GPIO7

// LEDS (traced from schematics)
#define LED_1 10 // D1 -> MCU 11 -> GPIO10
#define LED_2 9 // D2 -> MCU 10 -> GPIO9

// Buzzer (traced from schematic)
#define BUZZER_PIN 5   // Buzzer signal → MCU Pin 4 → GPIO5

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Morse patterns for each status alert
const char* morse_hungry = ". .- -"; // EAT
const char* morse_play = "-.-- .- -.--";  // YAY
const char* morse_dirty = ". .-- .--";    // EWW
const char* morse_sleep = "--.. --.. --.."; //ZZZ

// State machine tracker variables
const char* currentPattern = nullptr;
int patternIndex = 0;
unsigned long morseMillis = 0;
bool ledIsOn = false;

//Screen Bath variables
volatile int CW = 0;
volatile int CCW = 0;
volatile int turnCounter = 0;
volatile unsigned long lastRun = 0;
int totalRotation;
int turnsLeft;

// Notes as frequencies in Hertz (A4=440)
// "NOTE_" prefix avoids conflict with Arduino's analog pin names (A0–A5)
const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int REST    = 0;   // silence

// --- Timing control ---
const int BPM = 120;               // beats per minute
const int QUARTER = 60000 / BPM;   // 1 beat = 60000ms / BPM
bool jinglePlayed = false; // Tracks if the 100% sound alert has already fired


struct Pet {
  int hunger;       // 0 to 100 (0 = starving, 100 = full)
  int happiness;    // 0 to 100 (0 = miserable, 100 = ecstatic)
  int energy;       // 0 to 100 (0 = exhausted, 100 = fully rested)
  int hygiene;      // 0 to 100 (0 = filthy, 100 = sparkling clean)
  float age; // total seconds the pet has been alive
};

Pet pet;

enum Screen {
  SCREEN_MAIN,
  SCREEN_FEED,
  SCREEN_PLAY,
  SCREEN_SLEEP,
  SCREEN_BATH
};

Screen currentScreen = SCREEN_MAIN;


void setup() {

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(ROT_S1, INPUT_PULLUP);
  pinMode(ROT_A, INPUT_PULLUP); 
  pinMode(ROT_B, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  
  attachInterrupt(digitaPinToInterrupt(ROT_A), updateRotation(), FALLING);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Tamagotchi Init...");
  display.display();
  delay(1000);

  pet.hunger = 70;
  pet.happiness = 70;
  pet.energy = 70;
  pet.hygiene = 70;
  pet.age = 0;
}

void loop() {
  checkButtons();       // 1. Read input
  updatePet();          // 2. Update state over time
  handleScreenLogic();  // 3. Process actions
  processMorseEngine();
  render();             // 4. Display results
  delay(100);           // 5. Short pause

}  
void processMorseEngine() {
  if (currentPattern == nullptr) return; // No alert running

  char currentSymbol = currentPattern[patternIndex];

  // If we reach the end of the string, stop the pattern
  if (currentSymbol == '\0') {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    currentPattern = nullptr; // Turn engine off
    return;
  }

  unsigned long currentMillis = millis();

  if (!ledIsOn) {
    // LED IS OFF: Determine how long it should turn ON for
    if (currentSymbol == '.' || currentSymbol == '-') {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    ledIsOn = true;
    morseMillis = currentMillis;

  
    unsigned long targetDuration = (currentSymbol == '.') ? 50 : 300;
    tone(BUZZER_PIN, 1000, targetDuration);
  } 
    else if (currentSymbol == ' ') {
      // Space! Keep LEDs off for a longer inter-word pause (400ms)
      if (currentMillis - morseMillis >= 400) {
        patternIndex++; // Move to next character
        morseMillis = currentMillis;
      }
    }
  } 
  else {
    // LED IS ON: Wait for the blink time to expire, then turn OFF
    unsigned long targetDuration = (currentSymbol == '.') ? 50 : 300;

    if (currentMillis - morseMillis >= targetDuration) {
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      ledIsOn = false;
      
      // Standard small gap between symbols inside the same phrase (100ms) 
      //makes the targetDuration a negative number so that there is a pause between symbools
      morseMillis = currentMillis + 100; 
      patternIndex++; // Step to the next dot or dash
    }
  }
}

void triggerAlert(const char* pattern) {
  currentPattern = pattern;
  patternIndex = 0;
  morseMillis = millis();
  ledIsOn = false;
}

unsigned long lastUpdate = 0; // check how mauch time has passed

void updatePet() {
  if (millis() - lastUpdate > 15000) {  // every 15 seconds
    pet.hunger -= 2;
    pet.happiness -= 2;
    pet.energy -= 2;
    pet.hygiene -= 2;

    if (pet.hunger < 0) pet.hunger = 0;
    if (pet.happiness < 0) pet.happiness = 0;
    if (pet.energy < 0) pet.energy = 0;
    if (pet.hygiene < 0) pet.hygiene = 0;

    pet.age += 0.25;
    lastUpdate = millis();
  }
}

unsigned long lastButtonPress = 0;

void checkButtons() {
  if (millis() - lastButtonPress < 200) return;  // debounce: ignore presses within 200ms
  
  if (currentPattern != nullptr) {
    currentPattern = nullptr;
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW); 
    noTone(BUZZER_PIN);
    
  }

  if (digitalRead(BTN1) == LOW) {
    currentScreen = SCREEN_FEED;
    triggerAlert(morse_hungry);
    lastButtonPress = millis();
  }
  else if (digitalRead(BTN2) == LOW) {
    currentScreen = SCREEN_PLAY;
    tiggerAlert(morse_play);
    lastButtonPress = millis();
  }
  else if (digitalRead(BTN3) == LOW) {
    currentScreen = SCREEN_SLEEP;
    tiggerAlert(morse_sleep);
    lastButtonPress = millis();
  }
  else if (digitalRead(ROT_S1) == LOW) {
    currentScreen = SCREEN_BATH;
    tiggerAlert(morse_dirty);
    lastButtonPress = millis();
  }

}


void handleScreenLogic() {
  switch(currentScreen) {

    case SCREEN_FEED:
      pet.hunger += 8;
      pet.energy += 5;
      if (pet.hunger > 100) pet.hunger = 100;
      if (pet.energy > 100) pet.energy = 100;
      if (pet.hunger >= 100 && !jinglePlayed) {
        full_jingle();
        jinglePlayed = true;
      }
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_PLAY:
      pet.happiness += 8;
      pet.energy -= 5;
      if (pet.happiness > 100) pet.happiness = 100;
      if (pet.energy < 0) pet.energy = 0;
      if (pet.happiness >= 100 && !jinglePlayed){
        happy_jingle();
        jinglePlayed = true;
      }
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_SLEEP:
      pet.energy += 10;
      if (pet.energy > 100) pet.energy = 100;
      if (pet.energy >= 100 && !jinglePlayed){
        energy_jingle();
        jinglePlayed = true;
      }
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_BATH:

     // press takes you to screen bath
     // turning knob --> turnCounter = 5 then hygiene and energy increase

      checkRotation();
      displayTurns();

      if (pet.hygiene > 100) pet.hygiene = 100;
      if (pet.energy < 0) pet.energy = 0;

      if (digitalRead(ROT_S1) == LOW) {
        currentScreen = SCREEN_MAIN; //if button pressed again THEN break TO MAIN SCREEN 
        delay(200);
      }
      if (pet.hygiene >= 100 && !jinglePlayed){
        clean_jingle();
        jinglePlayed = true;
      }
      break;

    case SCREEN_MAIN:
      jinglePlayed = false;
      break;  // do nothing, just display stats
  }
}


void updateRotation(){

  unsigned long currentMillis = millis();

  if (currentMillis - lastRun > 5){
   //make sure atleast 5ms passed since rotation

   lastRun = currentMills;

    if (digitalPinRead(ROT_B) == HIGH){
      Serial.print("Clockwise");
      CW += 1;
    } else if (digitalPinRead(ROT_B) == LOW){
      Serial.print("Counter Clockwise");
      CCW += 1;
    }
  }
}

void checkRotation(){

  totalRotations = CW + CCW;

  if (totalRotation >= 5){

    turnCounter += 1;

    CW = 0;
    CCW = 0;

    if (turnCounter >= 3) {

      pet.hygiene += 10;
      pet.energy -= 5;

      turnCounter = 0;
    }
    
  }
  
}

void displayTurns(){

  turnsLeft = 3 - turnCounter;
  
  display.setCursor(92, 22); 
  display.print("Scrubs"); 

  display.setCursor(92, 31);
  display.print("Left: ");
  display.print(turnsLeft);


}

// --- Helper function to play a note ---
void playTone(int frequency, int duration) {
  if (frequency == REST) {
    delay(duration); // silence
  } else {
    // tone() rapidly switches the pin between HIGH and LOW at 'frequency' Hz
    tone(BUZZER_PIN, frequency, duration);
    delay(duration * 1.3); // short gap between notes
  }
}

void happy_jingle (){

  playTone(NOTE_G4, QUARTER);
  playTone(NOTE_A4, QUARTER);
  playTone(NOTE_A4, QUARTER);
  playTone(NOTE_B4, QUARTER);

}

void full_jingle (){

  playTone(NOTE_F4, QUARTER);
  playTone(NOTE_B4, QUARTER);
  playTone(NOTE_B4, QUARTER);
  playTone(NOTE_G4, QUARTER);
  playTone(NOTE_A4, QUARTER);

}

void energy_jingle (){

  playTone(NOTE_F4, QUARTER);
  playTone(NOTE_A4, QUARTER);
  playTone(NOTE_C4, QUARTER);
  playTone(NOTE_C4, QUARTER);


}

void clean_jingle(){

  playTone(NOTE_G4, QUARTER);
  playTone(NOTE_G4, QUARTER);
  playTone(NOTE_D4, QUARTER);
  playTone(NOTE_B4, QUARTER);

}

void render() {

  display.clearDisplay();

  // Choose the right sprite based on pet stats
  const unsigned char* sprite;
  if (pet.energy < 40) {
    sprite = petSleep;
  } else if (pet.hygiene < 30) {
    sprite = petDirty;
  } else if (pet.hunger < 30 || pet.happiness < 30) {
    sprite = petSad;
  } else if (pet.happiness > 60 && pet.energy > 60) {
    sprite = petHappy;
  } else {
    sprite = petNeutral;
  }

  if (currentScreen == SCREEN_BATH){

    displayTurns();

  }

  // Draw the pet sprite (centered horizontally, near the top)
  display.drawBitmap(56, 2, sprite, 32, 32, SSD1306_WHITE);

  // Draw stat bars below the pet
  display.setTextSize(1);

  display.setCursor(0, 39);
  display.print("HUN ");
  drawBar(24, 41, pet.hunger);

  display.setCursor(0, 46);
  display.print("HAP ");
  drawBar(24, 48, pet.happiness);

  display.setCursor(0, 53);
  display.print("ENG ");
  drawBar(24, 55, pet.energy);

  display.setCursor(0, 60);
  display.print("HYG ");
  drawBar(24, 62, pet.hygiene);

  // Draw age
  display.setCursor(95, 2);
  display.print("Age: ");
  display.print(pet.age);

  // Button labels at the sides
  display.setCursor(2, 2);
  display.println("[Feed]");

  display.setCursor(2, 11);
  display.println("[Play]");

  display.setCursor(2, 20);
  display.println("[Sleep]");

  display.setCursor(92, 12);
  display.println("[Bath]");

  display.display();
}

// Draws a stat bar: empty rectangle with a filled portion based on value (0 to 100)
void drawBar(int x, int y, int value) {
  int barWidth = 90;
  int barHeight = 3;
  int fillWidth = map(value, 0, 100, 0, barWidth);

  display.drawRect(x, y, barWidth, barHeight, SSD1306_WHITE);       // outline
  display.fillRect(x, y, fillWidth, barHeight, SSD1306_WHITE);      // filled portion
}

void triggerAlert(const char* pattern) {
  currentPattern = pattern;
  patternIndex = 0;
  morseMillis = millis();
  ledIsOn = false;
}


const unsigned char PROGMEM petHappy[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00011011, 0b00000000, 0b00000000,
  0b00000111, 0b10000100, 0b00111100, 0b00000000,
  0b00001000, 0b01000100, 0b01000010, 0b00000000,
  0b00010011, 0b00111111, 0b10011001, 0b00010000,
  0b00010100, 0b10000000, 0b00100101, 0b00101000,
  0b00010000, 0b00000000, 0b00000001, 0b00010000,
  0b00010001, 0b10000000, 0b00110001, 0b00000000,
  0b00010010, 0b00000000, 0b00001001, 0b00000100,
  0b00010000, 0b00000000, 0b00000001, 0b00001010,
  0b00100001, 0b11000000, 0b01110000, 0b10000100,
  0b00100010, 0b00100000, 0b10001000, 0b10000000,
  0b00100100, 0b10010001, 0b00100100, 0b10000000,
  0b00100101, 0b01011111, 0b01010100, 0b10000000,
  0b01110101, 0b11010001, 0b01110101, 0b11000000,
  0b00100010, 0b10100000, 0b10101000, 0b10000000,
  0b01110001, 0b11001010, 0b01110001, 0b11000000,
  0b00000000, 0b00000100, 0b00000000, 0b00000000,
  0b00001000, 0b00000000, 0b00000010, 0b01111000,
  0b00000100, 0b00000000, 0b00000100, 0b10000100,
  0b00000011, 0b11111111, 0b11111001, 0b00000010,
  0b00000100, 0b00000000, 0b00000101, 0b01001010,
  0b00000100, 0b10000000, 0b00100101, 0b00110010,
  0b00000011, 0b00000000, 0b00011001, 0b00110010,
  0b00000001, 0b00000000, 0b00010000, 0b10000100,
  0b00101001, 0b00000000, 0b00010000, 0b10000011,
  0b01000100, 0b10000100, 0b00100000, 0b01000000,
  0b00010000, 0b01111011, 0b11000000, 0b01000000,
  0b01000100, 0b00000000, 0b00011011, 0b00110000,
  0b00101000, 0b00010100, 0b00011111, 0b00001111,
  0b00010010, 0b10001000, 0b00000100, 0b00000000,
  0b00010001, 0b00001000, 0b00000100, 0b00000000
};

const unsigned char PROGMEM petsad[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00001010, 0b00000000, 0b00000000,
  0b00000111, 0b10010101, 0b00111100, 0b00000000,
  0b00001000, 0b01000100, 0b01000010, 0b00000000,
  0b00010011, 0b00111111, 0b10011001, 0b00000000,
  0b00010100, 0b10000000, 0b00100101, 0b00000000,
  0b00010000, 0b00100000, 0b10000001, 0b00101010,
  0b00010000, 0b11000000, 0b01100001, 0b00101010,
  0b00010001, 0b00000000, 0b00010001, 0b00101010,
  0b00010000, 0b00000000, 0b00000001, 0b00001010,
  0b00100001, 0b11000000, 0b01110000, 0b10000010,
  0b00100010, 0b00100000, 0b10001000, 0b10000010, 
  0b00100100, 0b10010001, 0b00100100, 0b10000000,
  0b00100101, 0b01011111, 0b01010100, 0b10000000,
  0b01110101, 0b11010001, 0b01110101, 0b11000000,
  0b00100010, 0b10100100, 0b10101000, 0b10000000,
  0b01110001, 0b11001010, 0b01110001, 0b11000000,
  0b00000000, 0b10000000, 0b00100000, 0b00000000,
  0b00001000, 0b00000000, 0b00100010, 0b01111000,
  0b00000100, 0b00000000, 0b00000100, 0b10000100,
  0b00000011, 0b11111111, 0b11111001, 0b00000010,
  0b00000100, 0b00000000, 0b00000101, 0b00000010,
  0b00000100, 0b10000000, 0b00100101, 0b01001010,
  0b00000011, 0b00000000, 0b00011001, 0b00110010,
  0b00000001, 0b00000000, 0b00010000, 0b10000100,
  0b00000001, 0b00000000, 0b00010000, 0b10000011,
  0b00000000, 0b10000100, 0b00100000, 0b01000000,
  0b00000000, 0b01111011, 0b11000000, 0b01000000,
  0b00000000, 0b00000000, 0b00000000, 0b00110000,
  0b00000000, 0b00000000, 0b00000000, 0b00001111,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

const unsigned char PROGMEM petSleep[] = {
  0b00000000, 0b00001110, 0b00000000, 0b00000000,
  0b00000000, 0b00000100, 0b00000000, 0b00000000,
  0b00000111, 0b10001010, 0b00111100, 0b00000000,
  0b00001000, 0b01010001, 0b01000010, 0b00000000,
  0b00010011, 0b00111111, 0b10011001, 0b00111000,
  0b00010100, 0b10011111, 0b00100101, 0b00010000,
  0b00010000, 0b00000000, 0b00000001, 0b00100000,
  0b00010001, 0b10000000, 0b00110001, 0b01110000,
  0b00010010, 0b00000000, 0b00001001, 0b00000000,
  0b00010000, 0b00000000, 0b00000001, 0b00011100,
  0b00100001, 0b11000000, 0b01110000, 0b10001000,
  0b00100010, 0b00100000, 0b10001000, 0b10010000,
  0b00100100, 0b00010001, 0b00000100, 0b10111000,
  0b00100101, 0b01011111, 0b01010100, 0b10000000,
  0b01110100, 0b10010001, 0b00100101, 0b11000000,
  0b00100010, 0b00100000, 0b10001000, 0b10000000,
  0b01110001, 0b11001110, 0b01110001, 0b11000000,
  0b00000000, 0b00001110, 0b00000000, 0b00000000,
  0b00001000, 0b00000011, 0b00000010, 0b01111000,
  0b00000100, 0b00000001, 0b00000100, 0b10000100,
  0b00000011, 0b11111111, 0b11111001, 0b00000010,
  0b00000100, 0b00000000, 0b00000101, 0b00000010,
  0b00000100, 0b10000000, 0b00100101, 0b11011010,
  0b00000011, 0b00000000, 0b00011001, 0b00100010,
  0b00000001, 0b00000000, 0b00010000, 0b10000100,
  0b00000001, 0b00000000, 0b00010000, 0b10000011,
  0b00000000, 0b10000100, 0b00100000, 0b01000000,
  0b00000000, 0b01111011, 0b11000000, 0b01000000,
  0b00000000, 0b00000000, 0b00000000, 0b00110000,
  0b00000000, 0b00000000, 0b00000000, 0b00001111,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

const unsigned char PROGMEM petDirty[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000111, 0b10000000, 0b00111100, 0b00000000,
  0b00001000, 0b01000000, 0b01000010, 0b00000000,
  0b00010011, 0b00111111, 0b10011001, 0b00000010,
  0b00010100, 0b10000000, 0b00100101, 0b00000100,
  0b00010000, 0b00000000, 0b00000001, 0b00000100,
  0b00010001, 0b10000100, 0b00110001, 0b00011000,
  0b00010000, 0b01001000, 0b01000001, 0b00100000,
  0b00010000, 0b00100110, 0b10000001, 0b00100000,
  0b00100001, 0b11000000, 0b01110000, 0b10000010,
  0b00100010, 0b00100000, 0b10001000, 0b10001100,
  0b00100100, 0b10010001, 0b00100100, 0b10010000,
  0b00100101, 0b01011111, 0b01010100, 0b10000011,
  0b01110101, 0b11010001, 0b01110101, 0b11000100,
  0b00100010, 0b00100100, 0b10001000, 0b10011000,
  0b01110001, 0b11001010, 0b01110001, 0b11000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00001000, 0b10000000, 0b11000010, 0b00011100,
  0b00000101, 0b00000000, 0b00000100, 0b00100010,
  0b01100011, 0b11111111, 0b11111000, 0b01000001,
  0b00010100, 0b00000000, 0b00000100, 0b01100101,
  0b00000100, 0b10000000, 0b01100100, 0b01011001,
  0b00110011, 0b00010001, 0b10011000, 0b01011001,
  0b11000001, 0b00100000, 0b00010000, 0b00100010,
  0b00001101, 0b01100000, 0b00010000, 0b00100011,
  0b00010000, 0b10000100, 0b00100000, 0b00010000,
  0b00100000, 0b01111011, 0b11000000, 0b00010000,
  0b00000000, 0b00000000, 0b00000011, 0b10001000,
  0b00000000, 0b00000000, 0b00000000, 0b00000111,
  0b00000000, 0b00000000, 0b00000000, 0b11110000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

const unsigned char PROGMEM petNeutral[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000111, 0b10000000, 0b00111100, 0b00000000,
  0b00001000, 0b01000000, 0b01000010, 0b00000000,
  0b00010011, 0b00111111, 0b10011001, 0b00000000,
  0b00010100, 0b10000000, 0b00100101, 0b00000000,
  0b00010000, 0b00000000, 0b00000001, 0b00000000,
  0b00010001, 0b10000000, 0b00110001, 0b00000000,
  0b00010010, 0b00000000, 0b00001001, 0b00000000,
  0b00010000, 0b00000000, 0b00000001, 0b00000000,
  0b00100001, 0b11000000, 0b01110000, 0b10000000,
  0b00100010, 0b00100000, 0b10001000, 0b10000000,
  0b00100100, 0b10010001, 0b00100100, 0b10000000,
  0b00100101, 0b01011111, 0b01010100, 0b10000000,
  0b01110101, 0b11010001, 0b01110101, 0b11000000,
  0b00100010, 0b10100100, 0b10101000, 0b10000000,
  0b01110001, 0b11001010, 0b01110001, 0b11010100,
  0b00000000, 0b00000000, 0b00000000, 0b00001000,
  0b00001000, 0b00000000, 0b00000010, 0b01111000,
  0b00000100, 0b00000000, 0b00000100, 0b10000100,
  0b00000011, 0b11111111, 0b11111001, 0b00000010,
  0b00000100, 0b00000000, 0b00000101, 0b00000010,
  0b00000100, 0b10000000, 0b00100101, 0b01001010,
  0b00000011, 0b00000000, 0b00011001, 0b00110010,
  0b00000001, 0b00000000, 0b00010000, 0b10000100,
  0b00000001, 0b00000000, 0b00010000, 0b10000011,
  0b00000000, 0b10000100, 0b00100000, 0b01000000,
  0b00000000, 0b01111011, 0b11000000, 0b01000000,
  0b00000000, 0b00000000, 0b00000000, 0b00110000,
  0b00000000, 0b00000000, 0b00000000, 0b00001111,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

