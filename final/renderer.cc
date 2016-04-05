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
#include <time.h>

extern float getTime();
extern void  printProgress(float perc,
                           float time);

extern float getTime();
extern void  printProgress(float perc,
                           float time);

extern float rayMarch(const RenderParams& render_params,
                      const vec3        & from,
                      const vec3        & to,
                      float               eps,
                      pixelData         & pix_data,
                      MandelBoxParams   & mandelBox_params);

extern vec3 getColour(const pixelData   & pixData,
                      const RenderParams& render_params,
                      const vec3        & from,
                      const vec3        & direction);

int renderFractal(const CameraParams& camera_params,
                  float              *camera_position_array,
                  float              *camera_position_changes_array,
                  int                 move_position,
                  float              *camera_angle_array,
                  float              *camera_angle_changes_array,
                  int                 frame_no,
                  const RenderParams& renderer_params,
                  unsigned char      *image,
                  MandelBoxParams   & mandelBox_params)
{
  const float eps = pow(10.0, renderer_params.detail);
  vec3 from;

  SET_POINT(from, camera_params.camPos)

  const int height = renderer_params.height;
  const int width = renderer_params.width;
  int j;

  // float time = getTime();


  float *distance_to_pixel_array;
  float *distance_to_point_array;
  vec3  *distance_vector_points;
  float  step_size   = 0.01;
  int    pin_spacing = 5;

  // int check_frame_path = 30;
  distance_to_pixel_array =
    (float *)malloc(sizeof(float) * (width * height / pin_spacing));
  distance_to_point_array =
    (float *)malloc(sizeof(float) * (width * height / pin_spacing));
  distance_vector_points =
    (vec3 *)malloc(sizeof(vec3) * (width * height / pin_spacing));
  int   pixel_count                = 0;
  int   distance_to_pixel_index    = 0;
  int   min_reached_flag           = 0;
  float max_threshold              = 3;
  float change_direction_threshold = 0.06;
  float min_threshold              = 0.03;
  srand(time(NULL));

#pragma omp parallel for default(shared) schedule(dynamic) num_threads(4)

  for (j = 0; j < height; j++) {
    int i = 0;

    for (i = 0; i < width; i++) {
      pixelData pix_data;
      vec3  color;
      float farPoint[3];
      vec3  to;

      // get point on the 'far' plane
      // since we render one frame only,
      // we can use the more specialized method
      UnProject(i, j, camera_params, farPoint);

      // to = farPoint - camera_params.camPos
      to = SUBDUBDUB(farPoint, camera_params.camPos);
      NORMALIZE(to);

      // render the pixel

      // render the pixel
      float distance_to_pixel = rayMarch(renderer_params,
                                         from,
                                         to,
                                         eps,
                                         pix_data,
                                         mandelBox_params);

      if ((i % pin_spacing == 0) && (j % pin_spacing == 0)) {
        distance_to_pixel_array[distance_to_pixel_index] = distance_to_pixel;
        distance_vector_points[distance_to_pixel_index]  = pix_data.hit;
        float new_distance =
          sqrtf(powf(pix_data.hit.x - camera_position_array[0], 2)
                + powf(pix_data.hit.y -
                       camera_position_array[1],
                       2) + powf(pix_data.hit.z - camera_position_array[3], 2));
        distance_to_point_array[distance_to_pixel_index] = new_distance;

        if (new_distance < min_threshold) {
          min_reached_flag = 1;
        }
        distance_to_pixel_index++;
      }

      // get the colour at this pixel
      color = getColour(pix_data, renderer_params, from, to);

      // save colour into texture
      int k = (j * width + i) * 3;
      image[k + 2] = (unsigned char)(color.x * 255);
      image[k + 1] = (unsigned char)(color.y * 255);
      image[k]     = (unsigned char)(color.z * 255);
      pixel_count++;
    } // inner for
      // printProgress((j+1)/(float)height,getTime()-time);
  }   // end of outer for

  if (min_reached_flag == 1) {
    move_position                  = 0;
    camera_angle_changes_array[0] += ((rand() % 200) - 100) * .01;
    camera_angle_changes_array[1] += ((rand() % 200) - 100) * .01;
    camera_angle_changes_array[2] += ((rand() % 200) - 100) * .01;
  }

  if (move_position == 1) {
    float vector_distance =
          sqrtf(powf(camera_angle_array[0] - camera_position_array[0], 2)
            + powf(camera_angle_array[1] -
                   camera_position_array[1],
                   2) + powf(camera_angle_array[3] - camera_position_array[3], 2));

    float t = step_size / vector_distance;
    camera_position_array[0] += t *
                                (camera_angle_array[0] - camera_position_array[0]);
    camera_position_array[1] += t *
                                (camera_angle_array[1] - camera_position_array[1]);
    camera_position_array[2] += t *
                                (camera_angle_array[2] - camera_position_array[2]);

    if ((vector_distance < change_direction_threshold) || (frame_no == 0)) {
      printf("\nVector Size: %f \n", vector_distance);
      move_position = 0;
      int   max_index            = 0;
      int   found_flag           = 0;
      float current_max          = 0;
      float current_max_distance = 0;
      vec3  current_max_vector_point;

      // while (found_flag == 0){

      //     if (distance_to_pixel_array[max_index] < 0.0002){
      //         current_max = distance_to_pixel_array[max_index];
      //         current_max_vector_point = distance_vector_points[max_index];
      //         current_max_distance = distance_to_point_array[max_index];
      //         found_flag = 1;
      //     }
      //     max_index++;
      // }

      // for (;max_index < distance_to_pixel_index; max_index++){
      //     if (distance_to_pixel_array[max_index] > current_max &&
      // distance_to_pixel_array[max_index] < 0.0002){
      //         current_max = distance_to_pixel_array[max_index];
      //         current_max_vector_point = distance_vector_points[max_index];
      //         current_max_distance = distance_to_point_array[max_index];
      //     }

      // }
      // printf("\nPixel Furthest: %f Point Furthest: %f\n",  current_max,
      // current_max_distance);
      max_index = 0;

      while (found_flag == 0) {
        if (distance_to_point_array[max_index] < max_threshold) {
          current_max              = distance_to_point_array[max_index];
          current_max_vector_point = distance_vector_points[max_index];
          current_max_distance     = distance_to_pixel_array[max_index];
          found_flag               = 1;
        }
        max_index++;
      }

      for (; max_index < distance_to_pixel_index; max_index++) {
        if ((distance_to_point_array[max_index] > current_max) &&
            (distance_to_point_array[max_index] < max_threshold)) {
          current_max              = distance_to_point_array[max_index];
          current_max_vector_point = distance_vector_points[max_index];
          current_max_distance     = distance_to_pixel_array[max_index];
        }
      }

      // printf("\nPoint Furthest: %f Pixel Furthest: %f\n",  current_max,
      // current_max_distance);
      camera_angle_changes_array[0] = current_max_vector_point.x;
      camera_angle_changes_array[1] = current_max_vector_point.y;
      camera_angle_changes_array[2] = current_max_vector_point.z;

      printf("\nNew Angle: %f %f %f\n",
             camera_angle_changes_array[0],
             camera_angle_changes_array[1],
             camera_angle_changes_array[2]);
    }
  }
  else
  {
    float vector_distance =
      sqrtf(powf(camera_angle_changes_array[0] - camera_angle_array[0], 2)
            + powf(camera_angle_changes_array[1] -
                   camera_angle_array[1], 2)
            + powf(camera_angle_changes_array[3] -
                   camera_angle_array[3], 2));

    float t = 0.20; // *(step_size/vector_distance);

    if (vector_distance < 0.05) {
      camera_angle_array[0] = camera_angle_changes_array[0];
      camera_angle_array[1] = camera_angle_changes_array[1];
      camera_angle_array[2] = camera_angle_changes_array[2];

      move_position = 1;
    }
    camera_angle_array[0] += t *
                             (camera_angle_changes_array[0] -
                              camera_angle_array[0]);
    camera_angle_array[1] += t *
                             (camera_angle_changes_array[1] -
                              camera_angle_array[1]);
    camera_angle_array[2] += t *
                             (camera_angle_changes_array[2] -
                              camera_angle_array[2]);
  }
  printf("\n rendering done:\n");


  free(distance_to_pixel_array);
  free(distance_to_point_array);


  return move_position;
}

