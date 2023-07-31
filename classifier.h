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

#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "misc.h"
#include "vignette.h"

class Classifier {
public:

  // We need a virtual destructor since there are virtual methods
  virtual ~Classifier();

  // This method returns an upper-caps string to identify the classifier
  virtual const char *name() = 0;

  // Train the classifier from a set of vignettes. The labels are in the
  // vignettes.
  virtual void train(int nb_vignettes, Vignette *vignettes, int *labels) = 0;

  // Compute a scalar value which should be strictly positive on
  // positive vignettes and strictly negative on negative ones.
  virtual scalar_t classify(Vignette *vignette) = 0;

  // Read or write the classifier from or to a stream
  virtual void read(istream *in) = 0;
  virtual void write(ostream *out) = 0;
};

#endif
