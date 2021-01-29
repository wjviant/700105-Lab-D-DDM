# Games Architecture - Lab C - DDM

The idea for the next several weeks is to give you practical experience of building a game engine architecture.

The code here is in bad shape.  Today you will look at this code and you will make a number of improvements and/or add features.

---

## Task 1
1. Create the new `GameObject` hierarchy
    - You probably want classes for `Ship`, `Asteroid`, `Bullet`, and `Explosion`.
    - All should inherit from `GameObject`
    - Move the appropriate data/functions to the new classes
    - The score numbers can be ignored for the time being
2. Modify `Window` to create, update, and draw the correct classes

#### [LAB BOOK - Describe what you changed in the code and reflect on the advantage of using a hierarchy of game objects]

---

## Task 2
1. Put all game objects into a single `std::vector<GameObject*>` that will be used for rendering
    - Ignore the score numbers for now
    - Keep the original lists of asteroids and bullets too as you will still need them for collision detection
2. Update the rendering code to just go through and draw all objects from the single rendering list
3. Pay attention to what happens when things die!

#### [LAB BOOK - Describe what you changed in the code and reflect on the advantage of using a render list of objects]

---


