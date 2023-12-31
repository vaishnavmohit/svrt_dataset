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

#include "vision_problem_7.h"
#include "shape.h"

VisionProblem_7::VisionProblem_7() { }

void VisionProblem_7::generate(int label, Vignette *vignette) {
  int nb_shapes;

  nb_shapes = 6;

  vignette->clear();
  Shape shape;

  for(int s = 0; s < nb_shapes; s++) {
    if((label == 0 && s%2 == 0) || (label == 1 && s%3 == 0)) {
      shape.randomize(part_size, part_hole_size);
    }

    int xs, ys;
    do {
      xs = int(drand48() * Vignette::width);
      ys = int(drand48() * Vignette::height);
    } while(shape.overwrites(vignette, xs, ys));
    shape.draw(vignette, xs, ys);
  }
}
