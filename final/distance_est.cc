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
#include "vector3d.h"
#include "mandelbox.h"
#include "mandelboxde.h"

#include "color.h"
//#include "mandelbox.h" //already included
#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif

//using namespace std;
//extern MandelBoxParams mandelBox_params; // KA no global variables!
extern float MandelBoxDE(const vec3 &pos, const MandelBoxParams &mPar, float c1, float c2);
float MandelBulbDistanceEstimator(const vec3 &p0, const MandelBoxParams &params);

//Distance Estimator Field Selector
float DE(const vec3 &p, MandelBoxParams &mandelBox_params)
{	
#ifdef BULB
  float d = MandelBulbDistanceEstimator(p, mandelBox_params);
#else
  float c1 = fabsf(mandelBox_params.scale - 1.0);
  float c2 = powf( fabsf(mandelBox_params.scale), 1 - mandelBox_params.num_iter);
  float d = MandelBoxDE(p, mandelBox_params, c1, c2);
#endif
  return d;
}

float MandelBulbDistanceEstimator(const vec3 &p0, const MandelBoxParams &params)
{
  vec3 z = COPY(p0);
  
  float dr = 1.0;
  float r = 0.0;

  float Bailout = params.rMin;
  float Power = params.rFixed;

  for (int i=0; i < params.num_iter; i++) 
    {
      r = MAGNITUDE_RET(z);
	  
      if(r > Bailout){ break; }

      float theta = acos(z.z/r);
      float phi   = atan2(z.y, z.x);
      dr = pow(r, Power - 1.0) * Power * dr + 1.0;

      float zr = pow(r, Power);
      theta     = theta * Power;
      phi       = phi * Power;

      z.x = zr*sin(theta)*cos(phi);
      z.y = zr*sin(phi)*sin(theta);
      z.z = zr*cos(theta);

      z.x = z.x + p0.x;
      z.y = z.y + p0.y;
      z.z = z.z + p0.z;
    }

  return 0.5*log(r)*r/dr;
}





