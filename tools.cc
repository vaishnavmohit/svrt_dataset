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

#include "misc.h"
#include "tools.h"
#include "fusion_sort.h"

scalar_t robust_sampling(int nb, scalar_t *weights, int nb_to_sample, int *sampled) {
  ASSERT(nb > 0);
  if(nb == 1) {
    for(int k = 0; k < nb_to_sample; k++) sampled[k] = 0;
    return weights[0];
  } else {
    scalar_t *pair_weights = new scalar_t[(nb+1)/2];
    for(int k = 0; k < nb/2; k++)
      pair_weights[k] = weights[2 * k] + weights[2 * k + 1];
    if(nb%2)
      pair_weights[(nb+1)/2 - 1] = weights[nb-1];
    scalar_t result = robust_sampling((nb+1)/2, pair_weights, nb_to_sample, sampled);
    for(int k = 0; k < nb_to_sample; k++) {
      int s = sampled[k];
      // There is a bit of a trick for the isolated sample in the odd
      // case. Since the corresponding pair weight is the same as the
      // one sample alone, the test is always true and the isolated
      // sample will be taken for sure.
      if(drand48() * pair_weights[s] <= weights[2 * s])
        sampled[k] = 2 * s;
      else
        sampled[k] = 2 * s + 1;
    }
    delete[] pair_weights;
    return result;
  }
}
