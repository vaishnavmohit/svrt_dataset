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

#include "boosted_classifier.h"
#include "classifier_reader.h"
#include "fusion_sort.h"
#include "tools.h"

inline scalar_t loss_derivative(int label, scalar_t response) {
  return - scalar_t(label * 2 - 1) * exp( - scalar_t(label * 2 - 1) * response );
}

BoostedClassifier::BoostedClassifier() {
  _nb_stumps = 0;
  _stumps = 0;
}

BoostedClassifier::BoostedClassifier(int nb_weak_learners) {
  _nb_stumps = nb_weak_learners;
  _stumps = new Stump[_nb_stumps];
}

BoostedClassifier::~BoostedClassifier() {
  delete[] _stumps;
}

const char *BoostedClassifier::name() {
  return "ADABOOST";
}

void BoostedClassifier::chose_stump_from_sampling(int t, int **integral_images, scalar_t *derivatives, int nb_samples) {
  int *indexes = new int[nb_samples];
  int *sorted_indexes = new int[nb_samples];
  scalar_t *stump_counts = new scalar_t[nb_samples];

  scalar_t max_loss_derivative = 0;
  Stump tmp;
  tmp.weight0 = -1;
  tmp.weight1 =  1;
  tmp.threshold = 0;

  for(int k = 0; k < global.nb_optimization_weak_learners; k++) {
    tmp.randomize();
    scalar_t s = 0;

    for(int n = 0; n < nb_samples; n++) {
      stump_counts[n] = tmp.count(integral_images[n]);
      indexes[n] = n;
      s += derivatives[n];
    }

    indexed_fusion_sort(nb_samples, indexes, sorted_indexes, stump_counts);

    for(int n = 0; n < nb_samples - 1; n++) {
      int i = sorted_indexes[n];
      int j = sorted_indexes[n + 1];
      s -= 2 * derivatives[i];
      if(stump_counts[j] > stump_counts[i]) {
        if(abs(s) > abs(max_loss_derivative)) {
          max_loss_derivative = s;
          _stumps[t] = tmp;
          _stumps[t].threshold = (stump_counts[i] + stump_counts[j])/2;
        }
      }
    }
  }

  delete[] stump_counts;
  delete[] indexes;
  delete[] sorted_indexes;
}

void BoostedClassifier::chose_stump(int t, int **integral_images, scalar_t *derivatives, int nb_samples) {
  if(global.nb_sampled_samples <= 0) {
    chose_stump_from_sampling(t, integral_images, derivatives, nb_samples);
  } else {
    int *sampled_indexes = new int[global.nb_sampled_samples];
    scalar_t *weights = new scalar_t[nb_samples];
    for(int s = 0; s < nb_samples; s++) {
      weights[s] = abs(derivatives[s]);
    }
    robust_sampling(nb_samples, weights, global.nb_sampled_samples, sampled_indexes);
    delete[] weights;

    int **sampled_integral_images = new int *[global.nb_sampled_samples];
    scalar_t *sampled_derivatives = new scalar_t[global.nb_sampled_samples];

    for(int s = 0; s < global.nb_sampled_samples; s++) {
      sampled_integral_images[s] = integral_images[sampled_indexes[s]];
      if(derivatives[sampled_indexes[s]] > 0) {
        sampled_derivatives[s] = 1.0;
      } else {
        sampled_derivatives[s] = -1.0;
      }
    }

    chose_stump_from_sampling(t, sampled_integral_images, sampled_derivatives, global.nb_sampled_samples);

    delete[] sampled_derivatives;
    delete[] sampled_integral_images;
    delete[] sampled_indexes;
  }
}

void BoostedClassifier::train(int nb_vignettes, Vignette *vignettes, int *labels) {
  int **integral_images = new int *[nb_vignettes];

  for(int n = 0; n < nb_vignettes; n++) {
    integral_images[n] = new int[(Vignette::width + 1) * (Vignette::height + 1)];
    compute_integral_image(&vignettes[n], integral_images[n]);
  }

  scalar_t *responses = new scalar_t[nb_vignettes];
  scalar_t *derivatives = new scalar_t[nb_vignettes];

  for(int n = 0; n < nb_vignettes; n++) {
    responses[n] = 0.0;
  }

  global.bar.init(&cout, _nb_stumps);
  for(int t = 0; t < _nb_stumps; t++) {

    for(int n = 0; n < nb_vignettes; n++) {
      derivatives[n] = loss_derivative(labels[n], responses[n]);
    }

    chose_stump(t, integral_images, derivatives, nb_vignettes);

    scalar_t num0 = 0, den0 = 0, num1 = 0, den1 = 0;

    for(int n = 0; n < nb_vignettes; n++) {
      if(_stumps[t].response(integral_images[n]) > 0) {
        if(labels[n] == 1) {
          num1 += exp( - responses[n] );
        } else {
          den1 += exp(   responses[n] );
        }
      } else {
        if(labels[n] == 1) {
          num0 += exp( - responses[n] );
        } else {
          den0 += exp(   responses[n] );
        }
      }
    }

    scalar_t weight_max = 5.0;

    _stumps[t].weight0 = 0.5 * log(num0 / den0);

    if(_stumps[t].weight0 < -weight_max)
      _stumps[t].weight0 = -weight_max;
    else if(_stumps[t].weight0 > weight_max)
      _stumps[t].weight0 = weight_max;

    _stumps[t].weight1 = 0.5 * log(num1 / den1);
    if(_stumps[t].weight1 < -weight_max)
      _stumps[t].weight1 = -weight_max;
    else if(_stumps[t].weight1 > weight_max)
      _stumps[t].weight1 = weight_max;

    for(int n = 0; n < nb_vignettes; n++) {
      responses[n] += _stumps[t].response(integral_images[n]);
    }

    // cout << "ADABOOST_STEP " << t + 1 << " " << loss << endl;
    global.bar.refresh(&cout, t);
  }
  global.bar.finish(&cout);

  scalar_t loss = 0;
  for(int n = 0; n < nb_vignettes; n++) {
    loss += exp( - scalar_t(labels[n] * 2 - 1) * responses[n]);
  }

  cout << "Final loss is " << loss << endl;

  delete[] derivatives;
  delete[] responses;

  for(int n = 0; n < nb_vignettes; n++) {
    delete[] integral_images[n];
  }

  delete[] integral_images;
}

scalar_t BoostedClassifier::classify(Vignette *vignette) {
  int integral_image[(Vignette::width + 1) * (Vignette::height + 1)];
  compute_integral_image(vignette, integral_image);
  scalar_t result = 0;
  for(int n = 0; n < _nb_stumps; n++) {
    result += _stumps[n].response(integral_image);
  }
  return result;
}

void BoostedClassifier::read(istream *in) {
  delete[] _stumps;
  read_var(in, &_nb_stumps);
  cout << "Reading " << _nb_stumps << " stumps." << endl;
  _stumps = new Stump[_nb_stumps];
  in->read((char *) _stumps, sizeof(Stump) * _nb_stumps);
}

void BoostedClassifier::write(ostream *out) {
  int t;
  t = CT_BOOSTED;
  write_var(out, &t);
  write_var(out, &_nb_stumps);
  out->write((char *) _stumps, sizeof(Stump) * _nb_stumps);
}

scalar_t BoostedClassifier::partial_sum(int first, int nb, Vignette *vignette) {
  int integral_image[(Vignette::width + 1) * (Vignette::height + 1)];
  compute_integral_image(vignette, integral_image);
  scalar_t result = 0;
  for(int n = first; n < first + nb; n++) {
    result += _stumps[n].response(integral_image);
  }
  return result;
}
