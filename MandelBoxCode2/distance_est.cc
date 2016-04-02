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

extern MandelBoxParams mandelBox_params;
extern float MandelBoxDE(const vec3 &pos, const MandelBoxParams &mPar, float c1, float c2);

//Distance Estimator Field Selector
float DE(const vec3 &p)
{
  
  float c1 = fabsf(mandelBox_params.scale - 1.0);
  float c2 = powf( fabsf(mandelBox_params.scale), 1 - mandelBox_params.num_iter);
  float d = MandelBoxDE(p, mandelBox_params, c1, c2);
  return d;
}
