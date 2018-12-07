# Ansel Engine

## Installation
Installing Ansel is super easy, all you have to do is clone the git repository and start programming in the **Game** project. 

Alternatively, you can create your own project with or without Visual Studio. In order to do this you must set `Ansel/src` as an include directory as well as AnselECS/src if you wish to take advantage of the Component System. Both `Ansel.dll` and `AnselECS.dll` must be in your executable directory, and their corresponding `*.lib` files found in `bin/'Your Configuration'/Ansel.lib` and `bin/'Your Configuration'/AnselECS.lib` respectively. Also the include directories which are `Ansel/src` and `AnselECS/src`.

## Utilizing the Component System

The only file you need to include is `Ansel.h` even if you are using the Component System. At the moment, the `AnselECS.dll` and `AnselECS.lib` files are required in order to run Ansel. I'll find a way around this in the future once I understand how I want them to be implemented. **No preprocessor definitions are needed!**

## How to Get Started 

For a detailed description of how to get started, read `Getting Started.pdf` (which is continually getting updated so there may be mistakes).

### Roadmap

---

*(Started on first component system implementation) 12/5/18*

| ~~Completed~~ | In Progress | *Planned* (just italics) | ***Long Term*** |
|-|-|-|-|

+ ~~Basic component system implementation~~
+ ~~Batch rendering~~
+ Font and text rendering
+ *Support for custom shaders with built in functionality*
+ *Built in memory manager*
+ *Threaded processor/manager (to allow client to use threads to run custom own functions)*
+ *Lua implementation*
+ ***Physically Based Rendering***
+ ***Deferred Rendering***
+ ***Sound Engine***

---

# Images

## Instancing

2,500 cubes being rendered with unique rotation, scale, and translation at 60 FPS. This image was taken with 3D was first implemented. 

[//]: # (![alt text](https://github.com/maxortner01/ansel/blob/master/Images/example1.png)

![alt text](Images/example1.png)

## Materials

This image was taken when UI elements and Materials were implemented.

![alt text](Images/example3.png)

