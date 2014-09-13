#ifndef WINDOW_H
#define WINDOW_H

#include <glm.hpp>
#include <GL/glew.h>
#include <glfw3.h>

class Game;

class Window
{
  private:
    /// The handle for GLFW
    GLFWwindow* windowHandle;
    /// The game to which this belongs
    Game* game;
    /// The width of the window
    int width;
    /// The height of the window
    int height;
  public:
    /// Constructs a new window
    Window(Game* game);
    /// Destructor.
    ~Window();
    /// Whether or not this window should close
    int shouldClose();
    /// Sets the openGL context of this window
    void setContext();
    /// Blits the image to the screen
    void swapBuffers();
    /// Sets the mouse scroll callback func for this window
    void setScrollCallBack(GLFWscrollfun func);
    /// Sets the key callback func for this window
    void setKeyCallBack(GLFWkeyfun func);
    /// Sets the title of the window
    void setTitle(const char* s);
    /// Accessor for the size of the window
    glm::vec2 getSize();
    /// Accessor for the mouse position in the window
    glm::vec2 getMousePos();
    /// Accessor for the relative mouse position
    glm::vec2 getMouseCentreOffset();
    /// Moves the mouse to the given position
    void setMousePos(glm::vec2);
    /// Moves the mouse to the middle of the window
    void setMouseCentre();
    /// Sets the OpenGL viewport to the size of the window
    void setViewport();
};
#endif