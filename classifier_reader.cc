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
#include "classifier_reader.h"
#include "naive_bayesian_classifier.h"
#include "boosted_classifier.h"

Classifier *read_classifier(istream *in) {
  Classifier *result = 0;

  unsigned int t;
  read_var(in, &t);

  switch(t) {
  case CT_NAIVE_BAYESIAN:
    cout << "Reading a CT_NAIVE_BAYESIAN." << endl;
    result = new NaiveBayesianClassifier();
    result->read(in);
    break;
  case CT_BOOSTED:
    cout << "Reading a CT_BOOSTED." << endl;
    result = new BoostedClassifier();
    result->read(in);
    break;
  default:
    cerr << "Unknown classifier type for reading." << endl;
    abort();
  }

  return result;
}
