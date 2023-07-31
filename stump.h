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

#ifndef STUMP_H
#define STUMP_H

#include "misc.h"
#include "vignette.h"

void compute_integral_image(Vignette *vignette, int *integral_mage);

class Stump {
public:
  int roi_x, roi_y, roi_w, roi_h;
  int k1, k2, k3, k4;
  scalar_t threshold;
  scalar_t weight0, weight1;

  inline scalar_t count(int *iimage) {
    return scalar_t(iimage[k1] + iimage[k4] - iimage[k2] - iimage[k3]);
  }

  inline scalar_t response(int *iimage) {
    if(count(iimage) >= threshold) {
      return weight1;
    } else {
      return weight0;
    }
  }

  void randomize();
};

#endif
