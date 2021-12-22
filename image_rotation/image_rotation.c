#include <omp.h>
#include <stdio.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

int main() {
   int w,h,n;
   // Read an image file into memory
   unsigned char *data = stbi_load("cat2.png", &w, &h, &n, 0);
   unsigned char *dest_data = stbi_load("white.png", &w, &h, &n, 0);
   printf("width = %d, height = %d, component (rgba) = %d\n",w,h,n); 

   // Define the center of the image
   float y_center = w/2;
   float x_center = w/2;
   
   int i,j;
   //Rotation processing in parallel 
   #pragma omp parallel num_threads(4)
   {
      #pragma omp for
      for (j=0; j < h; j++) {
         int count = 0;
         for (i=0; i < w; i++) {
            int xpos = (int) (cos(45) * (i - x_center) + sin(45) * (j - y_center) + x_center); //Find x position
            int ypos = (int) (-sin(45) * (i - x_center) + cos(45) * (j - y_center) + y_center); //Find y position
            //Check the border of the image
            if(((int)xpos>=0) && ((int)xpos< w) && ((int)ypos>=0) && ((int)ypos< h)){
               //Change the old data image to new data image
               dest_data[(j*w+i)*n+0] = (data[(xpos*w+ypos)*n+0]+data[(xpos*w+ypos)*n+1]+data[(xpos*w+ypos)*n+2])/3;
               dest_data[(j*w+i)*n+1] = (data[(xpos*w+ypos)*n+0]+data[(xpos*w+ypos)*n+1]+data[(xpos*w+ypos)*n+2])/3;
               dest_data[(j*w+i)*n+2] = (data[(xpos*w+ypos)*n+0]+data[(xpos*w+ypos)*n+1]+data[(xpos*w+ypos)*n+2])/3;
            }
            else{
               //Not in the image
               dest_data[(j*w+i)*n+0] = 0;
               dest_data[(j*w+i)*n+1] = 0;
               dest_data[(j*w+i)*n+2] = 0;
            }
         }
      }
   }
   // Write image in memory to file
   if (!stbi_write_png("imgout.png",w,h,n,dest_data,w*n)) {
      printf("Failed to write file\n"); 
   }
}

