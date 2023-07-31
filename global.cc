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

#include <string.h>

#include "global.h"

Global global;

Global::Global() { }

Global::~Global() { }

void Global::init_parser(ParamParser *parser) {
  // The nice level of the process
  parser->add_association("niceness", "15", false);
  // Seed to initialize the random generator
  parser->add_association("random_seed", "0", false);

  // Where to put the generated files
  parser->add_association("result_path", "/tmp/", false);
  // The classifier filename
  parser->add_association("classifier_name", "default.clf", false);

  // Should we display a progress bar for lengthy operations
  parser->add_association("progress_bar", "yes", false);

  // The problem number
  parser->add_association("problem_number", "0", false);

  // The number of samples for training and testing
  parser->add_association("nb_train_samples", "1000", false);
  parser->add_association("nb_test_samples", "1000", false);
  parser->add_association("nb_weak_learners", "1000", false);
  parser->add_association("nb_optimization_weak_learners", "100", false);
  parser->add_association("nb_sampled_samples", "-1", false);
}

void Global::read_parser(ParamParser *parser) {
  niceness = parser->get_association_int("niceness");
  random_seed = parser->get_association_int("random_seed");

  strncpy(result_path, parser->get_association("result_path"), buffer_size);
  strncpy(classifier_name, parser->get_association("classifier_name"), buffer_size);
  if(!classifier_name[0]) {
    sprintf(classifier_name, "%s/default.clf", result_path);
  }

  bar.set_visible(parser->get_association_bool("progress_bar"));

  problem_number = parser->get_association_int("problem_number");

  nb_train_samples = parser->get_association_int("nb_train_samples");
  nb_test_samples = parser->get_association_int("nb_test_samples");
  nb_weak_learners = parser->get_association_int("nb_weak_learners");
  nb_optimization_weak_learners = parser->get_association_int("nb_optimization_weak_learners");
  nb_sampled_samples = parser->get_association_int("nb_sampled_samples");
}
