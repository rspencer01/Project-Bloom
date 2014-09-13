#include <GL/glew.h>
#include <GL/glut.h>

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <log.h>


#include <stdio.h>
#include <object.h>
#include <game.h>


/// @param pos The position of this object in gamespace
/// @param g   The game to which this object belongs
Object::Object(glm::vec3 pos,Game* g)
{
  position = pos;
  game = g;
  buffersInitialised = false;
  vertexData = NULL;
  triDat = NULL;
  billboard = false;
  forward = glm::vec3(1,0,0);
  up = glm::vec3(0,1,0);
  right = glm::vec3(0,0,1);
  xySlew = 0;
  glGenBuffers(1, &objectBO);
  VAO = (GLuint)-1;
  updateMatrix();
  objectData.LODcols = 15;
  objectData.LODrows = 15;
  bboxMin = glm::vec3(100000.f);
  bboxMax = glm::vec3(-100000.f);
  // Construct the LOD Texture
  LODTexture = new Texture(LOD_TEXTURE,1024,1024);

  glGenFramebuffers(1, &LODFrameBuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, LODFrameBuffer);

  glGenRenderbuffers(1, &LODDepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, LODDepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, LODDepthBuffer);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, LODTexture->getTextureNumber(), 0);
 
  // Set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/// Frees the data used by this object (esp the buffers in the GPU)
///
/// More specifically, destroys the `vertexData`, `triangleData`, `vertexVBO`,
/// `indexVBO`, `objectBO` and `VAO`.
Object::~Object()
{
  if (vertexData!=NULL)
    delete vertexData;
  if (triDat!=NULL)
    delete triDat;
  if (buffersInitialised)
  {
  	glDeleteBuffersARB(1,&vertexVBO);
  	glDeleteBuffersARB(1,&indexVBO);
    glDeleteBuffersARB(1,&objectBO);
    glDeleteVertexArrays(1,&VAO);
  }
}

/// Changes the position of the object.
///
/// Updates the object matrix as well.
void Object::setPosition(glm::vec3 pos)
{
  position = pos;
  updateMatrix();
}

/// Renders this object to the screen, using the VBOs that were 
/// initialised using the addPoint, addTriangle and pushTriangleData
/// functions
/// @param refreshTime Number of milliseconds since the last rendering
/// @param cameraPos   Position of the camera in 3-space
void Object::Render(glm::vec3 cameraPos)
{
  // Rotate the object if it is a billboard
  if (billboard)
  {
  }
  // Only do something if we have data
	if (buffersInitialised)
	{
    if (glm::length(cameraPos - position)>40)
    {
      game->shaderManager->loadShader(shaderID);
    }
    else
    {
      game->shaderManager->loadShader(LODshaderID);
      game->shaderManager->getCurrentShader()->setInt("row",-1);
      game->shaderManager->getCurrentShader()->setInt("column",-1);
    }
    // Load our transformation matrix etc
    game->shaderManager->getCurrentShader()->setObjectData(objectBO);
    // Select this object's texture
    //glBindTexture(GL_TEXTURE_2D,textureNumber);
    // Use our data
    glBindVertexArray(VAO);
    LODTexture->load();
    if (glm::length(cameraPos - position)>40)
    {
      glDrawArrays(GL_POINTS,0,1);
    }
    else
    {
      glDrawArrays(GL_POINTS,0,numberOfPoints);
    }

    //glDrawElements(GL_TRIANGLES,numberOfTriangles*3,GL_UNSIGNED_INT,0);
  }
}

/// Reset the data.  Clear the internal arrays, and the GPU arrays.
/// Reserve space for p point and t triangles
/// @param p Number of points
/// @param t Number of triangles
void Object::clearTriangleData(int p, int t)
{
  // Clear all the data and the buffers (if required)
  if (triDat!=NULL)
  { 
    delete triDat;
    triDat = NULL;
  }
  if (vertexData!=NULL)
  {
    delete vertexData;
    vertexData = NULL;
  }
  triDat = new int[t*3];
  vertexData = new VertexDatum[p];
  if (buffersInitialised)
  {
  	glDeleteBuffersARB(1,&vertexVBO);
	  glDeleteBuffersARB(1,&indexVBO);
  }
  numberOfTriangles = t;
  numberOfPoints = p;
  // Now make buffers
	glGenBuffersARB(1,&indexVBO);
  glGenBuffersARB(1,&vertexVBO);
}
/// Add a new point to the object.  All points are taken relative to the origin of the object
/// @param i The index of the vertex to change
/// @param point The position of this point, relative to the object origin
/// @param r The red component of the colour
/// @param b The blue component of the colour
/// @param g The green component of the colour
void Object::addPoint(int i,glm::vec3 point,glm::vec3 normal, glm::vec4 col)
{
  // Add it to the internal array
	vertexData[i].position = point;
	vertexData[i].normal = normal;
	vertexData[i].colour = col;
  vertexData[i].texMix = glm::vec4(-1);

  bboxMin.x = glm::min(bboxMin.x,point.x);
  bboxMin.y = glm::min(bboxMin.y,point.y);
  bboxMin.z = glm::min(bboxMin.z,point.z);
  bboxMax.x = glm::max(bboxMax.x,point.x);
  bboxMax.y = glm::max(bboxMax.y,point.y);
  bboxMax.z = glm::max(bboxMax.z,point.z);

}


void Object::setTextureMix(int point,float a, float b, float c, float d)
{
  vertexData[point].texMix = glm::vec4(a,b,c,d);
}

/// Adds a new triangle to the object.  Indexes are the same as the order the points were added
/// @param i The index of the triangle to edit
/// @param a The index of the first point
/// @param b The index of the second point
/// @param c The index of the third point
void Object::addTriangle(int i, int a,int b, int c)
{
  // Add it to the internal array
	triDat[i*3] = a;
	triDat[i*3+1] = b;
	triDat[i*3+2] = c;
}

/// Constructs new VBOs and pushes all the data to the GPU
void Object::pushTriangleData()
{
  // Make our VAO.  Not sure why leaving this out stops the first region from rendering... does the vao need to know about glBufferData?
  if (VAO==(GLuint)-1)
    glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);
	// set it as the current one,
	glBindBufferARB(GL_ARRAY_BUFFER, vertexVBO);
	// ... and blit the data in.
	glBufferDataARB(GL_ARRAY_BUFFER, numberOfPoints*sizeof(VertexDatum),vertexData,GL_STATIC_DRAW);

	// set it as the current one,
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	// ... and blit the data in.
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, numberOfTriangles*3*sizeof(int),triDat,GL_STATIC_DRAW);
  updateVAO();
	// Set the variables that need setting
	buffersInitialised = true;
  objectData.boundingSphere = glm::vec4((bboxMin+bboxMax)/2.f,glm::length(bboxMin-bboxMax)/2);
  updateObjectBO();
}

void Object::updateVAO()
{
  if (VAO==(GLuint)-1)
    glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);
  glBindBufferARB(GL_ARRAY_BUFFER,vertexVBO);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointerARB(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexDatum),0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointerARB(1,4,GL_FLOAT,GL_FALSE,sizeof(VertexDatum),(void*)(3*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointerARB(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexDatum),(void*)(10*sizeof(float)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointerARB(3,3,GL_FLOAT,GL_FALSE,sizeof(VertexDatum),(void*)(7*sizeof(float)));
  glEnableVertexAttribArray(4);
  glVertexAttribPointerARB(4,4,GL_FLOAT,GL_FALSE,sizeof(VertexDatum),(void*)(12*sizeof(float)));
  glBindVertexArray(0);
}

void Object::updateTriangleData()
{

  glDeleteBuffersARB(1,&vertexVBO);
  glGenBuffersARB(1,&vertexVBO);
  glBindBufferARB(GL_ARRAY_BUFFER,vertexVBO);
  glBufferDataARB(GL_ARRAY_BUFFER, numberOfPoints*sizeof(VertexDatum),vertexData,GL_STATIC_DRAW);

  updateVAO();
}

void Object::editTextureCoord(int i, float u, float v)
{
	vertexData[i].texture = glm::vec2(u,v);
}

/// Rotates the object to match the given new axis
/// @param basisX The new X axis
/// @param basisY The new Y axis
void Object::rotate(glm::vec3 basisX,glm::vec3 basisY)
{
  forward = glm::normalize(basisX);
  up = glm::normalize(basisY);
  right = glm::normalize(glm::cross(up,forward));
  updateMatrix();
}

/// Updates the object translation and rotation matrix
void Object::updateMatrix()
{
  // This works.  You can check it yourself.
  /*objectData.transformMatrix = glm::mat4(glm::vec4(forward,0),
                                         glm::vec4(up,0) + glm::vec4(xySlew,0,0,0),
                                         glm::vec4(right,0),
                                         glm::vec4(position,1));*/
  updateObjectBO();
}

/// WARNING: Only call this function if you are sure that you will never 
/// edit the vertices again.  That will cause a seg fault
/// Only to be called after the triangle data has been set of an object
/// that is either massive, or very common and that will never animate
/// (eg trees)
void Object::freeze()
{
  if (triDat!=NULL)
  {
    delete[] triDat;
    triDat = NULL;
  }
  if (vertexData!=NULL)
  {
    delete[] vertexData;
    vertexData = NULL;
  }
}

void Object::updateObjectBO()
{
  glBindBuffer(GL_UNIFORM_BUFFER, objectBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(objectData), &objectData, GL_DYNAMIC_DRAW);
}

/// The accessor for the object position.
glm::vec3 Object::getPosition()
{
  return position;
}

/// Renders the object to the offscreen LOD texture.
void Object::RenderLOD()
{
  
  // Load our transformation matrix etcqq
  
  glBindVertexArray(VAO);
  game->shaderManager->loadShader(LODshaderID);
  game->shaderManager->getCurrentShader()->setObjectData(objectBO);
  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER,LODFrameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,1024,1024);
  for (int i = 0;i<=objectData.LODrows;i++)
  {
    for (int j = 0;j<=objectData.LODcols;j++)
    {
      game->shaderManager->getCurrentShader()->setInt("row",i);
      game->shaderManager->getCurrentShader()->setInt("column",j);
      glDrawArrays(GL_POINTS,0,numberOfPoints);
    }
  }

  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER,0);
  game->mainWindow->setContext();
}