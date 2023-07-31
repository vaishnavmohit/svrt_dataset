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

// All this is clearly non-optimal, loaded with news and deletes and
// should be rewritten.

#include <string.h>

#include "param_parser.h"

ParamParser::ParamParser() : _nb_max(10),
                             _nb(0),
                             _names(new char *[_nb_max]),
                             _values(new char *[_nb_max]),
                             _changed(new bool[_nb_max]) { }

ParamParser::~ParamParser() {
  for(int k = 0; k < _nb; k++) {
    delete[] _names[k];
    delete[] _values[k];
  }
  delete[] _names;
  delete[] _values;
  delete[] _changed;
}

void ParamParser::add_association(const char *variable_name, const char *variable_value, bool change) {
  int n;

  for(n = 0; n < _nb && strcmp(variable_name, _names[n]) != 0; n++);

  if(n < _nb) {
    delete[] _values[n];
    _values[n] = new char[strlen(variable_value) + 1];
    strcpy(_values[n], variable_value);
    _changed[n] = change;
  } else {
    int nm;
    nm = _nb_max; grow(&nm, _nb, &_names, 2);
    nm = _nb_max; grow(&nm, _nb, &_values, 2);
    grow(&_nb_max, _nb, &_changed, 2);

    _names[_nb] = new char[strlen(variable_name) + 1];
    strcpy(_names[_nb], variable_name);
    _values[_nb] = new char[strlen(variable_value) + 1];
    strcpy(_values[_nb], variable_value);
    _changed[_nb] = change;
    _nb++;
  }
}

char *ParamParser::get_association(const char *variable_name) {
  int n;
  for(n = 0; n < _nb && strcmp(variable_name, _names[n]) != 0; n++);
  if(n < _nb) return _values[n];
  else        {
    cerr << "Unknown parameter \"" << variable_name << "\", existing ones are" << endl;
    for(int n = 0; n < _nb; n++)
      cerr << "   \"" << _names[n] << "\"" << endl;
    exit(1);
  }
}

int ParamParser::get_association_int(const char *variable_name) {
  char *u = get_association(variable_name);
  char *s = u;
  while(*s)
    if((*s < '0' || *s > '9') && *s != '-') {
      cerr << "Non-numerical value for " << variable_name << " (" << u << ")" << endl;
      exit(1);
    } else s++;
  return atoi(u);
}

long int ParamParser::get_association_long_int(const char *variable_name) {
  char *u = get_association(variable_name);
  char *s = u;
  while(*s)
    if((*s < '0' || *s > '9') && *s != '-') {
      cerr << "Non-numerical value for " << variable_name << " (" << u << ")" << endl;
      exit(1);
    } else s++;
  return atol(u);
}

scalar_t ParamParser::get_association_scalar(const char *variable_name) {
  char *u = get_association(variable_name);
  char *s = u;
  while(*s)
    if((*s < '0' || *s > '9') && *s != '.' && *s != 'e' && *s != '-') {
      cerr << "Non-numerical value for " << variable_name << " (" << u << ")" << endl;
      exit(1);
    } else s++;
  return atof(u);
}

bool ParamParser::get_association_bool(const char *variable_name) {
  char *value = get_association(variable_name);
  if(strcasecmp(value, "") == 0 || strcasecmp(value, "y") == 0 || strcasecmp(value, "yes") == 0) return true;
  if(strcasecmp(value, "n") == 0 || strcasecmp(value, "no") == 0) return false;
  cerr << "Expects nothing (for yes), or y[es] or n[o] for a boolean argument and got '" << value << "'" << endl;
  exit(1);
}

void ParamParser::parse_options(int argc, char **argv,
                                bool allow_undefined,
                                int *new_argc, char **new_argv) {

  int i = 1;

  if(new_argc && new_argv)
    new_argv[(*new_argc)++] = argv[0];

  while(i < argc) {
    if(strncmp(argv[i], "--", 2) == 0) {
      // This is so 70s! I luuuuv it!
      char variable_name[buffer_size] = "", variable_value[buffer_size] = "";
      char *o = argv[i] + 2, *s = variable_name, *u = variable_value;
      while(*o && *o != '=') *s++ = *o++;
      *s = '\0';
      if(*o) { o++; while(*o) *u++ = *o++; }
      *u = '\0';
      if(!allow_undefined) get_association(variable_name);
      add_association(variable_name, variable_value, true);
    } else {
      if(new_argc && new_argv)
        new_argv[(*new_argc)++] = argv[i];
      else {
        cerr << "Can not parse " << argv[i] << endl;
        exit(1);
      }
    }
    i++;
  }
}

void ParamParser::print_all(ostream *os) {
  for(int n = 0; n < _nb; n++) {
    (*os) << (_changed[n] ? " * " : "   ") << "\"" << _names[n] << "\" \"" << _values[n] << "\"" << endl;
  }
}

