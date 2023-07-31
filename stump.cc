/*
 *  svrt is the ``Synthetic Visual Reasoning Test'', an image
 *  generator for evaluating classification performance of machine
 *  learning systems, humans and primates.
 *
 *  Copyright (c) 2009 Idiap Research Institute, http://www.idiap.ch/
 *  Written by Francois Fleuret <francois.fleuret@idiap.ch>
 *
 *  This file is part of svrt.
 *
 *  svrt is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 *
 *  svrt is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with svrt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stump.h"

void compute_integral_image(Vignette *vignette, int *integral_image) {
  int k = 0;
  for(int y = 0; y <= Vignette::height; y++) {
    for(int x = 0; x <= Vignette::width; x++) {
      if(x == 0 || y == 0) {
        integral_image[k] = 0;
      } else {
        ASSERT(k >= (Vignette::width + 1) + 1);
        integral_image[k] =  vignette->content[x - 1 + Vignette::width * (y - 1)]
          + integral_image[k - (Vignette::width + 1)]
          + integral_image[k - 1]
          - integral_image[k - (Vignette::width + 1) - 1];
      }
      k++;
    }
  }
}


void Stump::randomize() {
  do {
    roi_x = int(drand48() * Vignette::width);
    roi_y = int(drand48() * Vignette::height);
    roi_w = int(drand48() * Vignette::width);
    roi_h = int(drand48() * Vignette::height);
    // #warning *!*!*!*!*!*!*!*!*!*!*!*!*!*
    // roi_w = int(drand48() * Vignette::width/2);
    // roi_h = int(drand48() * Vignette::height/2);
  } while(roi_x + roi_w > Vignette::width || roi_y + roi_h > Vignette::height);

  k1 = (roi_x +     0) + (roi_y +     0) * (Vignette::width + 1);
  k2 = (roi_x + roi_w) + (roi_y +     0) * (Vignette::width + 1);
  k3 = (roi_x +     0) + (roi_y + roi_h) * (Vignette::width + 1);
  k4 = (roi_x + roi_w) + (roi_y + roi_h) * (Vignette::width + 1);
}
