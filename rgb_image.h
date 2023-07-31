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

  A simple image class to either load color images, or produce
  materials.

 */

#ifndef RGB_IMAGE_H
#define RGB_IMAGE_H

#include "misc.h"

class RGBImage {
protected:
  int _width, _height;
  unsigned char ***_bit_plans, **_bit_lines, *_bit_map;
  static const int RED = 0;
  static const int GREEN = 1;
  static const int BLUE = 2;
  static const int RGB_DEPTH = 3;

  void allocate();
  void deallocate();

public:

  RGBImage();
  RGBImage(int width, int height);
  RGBImage(RGBImage *image, scalar_t scale);
  virtual ~RGBImage();

  inline int width() const { return _width; }
  inline int height() const { return _height; }

  inline void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height);
    _bit_plans[RED][y][x] = r;
    _bit_plans[GREEN][y][x] = g;
    _bit_plans[BLUE][y][x] = b;
  }

  inline unsigned char pixel(int x, int y, int d) {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height && d >= 0 && d < RGB_DEPTH);
    return _bit_plans[d][y][x];
  }

  virtual void read_png(const char *filename);
  virtual void write_png(const char *filename);

  virtual void read_jpg(const char *filename);
  virtual void write_jpg(const char *filename, int quality);

  virtual void draw_line(int thickness,
                         unsigned char r, unsigned char g, unsigned char b,
                         scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1);

  virtual void draw_ellipse(int thickness,
                            unsigned char r, unsigned char g, unsigned char b,
                            scalar_t xc, scalar_t yc, scalar_t radius_1, scalar_t radius_2, scalar_t tilt);
};

#endif
