#!/bin/sh
#
# A simple script to run all the tests in this directory and check
# their results against the expected (previous) results
#
# $Id: run.sh,v 1.5 2003-10-01 14:52:20 gcosmo Exp $
# $Name: not supported by cvs2svn $
#
# Created:
#   12 May 99 - J. Apostolakis: starting from P.Kent's test.sh
#                               changed output to STDout only        
# Modified:
#   21 May 99 - J. Apostolakis: check the results
#   28 Feb 00 - G. Cosmo: changed script to use /bin/sh shell.
#                         Fixed $G4TARGET and invocation of gmake.

echo "Running on `hostname`, which is a `uname -a` machine" 
host=`hostname`

for i in CurveTest.cc G4*.cc
do
  target=`basename $i .cc`
  echo  "Compiling $target ... "
  gmake G4TARGET=$target
  echo -n "Executing $target .."
  $G4WORKDIR/bin/$G4SYSTEM/$target > $target.newout-$host
  echo  ".. difference from expected output: "
  diff $target.out $target.newout-$host
  echo  " "
done

exit

