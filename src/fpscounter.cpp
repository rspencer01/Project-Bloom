#include <cstdio>
#include <log.h>
#include <glfw3.h>
#include <math.h>

float fps = 0.0;
int currentTime;
int previousTime;
int frameCount;
int previousFrame;

/// Registers the passing of a frame into the frame rate counter
void logFrame()
{
  // Note that a frame took place
  frameCount++;
  // Get the time since last frame
  currentTime = int(glfwGetTime()*1000);
  int timeInterval = currentTime - previousTime;
  // Update the frame rate every tenth of a second
  if(timeInterval > 100)
  {
    // Calculate the fps
    fps = frameCount / (timeInterval / 1000.0f);
    // Reset the counters
    previousTime = currentTime;
    frameCount = 0;
  }
  previousFrame = currentTime;
}

/// Returns the absolute time (in milliseconds) since last frame
int getFrameTime()
{
  return glfwGetTime()-previousFrame;
}

/// Gets the time (in seconds) since the last frame, averaged over a number of frames
float getFrameDiff()
{
  if (fps==0.f)
    return 1;
  return 1.f/fps;
}

/// Gets the number of frames per second, on average
float getFrameRate()
{
  return fps;
}
