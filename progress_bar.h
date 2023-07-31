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

  This class displays a progress bar in text mode and computes a rough
  estimate of the remaining processing time.

*/

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <iostream>

using namespace std;

#include "misc.h"

class ProgressBar {
  const static int width = 80;
  bool _visible;
  scalar_t _value_max, _last_step;
  time_t _initial_time;
public:
  ProgressBar();
  void set_visible(bool visible);
  void init(ostream *out, scalar_t value_max);
  void refresh(ostream *out, scalar_t value);
  void finish(ostream *out);
};

#endif
