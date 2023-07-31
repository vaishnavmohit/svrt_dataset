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

#ifndef DISCRETE_DENSITY_H
#define DISCRETE_DENSITY_H

#include "misc.h"

class DiscreteDensityTree {
  scalar_t _proba_tree0;
  DiscreteDensityTree *_tree0, *_tree1;
  int _value;
public:
  DiscreteDensityTree(scalar_t *proba, int first_value, int nb_values);
  ~DiscreteDensityTree();
  int sample();
};

class DiscreteDensity {
  DiscreteDensityTree *_sampling_tree;
  int _nb_values;
  scalar_t *_probas, *_log_probas;
public:
  DiscreteDensity(int nb_values);
  ~DiscreteDensity();

  void set_non_normalized_proba(int n, scalar_t p);
  void normalize();
  scalar_t entropy();

  scalar_t proba(int n);
  scalar_t log_proba(int n);
  int sample();
};

#endif
