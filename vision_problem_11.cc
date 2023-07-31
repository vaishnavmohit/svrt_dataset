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

#include "vision_problem_11.h"
#include "shape.h"

VisionProblem_11::VisionProblem_11() { }

void VisionProblem_11::generate(int label, Vignette *vignette) {
  int nb_shapes;
  int xs, ys, i, pxs, pys;
  const int dist_min = Vignette::width/12;
  int nb_attempts, max_nb_attempts = 100;

  Vignette mask, tmp;

  nb_shapes = 2;

  do {
    nb_attempts = 0;

    mask.clear();
    vignette->clear();

    pxs = 0; pys = 0;

    for(int s = 0; nb_attempts < max_nb_attempts && s < nb_shapes; s++) {
      Shape shape;

      do {
        tmp.clear();

        do {

          if(s == 0) {
            shape.randomize(big_part_size / 2, big_part_hole_size / 2);
          } else {
            shape.randomize(small_part_size / 2, small_part_hole_size / 2);
          }

          if(nb_shapes == 2 || s == 0 || label == 0) {
            xs = int(drand48() * Vignette::width);
            ys = int(drand48() * Vignette::height);
          } else {
            xs = pxs + int(4 * (drand48() - 0.5) * small_part_hole_size);
            ys = pys + int(4 * (drand48() - 0.5) * small_part_hole_size);
          }
          nb_attempts++;

        } while(nb_attempts < max_nb_attempts &&
                shape.overwrites(&tmp, xs, ys));

        shape.draw(&tmp, xs, ys);
        tmp.fill(xs, ys, 128);
        i = tmp.intersection(&mask);

        nb_attempts++;
      } while(nb_attempts < max_nb_attempts &&
              s > 0 &&
              ((label == 0 && i > 0) || (label == 1 && (i < 1 || i > 4))));

      shape.draw(vignette, xs, ys);
      pxs = xs; pys = ys;

      if(label == 0) {
        for(int k = 0; k < dist_min; k++) tmp.grow();
      }

      mask.superpose(&mask, &tmp);
    }
  } while(nb_attempts >= max_nb_attempts);
}
