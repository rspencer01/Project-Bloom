#include <flower.h>
#include <log.h>
#include <gtx/random.hpp>
#include <game.h>
#include <images.h>
int sz=800;
Flower::Flower(Game* parent) : Object(glm::vec3(0),parent)
{
  shaderID = game->shaderManager->newShader("flower",VERTEX_SHADER|GEOMETRY_SHADER|FRAGMENT_SHADER);
  clearTriangleData(200+10*30,0);
  glm::vec3 p = glm::vec3(0);
  glm::vec3 l = glm::vec3(1,0,0);
  glm::vec3 f = glm::vec3(0,0,1);
  glm::vec3 u = glm::vec3(0,1,0);
  // The stalk
  glm::vec3 bboxmin = glm::vec3(1000);
  glm::vec3 bboxmax = glm::vec3(-1000);
  float vspace = 0.4;
  for (int i = 0;i<30;i++)
  {
    for (int j = 0;j<10;j++)
    {
      glm::vec3 pos = p+
                      l*float(0.1*cos(j/10.0*2*3.1415))+
                      f*float(0.1*sin(j/10.0*2*3.1415));
      bboxmin.x = glm::min(bboxmin.x,pos.x);bboxmin.y = glm::min(bboxmin.y,pos.y);bboxmin.z = glm::min(bboxmin.z,pos.z);
      bboxmax.x = glm::max(bboxmax.x,pos.x);bboxmax.y = glm::max(bboxmax.y,pos.y);bboxmax.z = glm::max(bboxmax.z,pos.z);

      addPoint(i*10+j,pos,
        glm::normalize(pos-p),glm::vec4(0,0.5,0,1));
    }
    u += l*0.05f * float(pow(i/15.0,2));
    u = glm::normalize(u);
    l = glm::normalize(glm::cross(u,f));
    p += vspace*u;

  }

  // The bell
  for (int i = 0;i<20;i++)
    for (int j = 0;j<10;j++)
    {
      float x = (i+1)/8.0;
      float y = 0.6*pow(pow(x -1.29349,3) - 0.9*(x -1.29349) + 1,0.5);
      glm::vec3 pos = p+
                      l*float(y*cos(j/10.0*2*3.1415))+
                      u*float(i/10.0)+
                      f*float(y*sin(j/10.0*2*3.1415));
      bboxmin.x = glm::min(bboxmin.x,pos.x);bboxmin.y = glm::min(bboxmin.y,pos.y);bboxmin.z = glm::min(bboxmin.z,pos.z);
      bboxmax.x = glm::max(bboxmax.x,pos.x);bboxmax.y = glm::max(bboxmax.y,pos.y);bboxmax.z = glm::max(bboxmax.z,pos.z);
      addPoint(i*10+j+300,pos,
        glm::normalize(pos-p),glm::vec4(0,0,1,1));
    }
  logi.log("Bounding box <%f %f %f> to <%f %f %f>",bboxmin.x,bboxmin.y,bboxmin.z,bboxmax.x,bboxmax.y,bboxmax.z);
  pushTriangleData();
}

void Flower::Render()
{
  game->shaderManager->loadShader(shaderID);
  // Load our transformation matrix etcqq
  //game->shaderManager->getCurrentShader()->setObjectData(objectBO);
  glBindVertexArray(VAO);

  glDrawArrays(GL_POINTS,0,200+10*30);
}