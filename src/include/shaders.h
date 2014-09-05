/**
 * @file    shaders.h
 * @author  Robert Spencer <robert.spencer94@gmail.com>
 * @section DESCRIPTION
 *
 * This file contains the shader class, which is used to load shaders and set their uniform variables
 */
#ifndef SHADERS_H
#define SHADERS_H

//#include <magrathea.h>

#include <map>
#include <string>

#include <glm.hpp>

#define VERTEX_SHADER 0x1
#define GEOMETRY_SHADER 0x2
#define TESSELATION_SHADER 0x4
#define FRAGMENT_SHADER 0x8

/// One of the structs passed to the shaders.  This struct should hold information that
/// is kept constant over a frame, ie over all objects.  Example things to keep in here
/// are game time and camera position.
typedef struct
{
  /// The translation and rotation to set the screen correct relative to the camera
  glm::mat4 cameraMatrix;
  /// The position of the camera
  glm::vec4 cameraPosition;
  /// The projection matrix to use
  glm::mat4 projectionMatrix;
} FrameData;

static_assert(sizeof(FrameData) == 36*sizeof(GLfloat),"FrameData wrong size");

class ShaderProgram;

/// A super class that contains a number of ShaderProgram classes that it can swap between
/// for rendering different things.
class ShaderManager
{
  private:
    /// The id of the currently loaded shader
    int currentShader;
    /// Total number of shaders held
    int numShaders;
    /// All the shaders
    ShaderProgram* shaders[100];
    /// The types of each shader program
    int stypes[100];
    /// A map for looking up positions of shaders based on their name
    std::map<std::string,int> shaderNames;
    /// The position of the frame information
    GLuint frameDataPosition;
    /// A gl buffer to hold the frameDataPosition
    GLuint frameDataBO;
  public:
    /// Constructs the manager
    ShaderManager();
    /// Constructs a new shader program
    int newShader(const char* name,int);
    /// Loads a shader by its id
    void loadShader(int);
    /// Returns a pointer to the currently loaded shader program
    ShaderProgram* getCurrentShader();
    /// Gets a shader by its id
    ShaderProgram* getShaderById(int);
    /// The frame data for this frame
    FrameData frameData;
    /// Pushes the frame data to the graphics card
    void setFrameData();
    /// Reloads all shaders.
    void reloadAll();
};

/// A shader program object handles the loading, compiling and executing of shaders (both vertex and fragment)
/// on the GPU.
class ShaderProgram
{
  private:
    /// The number OpenGL assigned to this program
    GLuint ShaderProgramID;
    /// Finds the location of a variable by name
    GLuint getVariablePosition(const char* name);
    /// A map to hold the positions in GPU memory of variables (so we don't look them up each time)
    std::map<std::string,GLuint> variableLocations;
    /// The position of the object transform matrix (changed often)
    GLuint objectDataPosition;
    /// The position of the frame information
    GLuint frameDataPosition;
  public:
    /// Construct the program (empty and unloaded)
    ShaderProgram(const char* nm);
    /// Add an object
    void LoadShader(const char*,GLenum);
    /// Link all the pieces together
    void CompileAll();
    /// Actually use this shader
    void Load();
    /// Set some uniform matrix variable
    void setInt(const char* varName, unsigned int value);
    /// Function to set the object transformation matrix etc.
    void setObjectData(GLuint);
    /// Function to set the camera transformation matrix etc.
    void setFrameData(int);
    /// An accessor to get the buffer index
    GLuint getFrameDataBufferNumber();
    /// The name of the shader.  Used for reloading purposes
    const char* name;
};

#endif 