# computerGraphics
A game made with openGL for Bil 421 Homework 1

### Lanes
- There is 18 lanes.
- Each lane has a exact direction.
- Even number lanes from right to left, odds are in opposite direction.

### Vehicles
- Vehicles are created at random times.
- It is randomly created in 18 different lanes.
- Cars or trucks is also chosen randomly.
- If the agent hits, the game is over.

### Coins
- Coins are created at random times.
- Its lane and its position are choosen randomly.
- It has a certain time and disappears when time runs out.
- If the agent catches it earns 5 points.
- The time of the coin can be followed in the upper left.

### Agent
- When the game starts, the agent is at the bottom center.
- Agent can be moved with the arrow keys.
- Moving up and down gives 1 point.
- Score can be followed from the bottom left.
- The agent cannot go down while looking up. (or vice versa)

### Keyboard and Mouse
- Q and q keys  -> quit game
- Arrow keys    -> agent moves
- Left click    -> Start and stop game
- Right click   -> Stop and move one frame.
- Enter         -> power move
- 1             -> Easy mode
- 2             -> Normal mode
- 3             -> Hard mode

### Extra Features:
- The number of vehicles in the game can be followed instantly from right bottom.
- The remaining time of the coin can be tracked from the top left.
- With the Enter key, the agent can suddenly go to the end of the road. (Power move)
- If agent can go to the end of the road while in power move, he gets 3 times more points.
- The color of the vehicle that crashed becomes orange. (Normally blue)
- When the game is over, the background color fades.
- There are 3 game modes in the game. Easy, normal, hard.
- Game mode can be seen on the top right.
- Press 1 for easy mode, 3 for normal mode, 3 for hard mode.
- The game starts in normal mode.
- Hard mode brings 2x points.
