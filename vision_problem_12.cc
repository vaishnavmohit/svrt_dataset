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

#include "vision_problem_12.h"
#include "shape.h"

VisionProblem_12::VisionProblem_12() { }

void VisionProblem_12::generate(int label, Vignette *vignette) {
  int nb_shapes = 3;
  scalar_t alpha, beta, gamma;
  int xs, ys;
  Shape shape;

  int error;

  do {
    scalar_t xc, yc, radius;
    xc = (drand48() * 0.5 + 0.25) * Vignette::width;
    yc = (drand48() * 0.5 + 0.25) * Vignette::height;
    radius = (drand48() * 0.5 + 0.1) * Vignette::width;
    alpha = drand48() * 2 * M_PI;
    beta = (drand48() * 0.4 + 0.1) * M_PI;

    vignette->clear();
    error = 0;

    for(int n = 0; n < nb_shapes; n++) {
      if(label) {
        if(n == 0)
          gamma = alpha + M_PI - beta/2;
        else if(n == 1)
          gamma = alpha + M_PI + beta/2;
        else
          gamma = alpha;
      } else {
        if(n == 0)
          gamma = alpha + M_PI - beta/2;
        else if(n == 1)
          gamma = alpha;
        else
          gamma = alpha + M_PI + beta/2;
      }

      if(n < 2) {
        shape.randomize(small_part_size, small_part_hole_size);
      } else {
        shape.randomize(big_part_size, big_part_hole_size);
      }

      xs = int(xc + radius * cos(gamma));
      ys = int(yc + radius * sin(gamma));

      error |= shape.overwrites(vignette, xs, ys);
      shape.draw(vignette, xs, ys);
    }
  } while(error);
}
