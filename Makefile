
#  svrt is the ``Synthetic Visual Reasoning Test'', an image generator
#  for evaluating classification performance of machine learning
#  systems, humans and primates.
#
#  Copyright (c) 2009 Idiap Research Institute, http://www.idiap.ch/
#  Written by Francois Fleuret <francois.fleuret@idiap.ch>
#
#  This file is part of svrt.
#
#  svrt is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  svrt is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
#  License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with svrt.  If not, see <http://www.gnu.org/licenses/>.

LDFLAGS=-lm -ljpeg -lpng12

ifeq ($(DEBUG),yes)
  OPTIMIZE_FLAG = -ggdb3 -DDEBUG -fno-omit-frame-pointer
else
  OPTIMIZE_FLAG = -ggdb3 -O3
endif

ifeq ($(PROFILE),yes)
  PROFILE_FLAG = -pg
endif

CXXFLAGS = -Wall $(OPTIMIZE_FLAG) $(PROFILE_FLAG) $(CXXGLPK)

all: vision_test TAGS

TAGS: *.cc *.h
	etags --members -l c++ *.cc *.h

vision_test: misc.o \
	global.o param_parser.o progress_bar.o \
	rgb_image.o jpeg_misc.o \
	fusion_sort.o \
	discrete_density.o \
	tools.o \
	vignette.o \
	shape.o \
	vignette_generator.o \
	vision_problem_tools.o \
	vision_problem_1.o \
	vision_problem_2.o \
	vision_problem_3.o \
	vision_problem_4.o \
	vision_problem_5.o \
	vision_problem_6.o \
	vision_problem_7.o \
	vision_problem_8.o \
	vision_problem_9.o \
	vision_problem_10.o \
	vision_problem_11.o \
	vision_problem_12.o \
	vision_problem_13.o \
	vision_problem_14.o \
	vision_problem_15.o \
	vision_problem_16.o \
	vision_problem_17.o \
	vision_problem_18.o \
	vision_problem_19.o \
	vision_problem_20.o \
	vision_problem_21.o \
	vision_problem_22.o \
	vision_problem_23.o \
	vision_problem_25.o \
	classifier_reader.o \
        classifier.o naive_bayesian_classifier.o \
	stump.o boosted_classifier.o \
	error_rates.o \
	vision_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Makefile.depend: *.h *.cc Makefile
	$(CC) -M *.cc > Makefile.depend

clean:
	\rm -f vision_test *.o Makefile.depend TAGS

-include Makefile.depend
