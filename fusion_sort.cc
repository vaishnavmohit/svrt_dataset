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

#include "fusion_sort.h"
#include <string.h>

inline void indexed_fusion(int na, int *ia, int nb, int *ib, int *ic, scalar_t *values) {
  int *ma = ia + na, *mb = ib + nb;
  while(ia < ma && ib < mb)
    if(values[*ia] <= values[*ib]) *(ic++) = *(ia++);
    else                           *(ic++) = *(ib++);
  while(ia < ma) *(ic++) = *(ia++);
  while(ib < mb) *(ic++) = *(ib++);
}

void indexed_fusion_sort(int n, int *from, int *result, scalar_t *values) {
  ASSERT(n > 0);
  if(n == 1) result[0] = from[0];
  else {
    int k = n/2;
    indexed_fusion_sort(k, from, result, values);
    indexed_fusion_sort(n - k, from + k, result + k, values);
    memcpy((void *) from, (void *) result, n * sizeof(int));
    indexed_fusion(k, from, n - k, from + k, result, values);
  }
}

// Sorting in decreasing order

inline void indexed_fusion_dec(int na, int *ia,
                           int nb, int *ib,
                           int *ic, scalar_t *values) {
  int *ma = ia + na, *mb = ib + nb;
  while(ia < ma && ib < mb)
    if(values[*ia] > values[*ib]) *(ic++) = *(ia++);
    else                          *(ic++) = *(ib++);
  while(ia < ma) *(ic++) = *(ia++);
  while(ib < mb) *(ic++) = *(ib++);
}

void indexed_fusion_dec_sort(int n, int *from, int *result, scalar_t *values) {
  ASSERT(n > 0);
  if(n == 1) result[0] = from[0];
  else {
    int k = n/2;
    indexed_fusion_dec_sort(k, from, result, values);
    indexed_fusion_dec_sort(n - k, from + k, result + k, values);
    memcpy((void *) from, (void *) result, n * sizeof(int));
    indexed_fusion_dec(k, from, n - k, from + k, result, values);
  }
}

void fusion_two_cells(int n1, scalar_t *cell1, int n2, scalar_t *cell2, scalar_t *result) {
  scalar_t *max1 = cell1 + n1, *max2 = cell2 + n2;
  while(cell1 < max1 && cell2 < max2) {
    if(*(cell1) <= *(cell2)) *(result++) = *(cell1++);
    else                     *(result++) = *(cell2++);
  }
  while(cell1 < max1) *(result++) = *(cell1++);
  while(cell2 < max2) *(result++) = *(cell2++);
}

void fusion_sort(int n, scalar_t *from, scalar_t *result) {
  if(n > 1) {
    fusion_sort(n/2, from, result);
    fusion_sort(n - n/2, from + n/2, result + n/2);
    memcpy(from, result, sizeof(scalar_t) * n);
    fusion_two_cells(n/2, from, n - n/2, from + n/2, result);
  } else result[0] = from[0];
}
