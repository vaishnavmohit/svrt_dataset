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

/*

  A class to parse the arguments from the command line.

 */

#ifndef PARAM_PARSER_H
#define PARAM_PARSER_H

#include <iostream>
#include "misc.h"

using namespace std;

class ParamParser {
  int _nb_max, _nb;
  char **_names, **_values;
  bool *_changed;
public:
  ParamParser();
  ~ParamParser();
  void add_association(const char *variable_name, const char *variable_value, bool change);
  char *get_association(const char *variable_name);
  int get_association_int(const char *variable_name);
  long int get_association_long_int(const char *variable_name);
  scalar_t get_association_scalar(const char *variable_name);
  bool get_association_bool(const char *variable_name);

  void parse_options(int argc, char **argv, bool allow_undefined, int *new_argc, char **new_argv);
  void print_all(ostream *os);
};

#endif
