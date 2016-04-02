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

void getParameters(char *filename, float *camera_position_array, float *camera_position_changes_array, 
  float *camera_angle_array, float *camera_angle_changes_array,
   int frame_no, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
int renderFractal(const CameraParams &camera_params, float *camera_position_array, float *camera_position_changes_array, 
  float *camera_angle_array, float *camera_angle_changes_array, int frame_no,
 const RenderParams &renderer_params, unsigned char* image);
void saveBMP      (const char* filename, int frame_no, const unsigned char* image, int width, int height);

MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{
  
  CameraParams    camera_params;
  RenderParams    renderer_params;
  float camera_position_array[3];
  float camera_position_changes_array[3] = {0 , 0 , 0};
  float camera_angle_array[3];
  float camera_angle_changes_array[3] = {0 , 0 , 0};
  int frame_counter = 0;
  int about_to_crash = 1;
  // int clear_position_changes;
  while (about_to_crash != 0){

  getParameters(argv[1], camera_position_array, camera_position_changes_array, camera_angle_array, 
    camera_angle_changes_array, frame_counter,
   &camera_params, &renderer_params, &mandelBox_params);
  // printf("%f\n", camera_position_array[0]);
  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

  init3D(&camera_params, &renderer_params);

  // for (clear_position_changes = 0; clear_position_changes < 3; clear_position_changes++){
  //   camera_position_changes_array[clear_position_changes] = 0;
  // }

  about_to_crash = renderFractal(camera_params, camera_position_array, camera_position_changes_array, 
    camera_angle_array, camera_angle_changes_array, frame_counter, renderer_params, image);

  saveBMP(renderer_params.file_name , frame_counter, image, renderer_params.width, renderer_params.height);
  
  free(image);
  frame_counter++;
  }
  return 0;
}
