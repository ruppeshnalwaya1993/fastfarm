fastfarm
========

A game submitted for intel app innovation challenge 2013

YOUTUBE link:
http://youtu.be/F_9mWhdrzTI

#FASTFARM - a puzzle game based on Conway's game of life

Installation:
One click installation using fastfarm.msi OR setup.exe
No additional drivers required for Lenovo AIO.


About the Game Development:
The game has been developed in C++ using Simple DirectMedia Layer(SDL2 2.0.1) libraries
for graphics rendering and handling events and window operation.

About Gameplay:
FastFarm is a puzzle game based on the zero player game of Conway's game of life. It is very brainstorming game. The enthusiasts who like cracking puzzle and can solve puzzles quiclkly would love this game.
The game consists of 2 modes:

1. puzzle challenge
2. 2-player

Puzzle challenge
----------------
It is the single player game mode which is turn based. The instructions to play are as follows: 
Dr. Plankenstein has a farm which is attacked by weeds. But he is a crazy scientist. 
Instead of cutting the weeds he uses his own theory of life according to which a weed 
has following life cycle:

1. If a weed has less than 2 weeds in its neighbourhood, it dies due to under-population. 
2. If a weed has 2,3 or 4 weeds in its neighbourhood, it lives. 
3. If a weed hs exactly 4 weeds in its neighbourhood, it reproduces, but only once in its lifetime. 
4. If a weed has more than 4 weeds in its neighbourhood, it dies due to over-population.

	
	
You have to help the doctor to clear the farm from weeds by planting more weeds strategically to kill them. As the weeds die successively a chain of dying weeds may get triggered which can be used for advantage. You only have limited stock of weeds to plant. To increase the stock you can either purchase a weed seedling or plant weeds such that they reproduce. Each weed reproducing once add one weed seedling to the stock. After clearing all weeds your score is high if moves taken(plantations) and purchases are less and stock retained is more. Moves and purchases affect the score more significantly. Play along with your friends and compare your score!!! :D

Neighbourhood : 8 surrounding squares of any given square.

See instructions in game for controls and detailed instructions...

2-player
---------
It is two-player realtime game mode in which two players compete against each other to plant more plants. The one who plants more plants by the end of game when time period gets over is the winner. Instructions are as follows:

From "Puzle challenge" Dr. Plankenstein has cleared all the weeds. Now he is going to plant his crops. But Dr. Herbestien has become his rival. 

2 Players play as Dr. Plankenstein (Player 1) and Dr. Herbestein (Player 2) and compete to capture the farm by planting more crops than opponent in given period of time. 
Life cylce of crop : 	It is same as that of weeds in "Puzzle challenge" except that crops due to genetic engineering by doctors has got special attributes like ---
1. Every crop irrespective of neighbourhood reproduces indefinately in its lifetime. So the stock is never empty. Infinite crops can be planted.
2. A crop may also die if its neighbourhood has more crops of opponent than itself.
3. Some crop plants have abilty to survive in either under or over population.

The game runs normally for initial 30 sec, but after that speciality of crops start to vary depending upon relative population. For first 30 seconds each player get 10 plants that can survive alpne and rest infinte are normal. After 30 seconds the player with less plants gets the special attribute of crops surviving under population and the other player gets the special attribute of crops surviving over population. If both have eqaul number of plants then crops/plants are normal. The game really becomes very intersting and mind-teasing when the players have to think to plant crops with dynamically changing attributes.

Due to multi-touch input in AIO it is possible to track the fingers of two players which make the controls very smooth. Other than touch there is keyboard controls as well. 

See instructions in game for controls and detailed instructions...

