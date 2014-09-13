#include <graphics.h>
#include <game.h>
#include <log.h>
#include <window.h>

Window::Window(Game* parent)
{
  width = height = 600;
  game = parent;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  windowHandle = glfwCreateWindow(width, height, "Project Magrathea", NULL, NULL);
  glfwSetWindowPos(windowHandle,800,30);
  if (!windowHandle)
    loge.log("Failed to create window");
  glfwMakeContextCurrent(windowHandle);
}

/// Asks glfw to destroy the window
Window::~Window()
{
  glfwDestroyWindow(windowHandle);
}

/// Sets the openGL context to this window.  Basically sets up the projection
/// matrix (should this be here?) and tells glfw to look at this window.
///
/// This should be called before starting rendering to the window.  As a result,
/// we do not need a resize handler anywhere else, and can update the window size
/// in this function.
void Window::setContext()
{
  glfwGetWindowSize(windowHandle,&width,&height);
  makePerspectiveMatrix(&game->shaderManager->frameData.projectionMatrix,float(width)/height);
  glfwMakeContextCurrent(windowHandle);
  setViewport();
}

/// Tells OpenGL to render to the whole of the screen
void Window::setViewport()
{
  glViewport(0,0,width,height);
}

/// Returns if the window has been requested to close (via the
/// little x or something else).
int Window::shouldClose()
{
  return glfwWindowShouldClose(windowHandle);
}

/// Moves the data from the background buffer onto the screen.
void Window::swapBuffers()
{
  glfwSwapBuffers(windowHandle);
}

void Window::setKeyCallBack(GLFWkeyfun func)
{
  glfwSetKeyCallback(windowHandle,func);
}

void Window::setScrollCallBack(GLFWscrollfun func)
{
  glfwSetScrollCallback(windowHandle,func);
}

void Window::setTitle(const char* s)
{
  glfwSetWindowTitle(windowHandle,s);
}

/// Gets the size of the window
glm::vec2 Window::getSize()
{
  return glm::vec2(width,height);
}

glm::vec2 Window::getMousePos()
{
  double x,y;
  glfwGetCursorPos(windowHandle,&x,&y);
  return glm::vec2((float)x,(float)y);
}

glm::vec2 Window::getMouseCentreOffset()
{
  return getMousePos() - glm::vec2(width/2.f,height/2.f);
}

void Window::setMousePos(glm::vec2 p)
{
  glfwSetCursorPos(windowHandle,p.x,p.y);
}

/// Sets the mouse to the center of the screen.
void Window::setMouseCentre()
{
  setMousePos(glm::vec2(width/2,height/2));
}
