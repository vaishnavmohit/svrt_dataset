This software should compile on any standard GNU/Linux machine, and
UNIX machine in general. It only requires the jpeg and png library.

To test it on a list of problems, just run the script doit.sh with the
problem numbers as arguments. For instance:

  ./doit.sh 1 2 3

It will compile the source and run the executable on the specified
problems to generate sample images and test a Boosted classifier. If
no argument is given, it will run on all the problems by default.

--
Francois Fleuret, Sep 2010.
