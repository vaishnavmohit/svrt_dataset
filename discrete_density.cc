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

#include "discrete_density.h"

DiscreteDensityTree::DiscreteDensityTree(scalar_t *proba, int first_value, int nb_values) {
  if(nb_values > 1) {
    _proba_tree0 = 0;
    for(int n = 0; n < nb_values/2; n++) _proba_tree0 += proba[first_value + n];
    scalar_t s = 0;
    for(int n = 0; n < nb_values; n++) s += proba[first_value + n];
    _proba_tree0 /= s;
    _tree0 = new DiscreteDensityTree(proba, first_value, nb_values/2);
    _tree1 = new DiscreteDensityTree(proba, first_value + nb_values/2, nb_values - (nb_values/2));
  } else {
    _tree0 = 0;
    _tree1 = 0;
    _value = first_value;
  }
}

DiscreteDensityTree::~DiscreteDensityTree() {
  if(_tree0) delete _tree0;
  if(_tree1) delete _tree1;
}

int DiscreteDensityTree::sample() {
  if(_tree0) {
    if(drand48() < _proba_tree0)
      return _tree0->sample();
    else
      return _tree1->sample();
  } else return _value;
}

DiscreteDensity::DiscreteDensity(int nb_values) {
  _nb_values = nb_values;
  _probas = new scalar_t[_nb_values];
  _log_probas = new scalar_t[_nb_values];
  _sampling_tree = 0;
}

DiscreteDensity::~DiscreteDensity() {
  delete[] _probas;
  delete[] _log_probas;
  delete _sampling_tree;
}

void DiscreteDensity::set_non_normalized_proba(int n, scalar_t p) {
  _probas[n] = p;
}

void DiscreteDensity::normalize() {
  scalar_t s = 0;
  for(int k = 0; k < _nb_values; k++) {
    s += _probas[k];
  }
  for(int k = 0; k < _nb_values; k++) {
    _probas[k] /= s;
    _log_probas[k] = log(_probas[k]);
  }
  delete _sampling_tree;
  _sampling_tree = new DiscreteDensityTree(_probas, 0, _nb_values);
}

scalar_t DiscreteDensity::entropy() {
  scalar_t h = 0;
  for(int k = 0; k < _nb_values; k++) {
    if(_probas[k] > 0) h += - _probas[k] * _log_probas[k]/log(2.0);
  }
  return h;
}

scalar_t DiscreteDensity::proba(int n) {
  return _probas[n];
}

scalar_t DiscreteDensity::log_proba(int n) {
  return _log_probas[n];
}

int DiscreteDensity::sample() {
  return _sampling_tree->sample();
}
