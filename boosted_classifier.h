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

#ifndef BOOSTED_CLASSIFIER_H
#define BOOSTED_CLASSIFIER_H

#include "classifier.h"
#include "stump.h"

class BoostedClassifier : public Classifier {
  Stump *_stumps;
  int _nb_stumps;
  void chose_stump_from_sampling(int t, int **integral_images, scalar_t *derivatives, int nb_samples);
  void chose_stump(int t, int **integral_images, scalar_t *derivatives, int nb_samples);
public:
  BoostedClassifier();
  BoostedClassifier(int nb_weak_learners);
  ~BoostedClassifier();
  inline int nb_stumps() { return _nb_stumps; }
  virtual const char *name();
  virtual void train(int nb_vignettes, Vignette *vignettes, int *labels);
  virtual scalar_t classify(Vignette *vignette);
  virtual void read(istream *in);
  virtual void write(ostream *out);
  scalar_t partial_sum(int first, int nb, Vignette *vignette);
};

#endif
