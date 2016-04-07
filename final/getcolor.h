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
#include "color.h"
#include "renderer.h"
#include "vector3d.h"
#include <cmath>
#include <algorithm>

using namespace std;

#ifndef GETCOL
#define GETCOL

#define MAXB(a, b)	(a<b)?b:a

static inline void foo(){
	//printf("HI\n");
}
//static
inline void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV)
{
     vec3 CamLight = {1.0,1.0,1.0};
   float CamLightW = 1.8;// 1.27536;
   float CamLightMin = 0.3;// 0.48193;


  vec3 nn = SUBK(n, 1);
  float ambient = MAXB(CamLightMin, (DOTRET(nn, direction)))*CamLightW;
  vec3 temp = MUL(CamLight, color);
  outV.x = temp.x; outV.y = temp.y; outV.z = temp.z; //MUL(CamLight, color);
  MULK_SET(outV, ambient); 
}

#pragma acc routine seq
void getColour(const pixelData &pixData, const RenderParams &render_params,
	       const vec3 &from, const vec3  &direction, vec3 *outp)
{
 const vec3 baseColor = {1.0, 1.0, 1.0};
 const vec3 backColor = {0.4, 0.4, 0.4};

  //colouring and lightning
  vec3 hitColor = COPY(baseColor);
  
  if (pixData.escaped == false) 
    {
      //apply lighting
      

      lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);

      

      
      //add normal based colouring
      if(render_params.colourType == 0 || render_params.colourType == 1)
	{
	  vec3 temp = MUL(hitColor, pixData.normal);
	  hitColor.x = temp.x; hitColor.y = temp.y; hitColor.z = temp.z;
	  ADD_SET(hitColor, 1.0);
	  //hitColor = DIVK(hitColor, 2.0);
	  hitColor.x /= 2; hitColor.y /= 2; hitColor.z /= 2;
	  MULK_SET(hitColor, render_params.brightness);
	  	  
	  //gamma correction
	  CLAMPVEC(hitColor, 0.0, 1.0);
	  hitColor.x *= hitColor.x; hitColor.y *= hitColor.y; hitColor.z *= hitColor.z; //= MUL(hitColor, hitColor);
	}
      if(render_params.colourType == 1)
	{
	  //"swap" colors
	  float t = hitColor.x;
	  hitColor.x = hitColor.z;
	  hitColor.z = t;
	}
    }
  else {
    //we have the background colour
    hitColor.x = backColor.x; hitColor.y = backColor.y; hitColor.z = backColor.z;//COPY(backColor);
  }
  outp->x = hitColor.x; outp->y = hitColor.y; outp->z = hitColor.z;
  //return hitColor;
}

#endif