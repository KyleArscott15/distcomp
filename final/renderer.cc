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

#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"
#include "getcolor.h"
#include "math.h"

extern float getTime();
extern void   printProgress( float perc, float time );

extern float getTime();
extern void   printProgress( float perc, float time );

extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, float eps, pixelData &pix_data, MandelBoxParams &mandelBox_params);
extern vec3 getColour(const pixelData &pixData, const RenderParams &render_params,
		      const vec3 &from, const vec3  &direction);

extern void foo();

void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, MandelBoxParams &mandelBox_params)
{
  const float eps = pow(10.0, renderer_params.detail); 
  vec3 from;
  
  SET_POINT(from,camera_params.camPos)
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;
  int j;
  //float time = getTime();
  
#pragma omp parallel for default(shared) schedule(dynamic) num_threads(4)
  for(j = 0; j < height; j++){
      int i=0;  
      for(i = 0; i <width; i++){
	  pixelData pix_data;
  	  vec3 color;
	  float farPoint[3];
	  vec3 to;

	  foo();

	  // get point on the 'far' plane
	  // since we render one frame only, 
	  // we can use the more specialized method
	  UnProject(i, j, camera_params, farPoint);
	  
	  // to = farPoint - camera_params.camPos
          to = SUBDUBDUB(farPoint, camera_params.camPos);
	  NORMALIZE(to);
	  
	  //render the pixel
	  rayMarch(renderer_params, from, to, eps, pix_data, mandelBox_params);

	  //get the colour at this pixel
	  color = getColour(pix_data, renderer_params, from, to);

	  //save colour into texture
	  int k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color.x * 255);
	  image[k+1] = (unsigned char)(color.y * 255);
	  image[k]   = (unsigned char)(color.z * 255);
	} // inner for
      //printProgress((j+1)/(float)height,getTime()-time);
    }//end of outer for

  printf("\n rendering done:\n");
}
