/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015-2016 at McMaster University.

    Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <strings.h>//bzero
#include <math.h>
#include "vector3d.h"
    #include "color.h"

//ifdef _OPENACC
#include <openacc.h>
//#endif

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params, MandelBoxParams *mandelBox_params);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams camera_params, const RenderParams &renderer_params, 
unsigned char* image, const MandelBoxParams &mandelBox_params, float *farPoint, vec3 *to, pixelData *pix, vec3* col, 
const int width, const int height, const int area, const float eps);//, const float &eps);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);

int main(int argc, char** argv)
{
  MandelBoxParams mandelBox_params;
  CameraParams    camera_params;
  RenderParams    renderer_params;
  
  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

  int image_size = renderer_params.width * renderer_params.height;
  int image_memory = 3*image_size*sizeof(unsigned char);
  unsigned char *image = (unsigned char*)malloc(image_memory);

  const float eps = pow((float)10.0, renderer_params.detail); 
  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  const int area   = width * height;

  float *farPoint = (float*) malloc(sizeof(float)*3*area);
  vec3 *to = (vec3*) malloc(sizeof(vec3) * area);
  pixelData *pix = (pixelData*) malloc(sizeof(pixelData) * area);
  vec3* col = (vec3*) malloc(sizeof(vec3) * area);

  char name[64];
  bzero(name,64);

  int x=0;

  #pragma acc data copyin(renderer_params, mandelBox_params)
  {
    for(;x<2;x++){
      init3D(&camera_params, &renderer_params);
      renderFractal(camera_params, renderer_params, image, mandelBox_params, farPoint, to, pix, col, width, height, area, eps);
      sprintf(name, "image-%05d.bmp", x);
      saveBMP(name, image, renderer_params.width, renderer_params.height); //renderer_params.file_name
      camera_params.camPos[0] = -7; // DONT DO +=
      camera_params.camPos[1] = 0; // DONT DO +=
      camera_params.camPos[2] = 0; // DONT DO +=
      //bzero(image, image_memory); // doesn't make next image work
    }
  }
  free(image);

  return 0;
}
