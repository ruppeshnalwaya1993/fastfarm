# Code Write-Up

## Overview of Code

The code of this game was written entirely in C++ and uses SDL library for handling graphics, events and windows operations. I learnt how to use SDL library for Game development from the tutorials at http://www.sdltutorials.com . The basic design is as follows. The fundamental flow of the program is that of a basic game loop:
```C++
// Basic Game Loop below
Initialise();

while(Running) {
    Events();
    Loop(&Running);
    Render();
}

Cleanup();
```
Here, Initialise() refers to all the steps like setting up the window, the game/graphics engine(if any), loading files like text, graphics, audio, and all other necessary operations that are required to be done just once at the beginning of the game. Event() handles listening to the input from user/player (e.g. mouse click) and taking actions depending upon the user input. Loop() handles all the state transitions in the game depending upon the current state of the game. Render() handles all the graphics rendering operations. At the end when game terminates, Cleanup() handles operations like freeing up the memory and exiting gracefully. 

The major classes in the code are:
1. CMenu: Handles selection between game modes (like puzzle challenge, 2 player, Puzzle builder, Different Help sections)
1. CApp: Handles the puzzle challenge mode
2. CMulti: Handles the 2-player game mode
4. CHelp: Handles the help section to learn about the game rules/controls
5. CBuilder: Handles the mode in which players can build new maps to play.

All these classes implement a game loop. Only one object for each of these classes is constructed. 

### Inheritance and Polymorphism
All the above classes (CMenu, CApp, CMulti, CHelp, CBuilder) inherit from (abstract)`Class CEvent` which provides interface to handle different kinds of events captured in the game loop. All the functions in class CEvent are virtual. This allows each of the subclasses to override the way different input events are handled but the subclass does not need to change the basic game loop and just call the `OnEvent(&Event)` function once.

Given that the basic game loop and event handling is setup in every game mode by the use of above classes, we need the way to represent entities i.e. objects that interact with each other and the environment (e.g. the player(s), plants, weeds, etc.). These entities are represented by the `Class CEntity`. For every entity, an instance of CEntity is created and put in a static list. CEntity does not implement a game loop but has functions which are called by the the major class (say CApp) in different parts of the game loop. 

**Polymorphism with CEntity:**
Since CEntity contains virtual functions, it is easy to inherit this class to form new different kinds of entities (players, weed, plants, etc.) and implement different functionality in each of them. The pointer of the base CEntity class can hold the objects of these derived class objects. These pointers can then be inserted into the static list of Entities and operated upon together very easily by just running a loop and calling required functions. Depending upon the object type which the pointer holds, the effect/output will be different (This is example of polymorphism).






## Possible Improvements in Code

1. **Reducing Memory Footprint:** The graphics used in the code are SDL_Texture formed by loading PNG images or text. Images can easily use a lot of memory if too many of them (or of very large dimensions) are loaded at once. As one may may notice, class CEntity has a data member `SDL_Texture *tex`. And for every object in the game, a new entity is created which loads its own texture adding to the memory. Now, if multiple entities are essentially using the same kind of texture image, then it would be a waste of memory to keep multiple copies of each image in memory, one associated to each Entity. Instead the image texture should be loaded just once and an index/pointer should be held by the respective object instances. This is already achieved in the code but declaring a new `static SDL_Texture *tex` in the subclasses of CEntity like CPlant. The static SDL_Texture *tex hides the non-static tex and is loaded just once. But a static member may not be the best way to handle this. A better way would be to have separate singleton `Class CTextureManager` that loads all the required textures (may be on demand) and return the (smart)pointer/index of the texture(s) needed by the instances of CEntity (and its subclasses). 

   Besides proper management of textures, vector graphics and computing graphics on the fly using shaders can also be used to reduce the memory footprint. Although this may add to a little more computation time, but one has to judge this tradeoff depending upon the platform and environment in which the game will run.

2. **Reduce redundant computation:** If a pattern made of combination of different textures is rendered multiple times or at multiple places (without any change), then instead of directly painting each texture on the window surface, it makes sense to precompute (pre-render) the pattern once to form a new texture (kept in memory managed by texture manager) and then paint this texture on the window at multiple places/time. This saves time to compute the pattern again n again and increases speed of execution at the cost of a little more memory. A typical case where this can be used is the top bar in the game (which contains the back/exit button in the centre and a bordered strip from extreme left of screen to extreme right). This bar does not change its basic appearance in the entire game and hence can be pre-rendered as a texture and then painted onto window. Once again the memory vs time tradeoff should be analysed and appropriate decision should be taken.

3. **Abstract class for Game Loop:** CMenu, CApp, CHelp, CBuilder, all of these classes implement game loops in their `OnExecute()` function and hence have very similar basic structure. Thus, it would be nicer to have all inherit from a new base `Class CGameLoop` having virtual function to provide appropriate interface and avoid redundant code. 

4. **Using Singleton classes:** There are many classes of which only one instance is created and those should not have more than one instance logically. An appropriate design for such classes would be a singleton class (created by making constructor private and providing a public static function to return the pointer to the static object of that class). CMenu, CApp, CHelp, CBuilder as well as CPlayerM should be made singleton. 

5. Right now, a lot code related for initialising SDL, making the window and renderer as well as the objects of CMenu, CApp, CHelp, CBuilder is written in the main(). This should be moved to a separate Class CGameManager for better modularity and un-clutter the main().

6. Call OnCleanup() inside the destructor of respective classes also, so that no memory leaks even if OnCleanup is not called before the object goes out of scope.

7. Improvements in Class CApp: 
   1. Change the name to CPuzzleChallenge to better represent functionality.
   2. Make the utility functions loadWarehouse(), levelSel(), loadStable() part of the class CApp as private functions. Right now these are global defined in CApp.cpp but meant to be used only by CApp's OnInit().
   3. For selecting the level of the Puzzle Challenge mode, an entire game loop is written separately in the levelSel() function which is being called in OnInit() of CApp. This should have been part of either a class inherited from the CMenu class or the CMenu class should have contained several pages thus forming a multi-page multi-level menu for selecting game configurations.

8. Functionality of Error Checking and logging  as well as testing and profiling should be introduced.

9. A few more cosmetic but important changes would make the code more readable and easily understandable:
   1. Appropriate comments representing the use of classes and the functions (with description of members/arguments).
   2. Appropriate indentations and following proper naming convention for variables and functions as well avoiding excessively short names to prevent obscurity. 
   3. Remove unused variables.
