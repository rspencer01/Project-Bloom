#include <flower.h>
#include <log.h>
#include <gtx/random.hpp>
#include <game.h>
#include <images.h>
int sz=800;

Flower::Flower(Game* parent,glm::vec3 pos) : Object(pos,parent)
{
  LODshaderID = game->shaderManager->newShader("flowerLOD",VERTEX_SHADER|GEOMETRY_SHADER|FRAGMENT_SHADER);
  shaderID = game->shaderManager->newShader("flower",VERTEX_SHADER|GEOMETRY_SHADER|FRAGMENT_SHADER);
  clearTriangleData(200+10*30,20*(20+30-1));
  glm::vec3 p = glm::vec3(0);
  glm::vec3 l = glm::vec3(1,0,0);
  glm::vec3 f = glm::vec3(0,0,1);
  glm::vec3 u = glm::vec3(0,1,0);
  // The stalk
  float vspace = 0.4;
  for (int i = 0;i<30;i++)
  {
    for (int j = 0;j<10;j++)
    {
      glm::vec3 pos = p+
                      l*float(0.1*cos(j/10.0*2*3.1415))+
                      f*float(0.1*sin(j/10.0*2*3.1415));

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
      addPoint(i*10+j+300,pos,
        glm::normalize(pos-p),glm::vec4(0,0,1,1));
    }

  for (int j = 0;j<30+20-1;j++)
  {
    for (int i = 0;i<10;i++)
    {
      addTriangle(20*j + i,
                  10*j+i,10*j+(i+1)%10,10*j+i+10);
      addTriangle(20*j + i+10,
                  10*j+(i+1)%10,10*j+i+10,10*j+(i+1)%10+10);
    }
  }

  pushTriangleData();
  RenderLOD();
}
