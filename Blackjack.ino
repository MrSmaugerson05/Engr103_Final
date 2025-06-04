#include <Adafruit_CircuitPlayground.h>

#define LED_COUNT   10 //Number of NeoPixels
#define MAX_CARDS   LED_COUNT //Max number of cards per hand
#define WIN_VALUE   21 //Target value to win blackjack

int  playerCards [MAX_CARDS]; //Player hand values
int  dealerCards [MAX_CARDS]; //Dealer hand values
int  playerCardCount = 0; //Number of cards in player hand
int  dealerCardCount = 0; //Number of cards in dealer hand

bool playerTurn = true; //True if it's still player's turn
bool gameOver   = false; //True if the game is over

volatile bool soundEnabled = false; //Flag to track sound state from slide switch

//Updates the soundEnabled flag based on slide switch state
void switchISR() {
  soundEnabled = (digitalRead(7) == LOW); //Update soundEnabled when switch toggled
}

//Resets the game state and deals initial cards
void resetGame() {
  memset(playerCards, 0, sizeof(playerCards)); //Clear player hand
  memset(dealerCards, 0, sizeof(dealerCards)); //Clear dealer hand
  playerCardCount = 0; //Reset player card count
  dealerCardCount = 0; //Reset dealer card count
  playerTurn = true; //Start with player's turn
  gameOver = false; //Game is not over yet

  playerHit(); //Initial card draw
  playerHit(); //Second card
  dealerPlayInitial(); //Dealer gets 2 cards

  Serial.println("New Game"); //Print header
  Serial.print("Dealer shows: "); //Reveal first dealer card
  Serial.println(dealerCards[0]);

  updateDisplay(); //Update LEDs
}

//Adds a card to the player's hand and plays a tone if sound is enabled
void playerHit() {
  if (playerCardCount >= MAX_CARDS) return; //Don't exceed max

  int card = random(1, 11); //Generate random card 1-10
  playerCards[playerCardCount++] = card; //Add to player hand

  Serial.print("Player draws: ");
  Serial.print(card);
  Serial.print("   Total now: ");
  Serial.println(handValue(playerCards, playerCardCount));

  if (soundEnabled) {
    CircuitPlayground.playTone(440, 100); //Beep if sound on
  }
}

//Gives the dealer two starting cards
void dealerPlayInitial() {
  while (dealerCardCount < 2) {
    dealerCards[dealerCardCount++] = random(1, 11); //Dealer draws 2 cards
  }
}

//Dealer draws cards until the hand value is at least 17
void dealerPlay() {
  while (handValue(dealerCards, dealerCardCount) < 17 && dealerCardCount < MAX_CARDS) {
    int card = random(1, 11); //Draw new card
    dealerCards[dealerCardCount++] = card; //Add to dealer hand

    Serial.print("Dealer draws: ");
    Serial.print(card);
    Serial.print("   Total now: ");
    Serial.println(handValue(dealerCards, dealerCardCount));

    if (soundEnabled) {
      CircuitPlayground.playTone(330, 100); //Play lower beep
    }

    showValue(handValue(dealerCards, dealerCardCount)); //Show dealer hand
    delay(300);
  }

  showValue(handValue(dealerCards, dealerCardCount)); //Show final dealer hand
}

//Determines the winner and ends the game
void finishGame() {
  dealerPlay(); //Let dealer finish

  int pTotal = handValue(playerCards, playerCardCount); //Get player score
  int dTotal = handValue(dealerCards, dealerCardCount); //Get dealer score

  Serial.print("Player Final: ");
  Serial.println(pTotal);
  Serial.print("Dealer Final: ");
  Serial.println(dTotal);

  if (pTotal > WIN_VALUE) {
    sadBeep(); //Player bust
  } else if (dTotal > WIN_VALUE || pTotal > dTotal) {
    triumphantBeep(); //Player wins
  } else {
    sadBeep(); //Dealer wins or tie
  }

  gameOver = true; //End game
}

//Returns the total value of the given hand
int handValue(int *hand, int count) {
  int total = 0;
  for (int i = 0; i < count; i++) total += hand[i]; //Sum hand
  return total;
}

//Updates the LED display based on switch position
void updateDisplay() {
  if (digitalRead(7) == HIGH) {
    showPlayerScore(); //Switch left, show player
  } else {
    showDealerScore(); //Switch right, show dealer
  }
}

//Displays the current player hand value
void showPlayerScore() {
  showValue(handValue(playerCards, playerCardCount)); //Display player score
}

//Displays the dealer's hand value depending on game state
void showDealerScore() {
  if (!gameOver) {
    showValue(dealerCards[0]); //Only show first dealer card
  } else {
    showValue(handValue(dealerCards, dealerCardCount)); //Show total if game over
  }
}

//Lights up NeoPixels to represent the hand value
void showValue(int value) {
  CircuitPlayground.clearPixels(); //Reset LEDs

  bool bust = (value > WIN_VALUE); //Check bust
  if (value >= WIN_VALUE) value = WIN_VALUE; //Cap at 21

  if (value == WIN_VALUE) {
    for (int i = 0; i < LED_COUNT - 1; i++) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0); //Green for 2 pts
    }
    if (bust) {
      CircuitPlayground.setPixelColor(LED_COUNT - 1, 255, 0, 0); //Red = bust
    } else {
      CircuitPlayground.setPixelColor(LED_COUNT - 1, 255, 255, 255); //White = 21
    }
  } else {
    int greens = value / 2; //2 pts per green
    int blues  = value % 2; //1 pt if odd

    for (int i = 0; i < LED_COUNT; i++) {
      if (i < greens)
        CircuitPlayground.setPixelColor(i, 0, 255, 0); //Green LED
      else if (i < greens + blues)
        CircuitPlayground.setPixelColor(i, 0, 0, 255); //Blue LED
      else
        CircuitPlayground.setPixelColor(i, 0, 0, 0); //Off
    }
  }

  CircuitPlayground.strip.show(); //Update strip
}

//Flashes the LED ring red briefly to show an error
void flashRedRing() {
  for (int i = 0; i < LED_COUNT; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0); //Red flash
  }
  CircuitPlayground.strip.show();
  delay(200);
  updateDisplay(); //Restore view
}

//Plays a happy sound sequence for player victory
void triumphantBeep() {
  int notes[] = {523, 659, 784};
  for (int i = 0; i < 3; i++) {
    CircuitPlayground.playTone(notes[i], 150); //Ascending beep
    delay(150);
  }
}

//Plays a sad sound sequence for player loss
void sadBeep() {
  int notes[] = {392, 330, 262};
  for (int i = 0; i < 3; i++) {
    CircuitPlayground.playTone(notes[i], 200); //Descending beep
    delay(150);
  }
}

//Initializes game and hardware components
void setup() {
  CircuitPlayground.begin(); //Start library
  Serial.begin(9600); //Start serial

  pinMode(7, INPUT_PULLUP); //Set switch pin
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE); //Attach switch interrupt
  switchISR(); //Initialize sound

  randomSeed(analogRead(A5)); //Seed RNG
  resetGame(); //Start game
}

//Handles player input and game loop logic
void loop() {
  if (!gameOver) {
    if (playerTurn) {
      if (CircuitPlayground.leftButton()) {
        if (digitalRead(7) == HIGH) {
          playerHit(); //Draw card

          if (handValue(playerCards, playerCardCount) >= WIN_VALUE) {
            finishGame(); //Auto finish
          }
        } else {
          flashRedRing(); //Invalid input
        }
        delay(300); //Debounce
      }

      if (CircuitPlayground.rightButton()) {
        if (digitalRead(7) == LOW) {
          playerTurn = false; //Stand
          finishGame(); //Finish game
        } else {
          flashRedRing(); //Invalid input
        }
        delay(300); //Debounce
      }
    }
    updateDisplay(); //Refresh LEDs
  } else {
    if (CircuitPlayground.leftButton()) {
      resetGame(); //Restart game
      delay(500); //Debounce
    }
  }
}