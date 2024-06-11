#include <Adafruit_CircuitPlayground.h>

// Game parameters
int currentLevel = 1;
int bounceCount = 0;
int ballPosition[] = {0, 1, 2, 9, 8, 7};
int ballIndex = 0;
int gameScore = 0;
bool gameRunning = true;
const uint8_t spOVERSPEED[] PROGMEM = {0xA9,0x0B,0x7A,0x87,0x59,0xB7,0x94,0x3A,0x8A,0x59,0x11,0xDE,0x5A,0xAA,0xC8,0x6B,0x4D,0xA9,0x4B,0x2B,0x22,0xEF,0x51,0xA3,0x2E,0x2D,0x8F,0xBC,0xCB,0x1D,0x53,0x9B,0x3A,0xA2,0xF4,0xB4,0x90,0x1D,0x9A,0x69,0x5C,0x94,0x5C,0x75,0x2B,0xA3,0x4D,0x77,0x77,0xBB,0xAD,0x88,0xD1,0xB8,0x92,0xB6,0x94,0x32,0x25,0xC3,0x1C,0xEA,0x53,0xCA,0xDC,0x1C,0x6C,0xA8,0x4F,0x29,0xCB,0x24,0xCA,0x90,0x3E,0xAD,0x9C,0xA6,0x84,0xCC,0x9C,0x06,0xE0,0x47,0x55,0x07,0xFC,0xE2,0x62,0x80,0x3F,0x4B,0x0C,0xF0,0xBD,0x32,0x02,0xA7,0x28,0x55,0xA1,0x1A,0x20,0x80,0xE2,0xC2,0x5B,0xD9,0x1C,0xB3,0x96,0xAD,0x1E,0x55,0x8F,0x42,0xDC,0xFE,0xB8,0xD5,0xC3,0x0B,0xF2,0xE6,0xE3,0x56,0x8F,0x28,0x28,0x9B,0xB3,0x4A,0x33,0xA2,0x80,0x6C,0xCD,0x2A,0xCD,0x0C,0x02,0x72,0x39,0xAB,0x34,0xD3,0x29,0xF0,0xD6,0xA4,0xD2,0x74,0xAE,0xA4,0xED,0x0B,0x45,0xCB,0x74,0x55,0x8B,0x44,0x66,0xBD,0x50,0x95,0xAD,0x6C,0xA9,0x4C,0xDD,0x88,0x72,0x64,0xE7,0xFF,0x07};


// State flags
volatile bool stateButtonLeft = false;
volatile bool stateButtonRight = false;
bool slideSwitchState = false;

// Tunes
const int happyTune[] = {330, 392, 349, 294, 262, 392}; 
const int levelUpTune[] = {392, 440, 494};
const int sadTune[] = {262, 150, 150, 150, 262, 150};

void setup() {
  CircuitPlayground.begin();

  // Play happy tune
  playTune(happyTune, sizeof(happyTune) / sizeof(happyTune[0]));
  delay(5000);

    Serial.println("Your reached level: 1");

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(4), stateButtonLeftHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), stateButtonRightHandler, FALLING);

  // Initial LED setup
  CircuitPlayground.setPixelColor(2, 128, 0, 128); // Platform LED 
  CircuitPlayground.setPixelColor(7, 128, 0, 128); // Platform LED
  CircuitPlayground.setPixelColor(ballPosition[ballIndex], 255, 255, 255); // Ball LED

  // Save initial slide switch state
  slideSwitchState = CircuitPlayground.slideSwitch();
}

void loop() {
  bool currentSlideSwitchState = CircuitPlayground.slideSwitch();
  if (currentSlideSwitchState != slideSwitchState) {
    slideSwitchState = currentSlideSwitchState;
    restartGame();
  }


  if (gameRunning) {
    moveBall();
  
  }
}

void moveBall() {
  CircuitPlayground.clearPixels(); 
  CircuitPlayground.setPixelColor(2, 128, 0, 128); // Re-set platform LED
  CircuitPlayground.setPixelColor(7, 128, 0, 128); // Re-set platform LED

  ballIndex++;
  if (ballIndex >= sizeof(ballPosition) / sizeof(ballPosition[0])) {
    ballIndex = 0;
  }

  CircuitPlayground.setPixelColor(ballPosition[ballIndex], 255, 255, 255); // Set new ball position

  delay(500 - (currentLevel - 1) * 100); // Adjust speed based on level

  // Check if ball hits the platfor
  if ((ballIndex == 2 && stateButtonLeft) || (ballIndex == 5 && stateButtonRight) && gameRunning == true ) {
    bounceCount++;
    gameScore = gameScore + 10;
    stateButtonLeft = false;  // Reset the state
    stateButtonRight = false; // Reset the state

    if (bounceCount % 6 == 0) {
      currentLevel++;
    Serial.print("Your reached level: ");
    Serial.println(currentLevel);
      gameRunning = false; // Pause the game
      playTune(levelUpTune, sizeof(levelUpTune) / sizeof(levelUpTune[0])); // Play the happy tune
      gameRunning = true; // Resume the game
    }
  } else if ((ballIndex == 2 && !stateButtonLeft) || (ballIndex == 7 && !stateButtonRight)) {
    gameRunning = false;
    playTune(sadTune, sizeof(sadTune) / sizeof(sadTune[0]));
    Serial.print("Your score was: ");
    Serial.println(gameScore);
  }

  // Check win condition
    if (currentLevel == 5 && bounceCount >= 5) {
      gameRunning = false;
      playTune(happyTune, sizeof(happyTune) / sizeof(happyTune[0]));
	    CircuitPlayground.speaker.say(spOVERSPEED);
      Serial.print("Your score was: ");
    Serial.println(gameScore);
    }
}

void playTune(const int *tune, int length) {
  for (int i = 0; i < length; i++) {
    CircuitPlayground.playTone(tune[i], 200);
  }
}

void stateButtonLeftHandler() {
  stateButtonLeft = true;
}

void stateButtonRightHandler() {
  stateButtonRight = true;
}

void restartGame() {
  currentLevel = 1;
  bounceCount = 0;
  ballIndex = 0;
  gameRunning = true;
  stateButtonLeft = false;
  stateButtonRight = false;

  // Initial LED setup
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(2, 128, 0, 128); // Platform LED (purple)
  CircuitPlayground.setPixelColor(7, 128, 0, 128); // Platform LED (purple)
  CircuitPlayground.setPixelColor(ballPosition[ballIndex], 255, 255, 255); // Ball LED (white)

  // Play happy tune to indicate restart
  playTune(happyTune, sizeof(happyTune) / sizeof(happyTune[0]));
}
