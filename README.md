# Ping Pong Game Final Project for ENGR 103.

## Rules for the Pong Game
**Objective:** The player needs to prevent the "ball" (LED) from going off the ends by pressing buttons A or B to "bounce" it back.

**Ball Movement:** The ball moves automatically from one LED to another.

**Bouncing Back:** The player presses Button A to bounce the ball back left and Button B to bounce the ball back right.

**Winning Condition:** The game continues indefinitely and gets slightly faster as you progress up to level 5. With music getting increasingly faster for each level as well. 6 bounces at level 5 signify you winning the game.

**Losing Condition:** The game ends if the ball goes off the end without being bounced back, or the button is pressed when the LED signifying the ball is already past the paddle. This will play the sad tune sound effect. 


## User Inputs
**Input Button 4:** Pressing Button while the White LED signifying the ball is at LED number 3 will make the ball or the White LED move back in the other direction to 2 1 0 9 8 7.

**Input Button 5:** Pressing Button while the White LED signifying the ball is at LED number 8 will make the ball or the White LED move back in the other direction to 7 8 9 0 1 2.

**Slide Switch 7:** Slide Switch value changing to true will start the game and then once you lose you can use the slide switch to restart the game. This slideswitch can also be used to pause and save your score.


## Device Outputs
**Paddle LED:** A LED at number 3 and 8 that are colored purple to signify the paddle in ping pong that is used to hit the ping pong ball.

**White Ball LED:** A LED that goes back and forth between the LED numbers 3 2 1 10 9 8, and is colored white and signifies the ball.

**Happy Tune:** A tune that keeps going faster and faster as we progress through the levels and the player is still alive.

**Sad Tune:** A tune that keeps implies that the user failed and the game is over.

**Level Up Tune:** A tune that signifies that the user has reached a level up.

**Serial Manager Score and Level Output:** The serial manager displays your score and the level that you ended the game on after you lose or pause the game.

**Speech to Text:** Speech to text screams out OVERSPEED when you reach the final level of the game.


