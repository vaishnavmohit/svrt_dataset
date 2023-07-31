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

#include <time.h>
#include "progress_bar.h"

ProgressBar::ProgressBar()  : _visible(false), _value_max(-1) { }

void ProgressBar::set_visible(bool visible) {
  _visible = visible;
}

void ProgressBar::init(ostream *out, scalar_t value_max) {
  _value_max = value_max;
  _last_step = -1;
  time(&_initial_time);
  refresh(out, 0);
}

void ProgressBar::refresh(ostream *out, scalar_t value) {
  if(_visible && _value_max > 0) {
    int step = int((value * 40) / _value_max);

    if(step > _last_step) {
      char buffer[width + 1], date_buffer[buffer_size];
      int i, j;
      j = sprintf(buffer, "Timer: ");

      for(i = 0; i < step; i++) buffer[j + i] = 'X';
      for(; i < 40; i++) buffer[j + i] = (i%4 == 0) ? '+' : '-';
      j += i;

      time_t current_time; time(&current_time);
      int rt = int(((current_time - _initial_time)/scalar_t(value)) * scalar_t(_value_max - value));

      if(rt > 0) {
        if(rt > 3600 * 24) {
          time_t current;
          time(&current);
          current += rt;
          strftime(date_buffer, buffer_size, "%a %b %e %H:%M", localtime(&current));
          j += snprintf(buffer + j, width - j - 1, " (end ~ %s)", date_buffer);
        } else {
          int hours = rt/3600, min = (rt%3600)/60, sec = rt%60;
          if(hours > 0)
            j += snprintf(buffer + j, width - j - 1, " (~%dh%dmin left)", hours, min);
          else if(min > 0)
            j += snprintf(buffer + j, width - j - 1, " (~%dmin%ds left)", min, sec);
          else
            j += snprintf(buffer + j, width - j - 1, " (~%ds left)", sec);
        }
      }

      for(; j < width; j++) buffer[j] = ' ';
      buffer[j] = '\0';
      (*out) << buffer << "\r";
      out->flush();
      _last_step = step;
    }
  }
}

void ProgressBar::finish(ostream *out) {
  if(_visible) {
    char buffer[width + 1];
    int j;
    time_t current_time; time(&current_time);
    int rt = int(current_time - _initial_time);
    int min = rt/60, sec = rt%60;
    j = sprintf(buffer, "Timer: Total %dmin%ds", min, sec);
    for(; j < width; j++) buffer[j] = ' ';
    buffer[j] = '\0';
    (*out) << buffer << endl;
    out->flush();
  }
}
