/**
 * @file game.h
 * @author Robert Spencer <robert.spencer94@gmail.com>
 * @section DESCRIPTION
 *
 * This file contains the game class, that handles keypresses and rendering.
 */

#ifndef GAMEOPS_H
#define GAMEOPS_H

#include <window.h>
#include <camera.h>
#include <flower.h>
#include <shaders.h>

/// The `Game` class contains all the methods to do with gameplay.
///
/// This class interacts with glut graphics and keyboard handlers, and contains all the objects assiciated with a game instance.
/// It handles the render sequence (shadows) and all interactions with the user go through here.
class Game
{
  private:
    /// Initialises OpenGL and the window
    void initGraphics();
    /// Renders the main window
    void renderMainWindow(float ms);
    /// The main camera
    Camera* camera;
    /// Does the mouse control the camera?
    bool mouseCameraControl;
    /// Which keys are down
    bool keys[256];
    /// A sample flower
    Flower* flowers[100];
  public:
    /// Creates a game instance and initialises all variables.
    Game();
    /// Runs the game
    void run();
    /// The shader manager
    ShaderManager* shaderManager;
    /// A key event log
    void key(int,int,int,int);
    /// The event for the mouse scroll
    void scroll(double,double);
    /// The main window;
    Window* mainWindow;
};

#endif
