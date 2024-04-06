# Whack_a_mole_game
Game Description:
Each time the mole needs to appear, the micro-controller generates a random pair of (xm,ym) coordinates. These are the coor- dinate of where the mole appears at the current time step. The mole is rendered as a solid square of about 5x5 pixels on the LCD centered at the extracted coordinate, normalized to the size of the LCD.

The current (xb,yb) position of the ball on the touchscreen is computed, and reported on the LCD as an empty circle with a radius of about 10 pixels, and in the same coordinate space as the mole. The user can use the joystick to manually actuate the servos and move the ball along the x and y axis. When the circle on the LCD touches the mole, then the mole has been hit and the player is given points that count towards its total score. The points can be a constant, or depend on the time it took the player to reach the mole.

If the player is not able to reach the mole within a given amount of time (e.g. 2 seconds), then the mole disappears, and no points are given to the player.

