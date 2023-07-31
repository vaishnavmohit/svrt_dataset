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

#include "naive_bayesian_classifier.h"
#include "classifier_reader.h"

NaiveBayesianClassifier::NaiveBayesianClassifier() { }

NaiveBayesianClassifier::~NaiveBayesianClassifier() { }

const char *NaiveBayesianClassifier::name() {
  return "NAIVE_BAYESIAN";
}

void NaiveBayesianClassifier::train(int nb_vignettes, Vignette *vignettes, int *labels) {
  for(int k = 0; k < Vignette::width * Vignette::height * Vignette::nb_grayscales; k++) {
    proba_given_0[k] = 0;
    proba_given_1[k] = 0;
  }

  int nb_0 = 0, nb_1 = 0;

  global.bar.init(&cout, nb_vignettes);
  for(int n = 0; n < nb_vignettes; n++) {
    if(labels[n] == 1) {
      nb_1++;
      for(int k = 0; k < Vignette::width * Vignette::height; k++) {
        proba_given_1[k * Vignette::nb_grayscales + vignettes[n].content[k]] += 1.0;
      }
    } else {
      nb_0++;
      for(int k = 0; k < Vignette::width * Vignette::height; k++) {
        proba_given_0[k * Vignette::nb_grayscales + vignettes[n].content[k]] += 1.0;
      }
    }
    global.bar.refresh(&cout, n);
  }
  global.bar.finish(&cout);

  for(int k = 0; k < Vignette::width * Vignette::height * Vignette::nb_grayscales; k++) {
    proba_given_0[k] /= scalar_t(nb_0);
    proba_given_1[k] /= scalar_t(nb_1);
  }
}

scalar_t NaiveBayesianClassifier::classify(Vignette *vignette) {
  scalar_t result = 0.0;

  for(int k = 0; k < Vignette::width * Vignette::height; k++) {
    result += log(proba_given_1[k * Vignette::nb_grayscales + vignette->content[k]])
      - log(proba_given_0[k * Vignette::nb_grayscales + vignette->content[k]]);
  }

  return result;
}

void NaiveBayesianClassifier::read(istream *in) {
  in->read((char *) proba_given_0, sizeof(scalar_t) * Vignette::width * Vignette::height * Vignette::nb_grayscales);
  in->read((char *) proba_given_1, sizeof(scalar_t) * Vignette::width * Vignette::height * Vignette::nb_grayscales);
}

void NaiveBayesianClassifier::write(ostream *out) {
  int t;
  t = CT_NAIVE_BAYESIAN;
  write_var(out, &t);
  out->write((char *) proba_given_0, sizeof(scalar_t) * Vignette::width * Vignette::height * Vignette::nb_grayscales);
  out->write((char *) proba_given_1, sizeof(scalar_t) * Vignette::width * Vignette::height * Vignette::nb_grayscales);
}
