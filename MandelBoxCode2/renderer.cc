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
#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"
#include <math.h>


extern float getTime();
extern void   printProgress( float perc, float time );

extern float rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, float eps, pixelData &pix_data);
extern vec3 getColour(const pixelData &pixData, const RenderParams &render_params,
		      const vec3 &from, const vec3  &direction);

int renderFractal(const CameraParams &camera_params, float *camera_position_array,
  float *camera_position_changes_array,
float *camera_angle_array, float *camera_angle_changes_array, int frame_no,
  const RenderParams &renderer_params, unsigned char* image)
{

  
  const float eps = powf(10.0, renderer_params.detail); 
  float farPoint[3];
  vec3 to, from;
  
  from.SetfloatPoint(camera_params.camPos);
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;

  
  float *distance_to_pixel_array;
  int *pixel_position_width;
  int *pixel_position_height;
  float step_size = 0.05;
  float straight[3] = {0,0,0};
  float left_right[3] = {0,0,0};
  float up_down[3] = {0,0,0};
  int pin_spacing = 10;
  int check_frame_path = 5;
  distance_to_pixel_array =  (float *)malloc(sizeof(float) * (width*height/pin_spacing));
  pixel_position_height =  (int *)malloc(sizeof(int) * (width*height/pin_spacing));
  pixel_position_width =  (int *)malloc(sizeof(int) * (width*height/pin_spacing));
  pixelData pix_data;
  
  float time = getTime();
  vec3 color;
  
  int i,j,k;
  int pixel_count = 0;
  int distance_to_pixel_index = 0;

  for(j = 0; j < height; j++)
    {
      //for each column pixel in the row
      for(i = 0; i <width; i++)
	{
	  // get point on the 'far' plane
	  // since we render one frame only, we can use the more specialized method
	  UnProject(i, j, camera_params, farPoint);


	  // to = farPoint - camera_params.camPos
	  to = Subtractfloatfloat(farPoint,camera_params.camPos);
	  to.Normalize();
	  
	  //render the pixel
	  float distance_to_pixel = rayMarch(renderer_params, from, to, eps, pix_data);
    // printf("%f\n", distance_to_pixel );
	  // distance_to_pixel_array[j*width + i] = distance_to_pixel;
    if (pixel_count%pin_spacing == 0){
      distance_to_pixel_array[distance_to_pixel_index] = distance_to_pixel;
      pixel_position_width[distance_to_pixel_index] = i;
      pixel_position_height[distance_to_pixel_index] = j;
      distance_to_pixel_index++;
	  }

    //get the colour at this pixel
	  color = getColour(pix_data, renderer_params, from, to);
      
	  //save colour into texture
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color.x * 255);
	  image[k+1] = (unsigned char)(color.y * 255);
	  image[k]   = (unsigned char)(color.z * 255);
    pixel_count++;
	}
      // printf("%d", distance_to_pixel_index);
      printProgress((j+1)/(float)height,getTime()-time);
    }
  int max_index;
  float current_max = distance_to_pixel_array[0];
  int current_max_position_width = pixel_position_width[0];
  int current_max_position_height = pixel_position_height[0];
  for (max_index = 1; max_index < distance_to_pixel_index; max_index++){
      if (distance_to_pixel_array[max_index] > current_max && distance_to_pixel_array[max_index] < 0.02){
          current_max = distance_to_pixel_array[max_index];
          current_max_position_width = pixel_position_width[max_index];
          current_max_position_height = pixel_position_height[max_index];
      }

  }
  int min_index;
  float current_min = distance_to_pixel_array[0];
  int current_min_position_width = pixel_position_width[0];
  int current_min_position_height = pixel_position_height[0];
  for (min_index = 1; min_index < distance_to_pixel_index; min_index++){
      if (distance_to_pixel_array[min_index] < current_min && distance_to_pixel_array[min_index] < 0.02){
          current_min = distance_to_pixel_array[min_index];
          current_min_position_width = pixel_position_width[min_index];
          current_min_position_height = pixel_position_height[min_index];
      }

  }
// if (current_min <= 0.00005){
//   int clear_position_changes;
//   for (clear_position_changes = 0; clear_position_changes < 3; clear_position_changes++){
//   camera_position_changes_array[clear_position_changes] = 0;
//   }
//   camera_angle_changes_array[0] = step_size*2; 
//   printf("ITS TURNING!\n\n"); 

// }
if (frame_no%check_frame_path == 0 || current_min <= 0.0004){
    int clear_position_changes;
    for (clear_position_changes = 0; clear_position_changes < 3; clear_position_changes++){
    camera_position_changes_array[clear_position_changes] = 0;
    }
    for (clear_position_changes = 0; clear_position_changes < 3; clear_position_changes++){
    camera_angle_changes_array[clear_position_changes] = 0;
    }
    int on_left = 0;
    if (current_min_position_width <= width/2){
      on_left = 1;
    }
    float offset_angle = atanf((camera_angle_array[0] -camera_position_array[0])/(camera_angle_array[2] -camera_position_array[2]));
    // printf("\n tar_z: %f cam_z: %f angle: %f\n",camera_angle_array[2], camera_position_array[2],offset_angle);
    // float step_size_x = 0.05;
    // float step_size_y = 0.05;
    // float step_size_z = 0.05;
    // float step_size = 0.05;
    float ratio_scale = 1.57;

    straight[0] = step_size*(sinf(offset_angle)/ratio_scale);
    straight[1] = 0;//step_size*sinf(offset_angle);
    straight[2] = step_size*(cosf(offset_angle)/ratio_scale);
    left_right[0] = step_size*(cosf(offset_angle)/ratio_scale);
    left_right[1] = 0;//step_size*cosf(offset_angle);
    left_right[2] = step_size*(sinf(offset_angle)/ratio_scale);
    up_down[1] = step_size;
    // if (frame_no%(check_frame_path*2) == 0){
    // camera_position_changes_array[0] -= (straight[0]);
    // camera_position_changes_array[1] += (straight[1]);
    // camera_position_changes_array[2] -= (straight[2]);
    // }
    // else{
    //   camera_position_changes_array[0] -= (left_right[0]);
    //   camera_position_changes_array[1] += (left_right[1]);
    //   camera_position_changes_array[2] -= (left_right[2]);
    // }
  if (current_max_position_width <= (width/2-width/5) && current_max_position_height <= (height/2-height/5) 
    && on_left !=1){
      camera_position_changes_array[0] -= (straight[0] + left_right[0]);
      camera_position_changes_array[1] += (straight[1] + left_right[1] + up_down[1]);
      camera_position_changes_array[2] -= (straight[2] + left_right[2]);
      camera_angle_changes_array[0] -= 2*(straight[0] + left_right[0]);
      camera_angle_changes_array[1] += (straight[1] + left_right[1] + up_down[1]);
      camera_angle_changes_array[2] -= 2*(straight[2] + left_right[2]);

      printf("\ndistance: %f \t move: top left", current_max);
  }

  else if (current_max_position_width > (width/2+width/5) && current_max_position_height <= (height/2-height/5
    && on_left ==1)){
      camera_position_changes_array[0] += (straight[0] + left_right[0]);
      camera_position_changes_array[1] += (straight[1] + left_right[1] + up_down[1]);
      camera_position_changes_array[2] -= (straight[2] + left_right[2]);
      camera_angle_changes_array[0] += 2*(straight[0] + left_right[0]);
      camera_angle_changes_array[1] += (straight[1] + left_right[1] + up_down[1]);
      camera_angle_changes_array[2] -= 2*(straight[2] + left_right[2]);
      // camera_position_changes_array[0] = camera_position_changes_array[0] + step_size_x;
      // camera_position_changes_array[1] = camera_position_changes_array[1] + step_size_y;
      // camera_position_changes_array[2] = camera_position_changes_array[2] - step_size_z;
      printf("\ndistance: %f \t move: top right", current_max);
  }
  else if (current_max_position_width <= (width/2-width/5) && current_max_position_height > (height/2+height/5) 
    && on_left != 1){
      camera_position_changes_array[0] -= (straight[0] + left_right[0]);
      camera_position_changes_array[1] -= (straight[1] + left_right[1] + up_down[1]);
      camera_position_changes_array[2] -= (straight[2] + left_right[2]);
      camera_angle_changes_array[0] -= 2*(straight[0] + left_right[0]);
      camera_angle_changes_array[1] -= (straight[1] + left_right[1] + up_down[1]);
      camera_angle_changes_array[2] -= 2*(straight[2] + left_right[2]);

      // camera_angle_changes_array[0] -= step_size*2; 
      // camera_position_changes_array[0] = camera_position_changes_array[0] - step_size_x;
      // camera_position_changes_array[1] = camera_position_changes_array[1] - step_size_y;
      // camera_position_changes_array[2] = camera_position_changes_array[2] - step_size_z;
      printf("\ndistance: %f \t move: bottom left", current_max);
  }
  else if (current_max_position_width > (width/2+width/5) && current_max_position_height > (height/2+height/5) 
    && on_left == 1){
      camera_position_changes_array[0] += (straight[0] + left_right[0]);
      camera_position_changes_array[1] -= (straight[1] + left_right[1] + up_down[1]);
      camera_position_changes_array[2] -= (straight[2] + left_right[2]);
      camera_angle_changes_array[0] += 2*(straight[0] + left_right[0]);
      camera_angle_changes_array[1] -= (straight[1] + left_right[1] + up_down[1]);
      camera_angle_changes_array[2] -= 2*(straight[2] + left_right[2]);
     
      // camera_position_changes_array[0] = camera_position_changes_array[0] + step_size_x;
      // camera_position_changes_array[1] = camera_position_changes_array[1] - step_size_y;
      // camera_position_changes_array[2] = camera_position_changes_array[2] - step_size_z;
      printf("\ndistance: %f \t move: bottom right", current_max);
  }
  else
  {
      camera_position_changes_array[0] += (straight[0]);
      camera_position_changes_array[1] -= (straight[1]);
      camera_position_changes_array[2] -= (straight[2]);
      camera_angle_changes_array[0] += (straight[0]);
      camera_angle_changes_array[1] -= (straight[1]);
      camera_angle_changes_array[2] -= (straight[2]);
  }
}

  // FILE *f;

  // f = fopen("distances.txt", "a");
  // fprintf(f, "%f\twidth: %d height: %d\n", current_min, current_min_position_width, current_min_position_height);
  // fclose(f);
  // printf("\ndistance: %f \t pixel_no: %d ", current_max, current_max_position_width);
  printf("\n rendering done:\n");


  free(distance_to_pixel_array);
  free(pixel_position_height);
  free(pixel_position_width);

    return 1;
}





