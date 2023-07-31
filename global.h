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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>

using namespace std;

#include "misc.h"
#include "param_parser.h"
#include "progress_bar.h"

class Global {
public:
  int niceness;
  int random_seed;
  char result_path[buffer_size];
  char classifier_name[buffer_size];
  ProgressBar bar;

  int problem_number;

  int nb_train_samples;
  int nb_test_samples;
  int nb_weak_learners;
  int nb_optimization_weak_learners;
  int nb_sampled_samples;

  Global();
  ~Global();

  void init_parser(ParamParser *parser);
  void read_parser(ParamParser *parser);
};

extern Global global;

#endif
