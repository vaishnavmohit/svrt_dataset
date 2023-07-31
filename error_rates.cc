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

#include "error_rates.h"
#include "rgb_image.h"

scalar_t error_rate(Classifier *classifier, int nb_vignettes, Vignette *vignettes, int *labels) {
  int e = 0;

  for(int n = 0; n < nb_vignettes; n++) {
    if(classifier->classify(&vignettes[n]) >= 0) {
      if(labels[n] == 0) e++;
    } else {
      if(labels[n] == 1) e++;
    }
  }

  return scalar_t(e)/scalar_t(nb_vignettes);
}

scalar_t test_error_rate(VignetteGenerator *generator, Classifier *classifier, long int nb_to_try) {
  scalar_t e = 0;
  Vignette vignette;
  int label;

  global.bar.init(&cout, nb_to_try);

  for(long int k = 0; k < nb_to_try; k++) {
    label = int(drand48() * 2);
    generator->generate(label, &vignette);
    if(classifier->classify(&vignette) >= 0) {
      if(label == 0) e++;
    } else {
      if(label == 1) e++;
    }
    global.bar.refresh(&cout, k);
  }
  global.bar.finish(&cout);

  return scalar_t(e)/scalar_t(nb_to_try);
}


void compute_response_mu_and_sigma(int nb_samples, Vignette *vignette, Classifier *classifier,
                                   scalar_t *mu, scalar_t *sigma) {
  scalar_t sum = 0, sum_sq = 0;
  const int nb_pixels_to_switch = 1;
  int changed_pixels[nb_pixels_to_switch];

  for(int n = 0; n < nb_samples; n++) {
    for(int p = 0; p < nb_pixels_to_switch; p++) {
      changed_pixels[p] = int(drand48() * Vignette::width * Vignette::height);
      vignette->content[changed_pixels[p]] = 255 - vignette->content[changed_pixels[p]];
    }

    scalar_t r = classifier->classify(vignette);
    sum += r;
    sum_sq += sq(r);

    for(int p = 0; p < nb_pixels_to_switch; p++) {
      vignette->content[changed_pixels[p]] = 255 - vignette->content[changed_pixels[p]];
    }
  }

  *mu = sum_sq/scalar_t(nb_samples);
  *sigma = *mu - sq(sum/scalar_t(nb_samples));
}
