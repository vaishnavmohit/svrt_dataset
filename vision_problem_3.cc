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

#include "vision_problem_3.h"
#include "shape.h"

VisionProblem_3::VisionProblem_3() { }

void VisionProblem_3::generate(int label, Vignette *vignette) {
  int nb_shapes = 4;
  Vignette avoid, tmp;
  const int dist_min = Vignette::width / 8;

  int nb_attempts;
  const int max_nb_attempts = 100;

  do {
    avoid.clear();
    vignette->clear();

    nb_attempts = 0;

    for(int s = 0; nb_attempts < max_nb_attempts && s < nb_shapes; s++) {
      Shape shape;

      int xs, ys, i, right_margin, right_connection;

      do {
        tmp.clear();
        do {
          do {
            xs = int(drand48() * Vignette::width);
            ys = int(drand48() * Vignette::height);
            shape.randomize(part_size, hole_size);
          }  while(shape.overwrites(&tmp, xs, ys)); // check not out-of-vignette

          // omg this is ugly
          if(label && s == 1) {
            right_margin = 1;
          } else {
            right_margin = !shape.overwrites(&avoid, xs, ys);
          }

          if((label && (s == 1 || s == 3)) || (!label && (s >= 2))) {
            right_connection = shape.overwrites(vignette, xs, ys);
          } else {
            right_connection = 1;
          }

          nb_attempts++;

        } while(nb_attempts < max_nb_attempts && !right_margin);

        shape.draw(&tmp, xs, ys);
        tmp.fill(xs, ys, 0);

        if(right_margin && right_connection) {
          if((label && (s == 1 || s == 3)) || (!label && (s >= 2))) {
            i = vignette->intersection(&tmp);
            right_connection = (i > 0) && (i < 4);
          } else {
            right_connection = 1;
          }
        } else right_connection = 0; // To avoid compilation warning
      } while(nb_attempts < max_nb_attempts && (!right_margin || !right_connection));

      if(nb_attempts < max_nb_attempts) {
        shape.draw(vignette, xs, ys);
        vignette->fill(xs, ys, 128);
        if((label && s < 2) || (!label && s < 1)) {
          for(int k = 0; k < dist_min; k++) tmp.grow();
          avoid.superpose(&avoid, &tmp);
        }
      }
    }
  } while(nb_attempts >= max_nb_attempts);

  vignette->replace_value(128, 255);
}
