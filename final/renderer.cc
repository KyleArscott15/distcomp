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
#include <stdio.h>
#include <stdlib.h>

#include "omp.h"

#ifdef _OPENACC
#include <openacc.h>
#endif

//#include <iostream>

#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"
#include "getcolor.h"

#pragma acc routine seq  // XXX KA 
extern int UnProject(float winX, float winY, const CameraParams &camP, float *obj);

extern float getTime();
extern void   printProgress( float perc, float time );

#pragma acc routine seq
extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, float eps, pixelData &pix_data, MandelBoxParams &mandelBox_params);

void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image, MandelBoxParams mandelBox_params)
{
  const float eps = pow((float)10.0, renderer_params.detail); 
  vec3 from;
  
  SET_POINT(from,camera_params.camPos)
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  const int area   = width * height;

  float *farPoint = (float*) malloc(sizeof(float)*3*area);
  vec3 *to = (vec3*) malloc(sizeof(vec3) * area);
  pixelData *pix = (pixelData*) malloc(sizeof(pixelData) * area);
  vec3* col = (vec3*) malloc(sizeof(vec3) * area);

//#pragma omp parallel for
  /*
  for (int i = 1; i<area; i++){
    UnProject(i%width, i/width, camera_params, &farPoint[i*3]);
  }
  */
  
#pragma acc data copyout(image[0:3*area]) create(to[0:area],pix[0:area],col[0:area]) pcopyin(eps,from,renderer_params,mandelBox_params,camera_params)
{
#pragma acc parallel loop //#pragma acc kernels
{
#pragma acc loop independent //#pragma acc for independent
  for(int i = 0; i < area; i++){
    UnProject(i%width, i/width, camera_params, &farPoint[i*3]); // XXX KA 
    
	  VECESUBDUBDUB(to[i],farPoint,camera_params.camPos);
	  NORMALIZE(to[i]);
	  rayMarch(renderer_params, from, to[i], eps, pix[i], mandelBox_params);
	  getColour(pix[i], renderer_params, from, to[i], &col[i]);

	  //save colour into texture
	  //int k = (j * width + i)*3;
	  image[i*3+2] = (unsigned char)(col[i].x * 255);
	  image[i*3+1] = (unsigned char)(col[i].y * 255);
	  image[i*3]   = (unsigned char)(col[i].z * 255);
	} // inner for
      //printProgress((j+1)/(float)height,getTime()-time);
    //}//end of outer for
}
}
  printf("\n rendering done:\n");
}
