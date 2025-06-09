# Engr103 Final

Dependencies: Circuit Playground Express

Classic Blackjack style game, however, aces just count as 1. All output is on the board itself. The LEDs will light up:  
- Green is 2  
- Blue is 1  
- If it's exactly 21, the final LED will be white
- If it's over 21, the final LED will be red

The sound plays when:
- Player wins a happy set of tones are played
- Player loses a sad set of tones are played


The switch shows either the player's hand or the dealer's, depending on its position, and the buttons are used to hit or stand.

# Input/Output Table

| Input              | Condition                                  | Output                                                                |
|--------------------|--------------------------------------------|-----------------------------------------------------------------------|
| Slide Switch       | Left                                       | Shows players hand on LEDs                                            |
| Slide Switch       | Right                                      | Shows dealers hand but just the first card                            |
| Button A           | Pressed while switch is RIGHT              | Adds a card to player hand                                            |
| Button B           | Pressed while switch is LEFT               | Player stands, dealer begins playing                                  |
| Button A           | Pressed while game is over                 | Resets game                                                           |
| Hand Value         | Equals 21                                  | 9 green LEDs + 1 white LED                                            |
| Hand Value         | More than 21                               | 9 green LEDs + 1 red LED                                              |
| Invalid Button Use | Button pressed while switch is incorrect   | Red LED ring flashes as feedback                                      |
