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

#include <iostream>
#include <stdio.h>

#include <libpng12/png.h>
#include "jpeg_misc.h"

#include "rgb_image.h"

void RGBImage::allocate() {
  _bit_plans = new unsigned char **[RGB_DEPTH];
  _bit_lines = new unsigned char *[RGB_DEPTH * _height];
  _bit_map = new unsigned char [_width * _height * RGB_DEPTH];
  for(int k = 0; k < RGB_DEPTH; k++) _bit_plans[k] = _bit_lines + k * _height;
  for(int k = 0; k < RGB_DEPTH * _height; k++) _bit_lines[k] = _bit_map + k * _width;
}

void RGBImage::deallocate() {
  delete[] _bit_plans;
  delete[] _bit_lines;
  delete[] _bit_map;
}

RGBImage::RGBImage() : _bit_plans(0), _bit_lines(0), _bit_map(0) { }

RGBImage::RGBImage(int width, int height) : _width(width), _height(height) {
  allocate();
  memset(_bit_map, 0, _width * _height * RGB_DEPTH * sizeof(unsigned char));
}

RGBImage::RGBImage(RGBImage *image, scalar_t scale) {
  _width = int(scale * image->_width);
  _height = int(scale * image->_height);

  allocate();

  for(int y = 0; y < _height; y++) {
    for(int x = 0; x < _width; x++) {

      const int delta = 10;
      int sr = 0, sg = 0, sb = 0, t = 0;
      int xo, yo;

      for(int yy = y * delta; yy < (y + 1) * delta; yy++) {
        for(int xx = x * delta; xx < (x + 1) * delta; xx++) {
          xo = (image->_width * xx)/(_width * delta);
          yo = (image->_height * yy)/(_height * delta);
          if(xo >= 0 && xo < image->_width && yo >= 0 && yo < image->_height) {
            sr += image->_bit_plans[RED][yo][xo];
            sg += image->_bit_plans[GREEN][yo][xo];
            sb += image->_bit_plans[BLUE][yo][xo];
            t++;
          }
        }
      }

      if(t > 0) {
        _bit_plans[RED][y][x] = sr / t;
        _bit_plans[GREEN][y][x] = sg / t;
        _bit_plans[BLUE][y][x] = sb / t;
      } else {
        _bit_plans[RED][y][x] = 0;
        _bit_plans[GREEN][y][x] = 0;
        _bit_plans[BLUE][y][x] = 0;
      }

    }
  }
}

RGBImage::~RGBImage() {
  deallocate();
}

void RGBImage::read_png(const char *name) {
  // This is the number of bytes the read_png routine will read to
  // decide if the file is a PNG or not. According to the png
  // documentation, it can be 1 to 8 bytes, 8 being the max and the
  // best.

  const int header_size = 8;

  png_byte header[header_size];
  png_bytep *row_pointers;

  deallocate();

  // open file
  FILE *fp = fopen(name, "rb");
  if (!fp) {
    cerr << "Unable to open file " << name << " for reading.\n";
    exit(1);
  }

  // read header
  fread(header, 1, header_size, fp);
  if (png_sig_cmp(header, 0, header_size)) {
    cerr << "File " << name << " does not look like PNG.\n";
    fclose(fp);
    exit(1);
  }

  // create png pointer
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png_ptr) {
    cerr << "png_create_read_struct failed\n";
    fclose(fp);
    exit(1);
  }

  // create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) 0, (png_infopp) 0);
    cerr << "png_create_info_struct failed\n";
    fclose(fp);
    exit(1);
  }

  // get image info
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, header_size);
  png_read_info(png_ptr, info_ptr);

  _width = info_ptr->width;
  _height = info_ptr->height;

  png_byte bit_depth, color_type, channels;
  color_type = info_ptr->color_type;
  bit_depth = info_ptr->bit_depth;
  channels = info_ptr->channels;

  if(bit_depth != 8) {
    cerr << "Can only read 8-bits PNG images." << endl;
    exit(1);
  }

  // allocate image pointer
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
  for (int y = 0; y < _height; y++)
    row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

  allocate();

  // read image
  png_read_image(png_ptr, row_pointers);

  // send image to red, green and blue buffers
  switch (color_type) {
  case PNG_COLOR_TYPE_GRAY:
    {
      unsigned char pixel = 0;
      for (int y = 0; y < _height; y++) for (int x = 0; x < _width; x++) {
        pixel = row_pointers[y][x];
        _bit_plans[RED][y][x] = pixel;
        _bit_plans[GREEN][y][x] = pixel;
        _bit_plans[BLUE][y][x] = pixel;
      }
    }
    break;

  case PNG_COLOR_TYPE_GRAY_ALPHA:
    cerr << "PNG type GRAY_ALPHA not supported.\n";
    exit(1);
    break;

  case PNG_COLOR_TYPE_PALETTE:
    cerr << "PNG type PALETTE not supported.\n";
    exit(1);
    break;

  case PNG_COLOR_TYPE_RGB:
    {
      if(channels != RGB_DEPTH) {
        cerr << "Unsupported number of channels for RGB type\n";
        break;
      }
      int k;
      for (int y = 0; y < _height; y++) {
        k = 0;
        for (int x = 0; x < _width; x++) {
          _bit_plans[RED][y][x] = row_pointers[y][k++];
          _bit_plans[GREEN][y][x] = row_pointers[y][k++];
          _bit_plans[BLUE][y][x] = row_pointers[y][k++];
        }
      }
    }
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA:
    cerr << "PNG type RGB_ALPHA not supported.\n";
    exit(1);
    break;

  default:
    cerr << "Unknown PNG type\n";
    exit(1);
  }

  // release memory
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);

  for (int y = 0; y < _height; y++) free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void RGBImage::write_png(const char *name) {
  png_bytep *row_pointers;

  // create file
  FILE *fp = fopen(name, "wb");

  if (!fp) {
    cerr << "Unable to create image '" << name << "'\n";
    exit(1);
  }

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  if (!png_ptr) {
    cerr << "png_create_write_struct failed\n";
    fclose(fp);
    exit(1);
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    cerr << "png_create_info_struct failed\n";
    fclose(fp);
    exit(1);
  }

  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr, _width, _height,
               8, 2, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  // allocate memory
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * _height);
  for (int y = 0; y < _height; y++)
    row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

  int k;
  for (int y = 0; y < _height; y++) {
    k = 0;
    for (int x = 0; x < _width; x++) {
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * RED)];
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * GREEN)];
      row_pointers[y][k++] = _bit_map[x + _width * (y + _height * BLUE)];
    }
  }

  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, 0);

  png_destroy_write_struct(&png_ptr, &info_ptr);

  // cleanup heap allocation
  for (int y = 0; y < _height; y++) free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void RGBImage::write_jpg(const char *filename, int quality) {
  struct jpeg_compress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *outfile;                /* target file */
  JSAMPARRAY buffer;            /* Output row buffer */

  jpeg_create_compress (&cinfo);

  if ((outfile = fopen (filename, "wb")) == 0) {
    fprintf (stderr, "Can't open %s\n", filename);
    exit(1);
  }

  cinfo.err = jpeg_std_error (&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp (jerr.setjmp_buffer)) {
    jpeg_destroy_compress (&cinfo);
    fclose (outfile);
    exit(1);
  }

  jpeg_stdio_dest (&cinfo, outfile);

  cinfo.image_width = _width;
  cinfo.image_height = _height;
  cinfo.input_components = RGB_DEPTH;

  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults (&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);
  jpeg_start_compress (&cinfo, TRUE);
  int y = 0;
  buffer =
    (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE,
                                _width * RGB_DEPTH, 1);
  while (int(cinfo.next_scanline) < _height) {
    for(int d = 0; d < RGB_DEPTH; d++)
      for(int x = 0; x < _width; x++)
        buffer[0][x * RGB_DEPTH + d] =
          (JSAMPLE) ((_bit_map[x + _width * (y + _height * d)] * (MAXJSAMPLE + 1)) / 255);
    jpeg_write_scanlines (&cinfo, buffer, 1);
    y++;
  }

  jpeg_finish_compress (&cinfo);
  fclose (outfile);

  jpeg_destroy_compress (&cinfo);
}

void RGBImage::read_jpg(const char *filename) {
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *infile;
  JSAMPARRAY buffer;

  deallocate();

  if ((infile = fopen (filename, "rb")) == 0) {
    fprintf (stderr, "can't open %s\n", filename);
    return;
  }

  cinfo.err = jpeg_std_error (&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp (jerr.setjmp_buffer)) {
    jpeg_destroy_decompress (&cinfo);
    fclose (infile);
    delete[] _bit_map;
    _width = 0;
    _height = 0;
    _bit_map = 0;
    return;
  }

  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, infile);
  jpeg_read_header (&cinfo, TRUE);
  jpeg_start_decompress (&cinfo);

  _width = cinfo.output_width;
  _height = cinfo.output_height;
  int depth = cinfo.output_components;

  allocate();

  buffer =
    (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE,
                                _width * depth, 1);

  int y = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines (&cinfo, buffer, 1);
    if(depth == 1) {
      for(int d = 0; d < RGB_DEPTH; d++)
        for(int x = 0; x < _width; x++)
          _bit_plans[d][y][x] =
            (unsigned char) ((buffer[0][x * depth] * 255) / (MAXJSAMPLE + 1));
    } else {
      for(int d = 0; d < depth; d++)
        for(int x = 0; x < _width; x++)
          _bit_plans[d][y][x] =
            (unsigned char) ((buffer[0][x * depth + d] * 255) / (MAXJSAMPLE + 1));
    }
    y++;
  }

  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);

  fclose (infile);
}

void RGBImage::draw_line(int thickness,
                         unsigned char r, unsigned char g, unsigned char b,
                         scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1) {
  int l = 0;
  int dx, dy, h, v;
  int ix0 = int(x0 + 0.5), iy0 = int(y0 + 0.5), ix1 = int(x1 + 0.5), iy1 = int(y1 + 0.5);

  if(ix0 < ix1) { dx = 1; h = ix1 - ix0; } else { dx = -1; h = ix0 - ix1; }
  if(iy0 < iy1) { dy = 1; v = iy1 - iy0; } else { dy = -1; v = iy0 - iy1; }

  int x = ix0, y = iy0;

  if(h > v) {
    for(int i = 0; i < h + 1; i++) {
      for(int ex = - thickness / 2 - 1; ex < (thickness + 1) / 2 + 1; ex++) {
        for(int ey = - thickness / 2 - 1; ey < (thickness + 1) / 2 + 1; ey++) {
          if(ex * ex + ey * ey <= thickness * thickness / 4) {
            int xx = x + ex, yy = y + ey;
            if(xx >= 0 && xx < _width && yy >= 0 && yy < _height)
              set_pixel(xx, yy, r, g, b);
          }
        }
      }

      x += dx; l += v;
      if(l > 0) { y += dy; l -= h; }
    }

  } else {

    for(int i = 0; i < v + 1; i++) {
      for(int ex = - thickness / 2 - 1; ex < (thickness + 1) / 2 + 1; ex++) {
        for(int ey = - thickness / 2 - 1; ey < (thickness + 1) / 2 + 1; ey++) {
          if(ex * ex + ey * ey <= thickness * thickness / 4) {
            int xx = x + ex, yy = y + ey;
            if(xx >= 0 && xx < _width && yy >= 0 && yy < _height)
              set_pixel(xx, yy, r, g, b);
          }
        }
      }

      y += dy; l -= h;
      if(l < 0) { x += dx; l += v; }
    }

  }

}

void RGBImage::draw_ellipse(int thickness,
                            unsigned char r, unsigned char g, unsigned char b,
                            scalar_t xc, scalar_t yc, scalar_t radius_1, scalar_t radius_2, scalar_t tilt) {
  scalar_t ux1 =   cos(tilt) * radius_1, uy1 =   sin(tilt) * radius_1;
  scalar_t ux2 = - sin(tilt) * radius_2, uy2 =   cos(tilt) * radius_2;

  const int nb_points_to_draw = 80;
  scalar_t x, y, px = 0, py = 0;

  for(int i = 0; i <= nb_points_to_draw; i++) {
    scalar_t alpha = (M_PI * 2 * scalar_t(i)) / scalar_t(nb_points_to_draw);

    x = xc + cos(alpha) * ux1 + sin(alpha) * ux2;
    y = yc + cos(alpha) * uy1 + sin(alpha) * uy2;

    if(i > 0) {
      draw_line(thickness, r, g, b, px, py, x, y);
    }

    px = x; py = y;
  }
}
