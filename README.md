# Games Architecture - Lab D - DDM

The idea for the next several weeks is to give you practical experience of building a game engine architecture.

This week you will implement a Renderer.

---

## Task 1
1. Open it up and take a look around the code, especially the new `Renderer` class
2. Note that the methods in Renderer.cpp are empty
    - Fill out the methods using the relevant code from the `Window` class
    - Strip out most of the DirectX code from `Window` as you copy it across
    - Change `Window::RenderFrame` to use the new `Renderer` instead of context etc.

#### [LAB BOOK - Describe what you changed in the code]

---

## Task 2
1. Move all rendering code out of `GameObject` and into the `Renderer`
    - Add code to the `Render(GameObject*)` and `Draw()` methods in the `Renderer`
        - Take the render code out of `GameObject`'s `Draw( )` function and put it in these methods instead
    - You will need to set up the modelview matrix with the object's position and then draw the object
    - The `GameObject` shouldn't have any knowledge of the `Renderer`, nor should it need to include `Renderer.h`
2. Change `Window::RenderFrame()` to use the new `Render()` method
    - Remember `ScoreDisplay` and `Mesh`

#### [LAB BOOK - Describe what you changed in the code and reflect on the advantage of using a Renderer]

---


