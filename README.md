# Engr103 Final

Dependencies: Circuit Playground Express

Classic Blackjack style game, however, aces just count as 1. All output is on the board itself. The LEDs will light up:  
- Green equals 2  
- Blue equals 1  
- In the case of 21, the final LED will be white  

The switch shows either the player's hand or the dealer's, depending on its position, and the buttons are used to hit or stand.

### Input/Output Table

| Input              | Condition                                  | Output                                                                 |
|-------------------|--------------------------------------------|------------------------------------------------------------------------|
| Slide Switch       | LEFT (HIGH)                                | Shows player's hand on LEDs                                           |
| Slide Switch       | RIGHT (LOW)                                | Shows dealer's hand (1st card or total if game over)                  |
| Button A           | Pressed while switch is LEFT               | Adds a card to player hand                                            |
| Button B           | Pressed while switch is RIGHT              | Player stands, dealer begins playing                                  |
| Button A           | Pressed while game is over                 | Resets game                                                           |
| Hand Value         | Equals 21                                  | 9 green LEDs + 1 white LED                                            |
| Hand Value         | Greater than 21 (bust)                     | 9 green LEDs + 1 red LED                                              |
| Sound Enabled      | Slide switch RIGHT (LOW)                  | Beeps play during actions                                             |
| Invalid Button Use | Button A/B with switch in wrong position   | Red LED ring flashes as feedback                                      |

