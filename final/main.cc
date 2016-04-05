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

void getParameters(char            *filename,
                   float           *camera_position_array,
                   float           *camera_position_changes_array,
                   float           *camera_angle_array,
                   float           *camera_angle_changes_array,
                   int              frame_no,
                   CameraParams    *camera_params,
                   RenderParams    *renderer_params,
                   MandelBoxParams *mandelBox_params);
void init3D(CameraParams       *camera_params,
            const RenderParams *renderer_params);
int  renderFractal(const CameraParams& camera_params,
                   float              *camera_position_array,
                   float              *camera_position_changes_array,
                   int                 move_position,
                   float              *camera_angle_array,
                   float              *camera_angle_changes_array,
                   int                 frame_no,
                   const RenderParams& renderer_params,
                   unsigned char      *image,
                   MandelBoxParams   & mandelBox_params);

void saveBMP(const char          *filename,
             int                  frame_no,
             const unsigned char *image,
             int                  width,
             int                  height);

int main(int argc, char **argv)
{
  MandelBoxParams mandelBox_params;
  CameraParams    camera_params;
  RenderParams    renderer_params;
  float camera_position_array[3];
  float camera_position_changes_array[3] = { 0, 0, 0 };
  float camera_angle_array[3];
  float camera_angle_changes_array[3] = { 0, 0, 0 };
  int   frame_counter                 = 0;
  int   move_position                 = 1;

  // int clear_position_changes;
  while (frame_counter < 2){//7200) {
    getParameters(argv[1],
                  camera_position_array,
                  camera_position_changes_array,
                  camera_angle_array,
                  camera_angle_changes_array,
                  frame_counter,
                  &camera_params,
                  &renderer_params,
                  &mandelBox_params);

    // printf("%f\n", camera_position_array[0]);
    int image_size       = renderer_params.width * renderer_params.height;
    unsigned char *image =
      (unsigned char *)malloc(3 * image_size * sizeof(unsigned char));

    init3D(&camera_params, &renderer_params);


    move_position = renderFractal(camera_params,
                                  camera_position_array,
                                  camera_position_changes_array,
                                  move_position,
                                  camera_angle_array,
                                  camera_angle_changes_array,
                                  frame_counter,
                                  renderer_params,
                                  image,
                                  mandelBox_params);

    saveBMP(renderer_params.file_name,
            frame_counter,
            image,
            renderer_params.width,
            renderer_params.height);

    free(image);
    frame_counter++;
  }
  return 0;
}

