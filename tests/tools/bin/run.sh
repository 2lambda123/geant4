#!/bin/sh -f
#
#  Usage :
#     UNIX> cd <g4sttdir>/bin
#     UNIX> chmod u+x run.sh
#     UNIX> ./run.sh all
#     UNIX> ./run.sh test01
#
#  When env variables are correctly setted, this script could be 
# executed from everywhere. (That a bit anbitious, anywhere is already
#                            quite risky).
#

# Some checks :
# set -x
if [ -z "${G4INSTALL}" ] ; then
  echo "G4INSTALL not set. Execute first setup file !"
  exit
fi

if [ -z "${G4WORKDIR}" ] ; then
  echo "G4WORKDIR not set. Execute first setup file !"
  exit
fi

if [ -z "${G4SYSTEM}" ] ; then
  echo "G4SYSTEM not set. Execute first setup file !"
  exit
fi

if [ -z "${G4STTDIR}" ] ; then
  echo "G4STTDIR not set. Execute first setup file !"
  exit
fi

##if [ -z "${LD_LIBRARY_PATH}" ] ; then
#    LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM
#    export LD_LIBARY_PATH
#    echo $LD_LIBRARY_PATH
##fi


# Make $G4WORKDIR/stt directory :
dir=$G4WORKDIR/stt
if [ ! -d $dir ] ; then
 echo "$dir does not exist."
 echo "This is now an error.  Create a directory or a symbolic link"
 echo "  to a directory by hand or in a calling script."
fi

# Make $G4WORKDIR/stt/$G4SYSTEM directory :
dir=$G4WORKDIR/stt/$G4SYSTEM
if [ ! -d $dir ] ; then
  mkdir $dir
  echo "$dir created."
fi

# Check setup

# I was wondering where these apparently useless directories were coming from.
#-- 
#-- if [ ! -d $G4INSTALL/tests/tools/lib/$G4SYSTEM ]; then
#--   mkdir $G4INSTALL/tests/tools/lib/$G4SYSTEM
#-- fi
#-- if [ ! -f $G4INSTALL/tests/tools/lib/$G4SYSTEM/libG4TestTool.a ]; then
#--   cd $G4INSTALL/tests/tools/lib; gmake
#-- fi
#-- if [ ! -d $G4INSTALL/tests/tools/bin/$G4SYSTEM ]; then
#--   mkdir $G4INSTALL/tests/tools/bin/$G4SYSTEM
#-- fi
#-- ##if [ ! -f $G4INSTALL/tests/tools/bin/$G4SYSTEM/diffhisto ]; then
#-- ##  cd $G4INSTALL/tests/tools/bin; gmake
#-- ##fi

if [ $1 = "all" ] ; then 

  nice $G4STTDIR/bin/run.sh test01
  nice $G4STTDIR/bin/run.sh test02
  nice $G4STTDIR/bin/run.sh test02.hadron
  nice $G4STTDIR/bin/run.sh test05
  nice $G4STTDIR/bin/run.sh test06
  nice $G4STTDIR/bin/run.sh test07
  nice $G4STTDIR/bin/run.sh test09
  nice $G4STTDIR/bin/run.sh test10
#
# NDL # Following tag 'neu-V03-02-02'
# On Thu, 26 Jul 2001, Hans-Peter Wellisch wrote:
# Please use G4NDL3.4 for test11, and G4NDL0.2 for the other tests.
# set default to G4NDL0.2 in setup scripts

  NeutronHPCrossSections=/afs/cern.ch/sw/geant4/dev/data/G4NDL3.4;export NeutronHPCrossSections
  echo "STT:NeutronHPCrossSections G4NDL3.4";
  nice $G4STTDIR/bin/run.sh test11

  NeutronHPCrossSections=/afs/cern.ch/sw/geant4/dev/data/G4NDL0.2;export NeutronHPCrossSections
  echo "STT:NeutronHPCrossSections G4NDL0.2";
  nice $G4STTDIR/bin/run.sh test12

  nice $G4STTDIR/bin/run.sh test13
  nice $G4STTDIR/bin/run.sh test14
  nice $G4STTDIR/bin/run.sh test15
  nice $G4STTDIR/bin/run.sh test16
  nice $G4STTDIR/bin/run.sh test17
  nice $G4STTDIR/bin/run.sh test18
  nice $G4STTDIR/bin/run.sh test20

  nice $G4STTDIR/bin/run.sh test101
  nice $G4STTDIR/bin/run.sh test102
  nice $G4STTDIR/bin/run.sh test103
  nice $G4STTDIR/bin/run.sh test104
  nice $G4STTDIR/bin/run.sh test104.EMtest
  nice $G4STTDIR/bin/run.sh test105
  nice $G4STTDIR/bin/run.sh test106
  nice $G4STTDIR/bin/run.sh test501
  nice $G4STTDIR/bin/run.sh test502
  nice $G4STTDIR/bin/run.sh test503
  nice $G4STTDIR/bin/run.sh test504
  nice $G4STTDIR/bin/run.sh test505
  nice $G4STTDIR/bin/run.sh test508
  if [ $G4USE_HEPODBMS ] ; then
    nice $G4STTDIR/bin/run.sh test401
    nice $G4STTDIR/bin/run.sh test402
  fi
  nice $G4STTDIR/bin/run.sh test601
  nice $G4STTDIR/bin/run.sh test602

#  nice $G4STTDIR/bin/run.sh test701
#  nice $G4STTDIR/bin/run.sh test702
#  nice $G4STTDIR/bin/run.sh test703
else

  if [ $1 = "test201" ] ; then 


  XENVIRONMENT=$G4INSTALL/tests/$1/$1.xrm;export XENVIRONMENT
    if [ $2 = "Xm" ] ; then 
      cd $G4INSTALL/tests/$1/basic
    else
      cd $G4INSTALL/tests/$1
    fi
    $G4WORKDIR/bin/$G4SYSTEM/$1 $2
    exit

  else

    # Other tests :
    shortname=`basename $1 .hadron`
    shortname=`basename $shortname .EMtest`


    timestamp=`date +%Y%m%d-%H%M%S`
    g4option=`echo $G4WORKDIR | sed "s!^.*${G4SYSTEM}/!!"`
    g4identify=`echo $G4WORKDIR | sed "s!^.*geant4/stt!stt!"`
    disabled=${G4INSTALL}/tests/disable.${shortname}.${G4SYSTEM}.${g4option}
    if [ -f $disabled ]; then
      echo "Disabled $1 in $g4identify $timestamp  $G4LARGE_N "
      echo "Disabled $1 in $G4WORKDIR $timestamp  $G4LARGE_N " > $dir/$1$dot_G4LARGE_N.err
      rm -f $dir/$1$dot_G4LARGE_N.out # or diff will run below
    elif [ ! -f $G4WORKDIR/bin/$G4SYSTEM/$shortname ]; then
      echo "Missing $1 in $g4identify $timestamp  $G4LARGE_N "
    else

      if [ $G4LARGE_N ]; then
        dot_G4LARGE_N=.$G4LARGE_N
      fi

      cd $G4INSTALL/tests/$shortname

      echo "Starting $1 in $g4identify $timestamp  $G4LARGE_N "

      if [ $1 = test02.hadron -o $1 = test11 -o $1 = test12 -o $1 = test13 \
        -o $1 = test15 -o $1 = test16 ]
      then
        if [ $1 = test11 ]; then
          NeutronHPCrossSections=/afs/cern.ch/sw/geant4/dev/data/G4NDL3.4
          export NeutronHPCrossSections
          echo "STT:hadrons! NeutronHPCrossSections G4NDL3.4";
        fi
        rm -f $dir/$1.exerciser$dot_G4LARGE_N.in
        $G4WORKDIR/bin/$G4SYSTEM/$shortname.hadronic.exerciser $G4LARGE_N \
        > $dir/$1.exerciser$dot_G4LARGE_N.in
        rm -f $dir/$1$dot_G4LARGE_N.out
        rm -f $dir/$1$dot_G4LARGE_N.err
        time $G4WORKDIR/bin/$G4SYSTEM/$shortname \
        $dir/$1.exerciser$dot_G4LARGE_N.in \
        > $dir/$1$dot_G4LARGE_N.out 2> $dir/$1$dot_G4LARGE_N.err
        if [ $1 = test11 ]; then
          NeutronHPCrossSections=/afs/cern.ch/sw/geant4/dev/data/G4NDL0.2
          export NeutronHPCrossSections
          echo "STT:hadrons! NeutronHPCrossSections G4NDL0.2";
        fi

      elif [ $1 = test601 -o $1 = test602 ] 
      then
        if [ -z "$G4LARGE_N" -o \
          \( -n "$G4LARGE_N" -a \
             -f $G4INSTALL/tests/$shortname/$1$dot_G4LARGE_N.in \) ]; then
          rm -f $dir/$1$dot_G4LARGE_N.out
          rm -f $dir/$1$dot_G4LARGE_N.err
          time $G4WORKDIR/bin/$G4SYSTEM/$shortname \
            $G4INSTALL/examples/extended/g3tog4/data/testmodel.dat \
            $G4INSTALL/tests/$shortname/$1$dot_G4LARGE_N.in \
          > $dir/$1$dot_G4LARGE_N.out 2> $dir/$1$dot_G4LARGE_N.err
        else
          echo "tests/$shortname/$1$dot_G4LARGE_N.in does not exist."
        fi

      else

        if [ -z "$G4LARGE_N" -o \
          \( -n "$G4LARGE_N" -a \
             -f $G4INSTALL/tests/$shortname/$1$dot_G4LARGE_N.in \) ]; then
          rm -f $dir/$1$dot_G4LARGE_N.out
          rm -f $dir/$1$dot_G4LARGE_N.err
          time $G4WORKDIR/bin/$G4SYSTEM/$shortname \
          $G4INSTALL/tests/$shortname/$1$dot_G4LARGE_N.in \
          > $dir/$1$dot_G4LARGE_N.out 2> $dir/$1$dot_G4LARGE_N.err
        else
          echo "tests/$shortname/$1$dot_G4LARGE_N.in does not exist."
        fi
      fi

      timestamp=`date +%Y%m%d-%H%M%S`
      echo "Finished $1 in $g4identify $timestamp"

      if [ -f $1$dot_G4LARGE_N.evalsh ]; then
        rm -f $dir/$1$dot_G4LARGE_N.eval
        $1$dot_G4LARGE_N.evalsh $dir/$1$dot_G4LARGE_N.out \
        $1$dot_G4LARGE_N.out > $dir/$1$dot_G4LARGE_N.eval 2>&1
        if [ $? != 0 ]; then
          if [ ! -f $dir/$1$dot_G4LARGE_N.badflag ]; then
            touch $dir/$1$dot_G4LARGE_N.badflag
          fi
          echo $1$dot_G4LARGE_N run failure \
          >> $dir/$1$dot_G4LARGE_N.badflag 2>&1
        fi
      else
        rm -f $dir/$1$dot_G4LARGE_N.diff
        rm -f $dir/$1$dot_G4LARGE_N.diff_err
        if [ -f $dir/$1$dot_G4LARGE_N.out ]; then
          diff -w $dir/$1$dot_G4LARGE_N.out $1$dot_G4LARGE_N.out \
          > $dir/$1$dot_G4LARGE_N.diff 2> $dir/$1$dot_G4LARGE_N.diff_err
          #cat $dir/$1$dot_G4LARGE_N.diff
        fi
      fi

    fi

  fi

fi
