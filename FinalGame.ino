#include <Adafruit_CircuitPlayground.h>

// Game parameters
int currentLevel = 1;
int bounceCount = 0;
int ballPosition[] = {1, 2, 3, 10, 9, 8, 7};
int ballIndex = 0;
bool gameRunning = true;

// State flags
volatile bool stateButtonLeft = false;
volatile bool stateButtonRight = false;
bool previousSlideSwitchState = false;

// Tunes
const int happyTune[] = {330, 392, 349, 294, 262, 392}; 
const int sadTune[] = {262, 150, 150, 150, 262, 150};

void setup() {
  CircuitPlayground.begin();

  // Play happy tune
  playTune(happyTune, sizeof(happyTune) / sizeof(happyTune[0]));
  delay(5000);

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(4), stateButtonLeftHandler, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), stateButtonRightHandler, FALLING);

  // Initial LED setup
  CircuitPlayground.setPixelColor(2, 128, 0, 128); // Platform LED (purple)
  CircuitPlayground.setPixelColor(7, 128, 0, 128); // Platform LED (purple)
  CircuitPlayground.setPixelColor(ballPosition[ballIndex], 255, 255, 255); // Ball LED (white)

  // Save initial slide switch state
  previousSlideSwitchState = CircuitPlayground.slideSwitch();
}

void loop() {
  bool currentSlideSwitchState = CircuitPlayground.slideSwitch();
  if (currentSlideSwitchState != previousSlideSwitchState) {
    previousSlideSwitchState = currentSlideSwitchState;
    restartGame();
  }

  if (gameRunning) {
    moveBall();

    // Check win condition
    if (currentLevel == 5 && bounceCount == 10) {
      gameRunning = false;
      playTune(happyTune, sizeof(happyTune) / sizeof(happyTune[0]));
    }
  }
}

void moveBall() {
  CircuitPlayground.clearPixels(); // Clear all previous ball positions
  CircuitPlayground.setPixelColor(2, 128, 0, 128); // Re-set platform LED (purple)
  CircuitPlayground.setPixelColor(7, 128, 0, 128); // Re-set platform LED (purple)

  ballIndex++;
  if (ballIndex >= sizeof(ballPosition) / sizeof(ballPosition[0])) {
    ballIndex = 0;
  }

  CircuitPlayground.setPixelColor(ballPosition[ballIndex], 255, 255, 255); // Set new ball position

  delay(500 - (currentLevel - 1) * 25); // Adjust speed based on level

  // Check if ball hits the platform
  if ((ballIndex == 2 && stateButtonLeft) || (ballIndex == 6 && stateButtonRight)) {
    bounceCount++;
    stateButtonLeft = false;  // Reset the state
    stateButtonRight = false; // Reset the state

    if (bounceCount % 10 == 0) {
      currentLevel++;
    }
  } else if ((ballIndex == 2 && !stateButtonLeft) || (ballIndex == 6 && !stateButtonRight)) {
    gameRunning = false;
    playTune(sadTune, sizeof(sadTune) / sizeof(sadTune[0]));
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
