#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= KEYPAD =================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ================= GAME =================
int birdRow;
int pipeCol;
int gapRow;

int score;
int lives;

bool gameStarted = false;
bool gameOver = false;

unsigned long lastUpdate = 0;
int gameSpeed = 450;


// ================= SETUP =================
void setup() {

  lcd.init();
  lcd.backlight();

  randomSeed(analogRead(A0));

  showStartScreen();
}


// ================= MAIN LOOP =================
void loop() {

  char key = keypad.getKey();

  // Start game
  if (!gameStarted && key == '*') {
    startGame();
  }

  // Flap
  if (gameStarted && !gameOver && key == '*') {
    birdRow = 0;
  }

  // Restart
  if (gameOver && key == '#') {
    startGame();
  }

  // Game update
  if (gameStarted && !gameOver) {

    if (millis() - lastUpdate >= gameSpeed) {

      lastUpdate = millis();

      updateGame();
      drawGame();
    }
  }
}


// ================= START SCREEN =================
void showStartScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("  FLAPPY BIRD");

  lcd.setCursor(0, 1);
  lcd.print("* START GAME");

}


// ================= START GAME =================
void startGame() {

  birdRow = 0;

  pipeCol = 15;

  gapRow = random(0, 2);

  score = 0;

  lives = 3;

  gameSpeed = 450;

  gameStarted = true;

  gameOver = false;

  lcd.clear();
}


// ================= UPDATE GAME =================
void updateGame() {

  // Gravity
  birdRow++;


  // Move pipe
  pipeCol--;


  // New pipe
  if (pipeCol < 0) {

    pipeCol = 15;

    gapRow = random(0, 2);

    score++;

    // Make game slightly faster
    if (gameSpeed > 200) {
      gameSpeed -= 10;
    }
  }


  // Bird hits top/bottom
  if (birdRow < 0 || birdRow > 1) {

    loseLife();

    return;
  }


  // Bird hits pipe
  if (pipeCol == 1) {

    if (birdRow != gapRow) {

      loseLife();

      return;
    }
  }
}


// ================= LOSE LIFE =================
void loseLife() {

  lives--;

  if (lives <= 0) {

    gameOver = true;

    showGameOver();

    return;
  }

  // Reset bird
  birdRow = 0;

  pipeCol = 15;

  gapRow = random(0, 2);

  showLifeLost();

}


// ================= LIFE LOST UI =================
void showLifeLost() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("  OOPS!");

  lcd.setCursor(0, 1);
  lcd.print("Lives: ");

  lcd.print(lives);

  delay(700);
}


// ================= DRAW GAME =================
void drawGame() {

  lcd.clear();


  // ================= BIRD =================

  lcd.setCursor(1, birdRow);
  lcd.print(">");


  // ================= PIPE =================

  if (pipeCol >= 0 && pipeCol < 16) {

    // Top pipe
    if (gapRow != 0) {

      lcd.setCursor(pipeCol, 0);

      lcd.print("|");
    }


    // Bottom pipe
    if (gapRow != 1) {

      lcd.setCursor(pipeCol, 1);

      lcd.print("|");
    }
  }


  // ================= SCORE UI =================

  lcd.setCursor(9, 0);

  lcd.print("S:");

  lcd.print(score);


  // ================= LIVES UI =================

  lcd.setCursor(9, 1);

  lcd.print("L:");

  lcd.print(lives);
}
void showGameOver() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   GAME OVER");

  lcd.setCursor(0, 1);

  lcd.print("Score:");

  lcd.print(score);

  delay(2000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Final Score:");

  lcd.print(score);

  lcd.setCursor(0, 1);
  lcd.print("# Restart");
}