#
# This file is sourced by setup.sh script.
# 
####################################################
####################################################
### Specific STT member machines configurations  ###
####################################################
####################################################
#
# Specfy CLHEP 1.5 as new, run persistance tests 401,402
# Use /tmp for G4TMP on dxplus01,02,03,04,
# Use /tmp for G4TMP on all dxplus, retain dev1/dev2/prod identity
#

REF=undefined
if [ `pwd | grep /stt/dev1/` ]; then
export  REF=dev1
fi
if [ `pwd | grep /stt/dev2/` ]; then
export  REF=dev2
fi
if [ `pwd | grep /stt/prod/` ]; then
export  REF=prod
fi

export CLHEP_VERSION=pro

if [ $G4DEBUG ]; then
export DEBOPT=debug
else
export DEBOPT=optim
fi

# General G4 build flags :
export G4UI_BUILD_TERMINAL_SESSION=1
export G4UI_BUILD_GAG_SESSION=1
export G4VIS_BUILD_DAWN_DRIVER=1
export G4VIS_BUILD_DAWNFILE_DRIVER=1
export G4VIS_BUILD_VRML_DRIVER=1
export G4VIS_BUILD_VRMLFILE_DRIVER=1
export G4VIS_BUILD_RAYTRACER_DRIVER=1
export G4VIS_BUILD_ASCIITREE_DRIVER=1

UNAMEN=`uname -n `
echo UNAMEN $UNAMEN

if [ `uname -n | grep sunasd1` ]; then
  export G4USE_OSPACE=1
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=SUN-CC
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  #######export G4VIS_BUILD_OPENGLX_DRIVER=1
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi


if [ `uname -n | grep refsol7` ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  if [ $G4STTNONISO ]; then
    echo "refsol7 only supports the ISO compiler"
    export G4SYSTEM=SUN-CC
    export DEBOPT=${DEBOPT}_NONISO
    export G4USE_OSPACE=1
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC/$CLHEP_VERSION
    # Persistency...
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
  else
# G4SYSTEM was changed!
    export G4SYSTEM=SUN-CC
    export DEBOPT=${DEBOPT}_ISO
    unset G4USE_OSPACE
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    # No Persistency tonight ...
    unset G4USE_HEPODBMS
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC/$CLHEP_VERSION
  fi
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  export G4VIS_BUILD_OPENGLX_DRIVER=1
  export OGLHOME=/usr/local
  export OGLFLAGS="-I$OGLHOME/include"
  export OGLLIBS="-L$OGLHOME/lib -lMesaGLU -lMesaGL"
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi

# BLOCKed!
if [ `uname -n | grep sundevAAA` ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  if [ $G4STTNONISO ]; then
    echo "refsol7 only supports the ISO compiler"
    export G4SYSTEM=SUN-CC
    export DEBOPT=${DEBOPT}_NONISO
    export G4USE_OSPACE=1
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC/$CLHEP_VERSION
    # Persistency...
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
  else
    export G4SYSTEM=SUN-CC5
    export DEBOPT=${DEBOPT}_ISO
    unset G4USE_OSPACE
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    # No Persistency tonight ...
    unset G4USE_HEPODBMS
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC5/$CLHEP_VERSION
  fi
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  export G4VIS_BUILD_OPENGLX_DRIVER=1
  export OGLHOME=/usr/local
  export OGLFLAGS="-I$OGLHOME/include"
  export OGLLIBS="-L$OGLHOME/lib -lMesaGLU -lMesaGL"
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi



if [ `uname -n | grep suncmsb` ]; then
  export G4USE_OSPACE=1
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=SUN-CC
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
#
  export CLHEP_BASE_DIR=/afs/cern.ch/user/s/stesting/work/clhep
  export CLHEP_LIB=CLHEP-CC
  export RWBASE=/afs/cern.ch/user/s/stesting/work/rogue
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  #######export G4VIS_BUILD_OPENGLX_DRIVER=1
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ `uname -n | grep sungeant` ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  if [ $G4STTNONISO ]; then
    export G4SYSTEM=SUN-CC4
    export DEBOPT=${DEBOPT}_NONISO
    export G4USE_OSPACE=1
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
###TEMPORARY!
    unset G4USE_HEPODBMS
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC4/$CLHEP_VERSION
    # Persistency...
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
  else
    export G4SYSTEM=SUN-CC
    export DEBOPT=${DEBOPT}_ISO
    unset G4USE_OSPACE
    export PATH=`echo $PATH | sed s/SUNWspro/SUNWspro50/`
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    # No Persistency...
    unset G4USE_HEPODBMS
    # Persistency...
#   if [ X$G4USE_HEPODBMS = Xaaa ]; then
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC/$CLHEP_VERSION
  fi
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  export G4VIS_BUILD_OPENGLX_DRIVER=1
  export OGLHOME=/usr/local
  export OGLFLAGS="-I$OGLHOME/include"
  export OGLLIBS="-L$OGLHOME/lib -lMesaGLU -lMesaGL"
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi

# refsol8 -> sundev008
if [ `uname -n | grep sundev008` ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  if [ $G4STTNONISO ]; then
    export G4SYSTEM=SUN-CC4
    export DEBOPT=${DEBOPT}_NONISO
    export G4USE_OSPACE=1
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
###TEMPORARY!
    unset G4USE_HEPODBMS
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC4/$CLHEP_VERSION
    # Persistency...
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
  else
    export G4SYSTEM=SUN-CC
    export DEBOPT=${DEBOPT}_ISO
    unset G4USE_OSPACE
    export PATH=`echo $PATH | sed s/SUNWspro/SUNWspro50/`
    export PATH=`echo $PATH | sed s/SUNWspro50/SUNWspro/`
    # No Persistency...
    unset G4USE_HEPODBMS
    # Persistency...
#   if [ X$G4USE_HEPODBMS = Xaaa ]; then
    if [ X$G4USE_HEPODBMS = Xaaa ]; then
       . $G4INSTALL/examples/extended/persistency/PersistentEx01/g4odbms_setup.sh
       export G4EXAMPLE_FDID=207
    fi
    export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/SUN-CC/$CLHEP_VERSION
  fi
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  #######export G4UI_BUILD_XM_SESSION=1
  #######export G4VIS_BUILD_OPENGLXM_DRIVER=1
  export G4VIS_BUILD_OPENGLX_DRIVER=1
  export OGLHOME=/usr/local
  export OGLFLAGS="-I$OGLHOME/include"
  export OGLLIBS="-L$OGLHOME/lib -lMesaGLU -lMesaGL"
  #######export G4VIS_BUILD_OIX_DRIVER=1
fi


if [ $UNAMEN = pcgeant ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-egcs
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-egcs/$CLHEP_VERSION
#
# The G4SYSTEm (and directory) was changed!
#  export CLHEP_LIB=CLHEP-egcs

  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM:${CLHEP_BASE_DIR}/lib
  
  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ $UNAMEN = pcg4speed ]; then
  export DEBOPT=${DEBOPT}_NEWGCC
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM
  
  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ $UNAMEN = pcitapi22 ]; then
  export DEBOPT=${DEBOPT}_NEWGCC
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM
  
  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

# pcgeant4 "backup" for pcg4speed
if [ $UNAMEN = pcgeant4 ]; then
  export DEBOPT=${DEBOPT}_NEWGCC
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM
  
  # G4 build flags :
fi

if [ $UNAMEN = pcgeant2 ]; then
  export DEBOPT=${DEBOPT}_7.2_2.95.2
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
#  export CLHEP_BASE_DIR=/home/stesting/CLHEP
#  export CLHEP_BASE_DIR=/afs/cern.ch/sw/lhcxx/specific/redhat61/gcc-2.95.2/CLHEP/1.7.0.0/
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION
#  export G4_NO_CBRT=1

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM:${CLHEP_BASE_DIR}/lib
#  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM

  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ $UNAMEN = tbed0087 ]; then
  export DEBOPT=${DEBOPT}_AFS
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
#  export CLHEP_BASE_DIR=/home/stesting/CLHEP
#  export CLHEP_BASE_DIR=/afs/cern.ch/sw/lhcxx/specific/redhat61/gcc-2.95.2/CLHEP/1.7.0.0/
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION
#  export G4_NO_CBRT=1

# ALternative compiler
#################################
export PATH=/usr/local/gcc-alt-2.95.2/bin:${PATH}
export LD_LIBRARY_PATH=/usr/local/gcc-alt-2.95.2/lib:${LD_LIBRARY_PATH}

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM:${CLHEP_BASE_DIR}/lib
#  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM

  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ $UNAMEN = tbed0088 ]; then
  export DEBOPT=${DEBOPT}_AFS
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
#  export CLHEP_BASE_DIR=/home/stesting/CLHEP
#  export CLHEP_BASE_DIR=/afs/cern.ch/sw/lhcxx/specific/redhat61/gcc-2.95.2/CLHEP/1.7.0.0/
  export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION
#  export G4_NO_CBRT=1

# ALternative compiler
#################################
export PATH=/usr/local/gcc-alt-2.95.2/bin:${PATH}
export LD_LIBRARY_PATH=/usr/local/gcc-alt-2.95.2/lib:${LD_LIBRARY_PATH}

  # Shareable library
  #####################
  export G4LIB_BUILD_SHARED=1
  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM:${CLHEP_BASE_DIR}/lib
#  export LD_LIBRARY_PATH=$G4LIB/$G4SYSTEM

  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi

if [ `uname -n | grep sgmedia` ]; then
  export G4USE_OSPACE=1
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=SGI-CC
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4STTDIR=/afs/cern.ch/sw/geant4/stt/$REF/testtools/geant4/tests/tools
  export G4LIB=$G4WORKDIR/lib
  # G4 build flags :
  ######export G4UI_BUILD_XM_SESSION=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
#  export G4VIS_BUILD_OIX_DRIVER=1
fi


UNAMEN=`uname -n `
if [ X"$UNAMEN" = X"pcl3eth4"  ]; then
UNAME=`uname `
if [ "$UNAME" = "AIX" ]; then
 export G4SYSTEM=AIX-xlC
else
 export G4SYSTEM=Linux-g++
fi	
export G4VIS_DEBUG=1
if [ "$UNAME" = "Linux" ]; then
  export CVSROOT=/afs/cern.ch/sw/geant4/cvs
  export G4SYSTEM=Linux-g++
  export G4INSTALL=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4
  export G4WORKDIR=/afs/cern.ch/sw/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
  export G4LIB=$G4WORKDIR/lib
export G4VIS_BUILD_OPENGLX_DRIVER=1
export G4VIS_USE_OPENGLX=1
export G4VIS_BUILD_OPENGLXM_DRIVER=1
export G4VIS_USE_OPENGLXM=1
export CLHEP_BASE_DIR=/afs/cern.ch/sw/geant4/dev/CLHEP/Linux-g++/$CLHEP_VERSION
#/usr/local/CLHEP1.3/CLHEP
export CLHEP_LIB=CLHEP
#CLHEP-c++
export RWBASE=/usr/local
export RWINC=/usr/local/include
export OGLHOME=/usr/local
fi
export XM_INSTALLED=1
export XKEYSYMDB=/usr/lib/X11/XKeysymDB
export G4UI_USE_TERMINAL=1
export G4UI_USE_GAG=1
export G4UI_BUILD_XM_SESSION=1
fi
#
if [ `uname -n` = aleph ] ; then
export G4USE_OSPACE=1
export CVSROOT=:pserver:barrand@g4cvs.cern.ch:/afs/cern.ch/sw/geant4/cvs
export G4INSTALL=/geant4/stt/$REF/src/geant4
export G4SYSTEM=HP-aCC
export G4WORKDIR=/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
export G4DEBUG=1
# G4 build flags :
export G4UI_BUILD_XM_SESSION=1
export G4VIS_BUILD_OPENGLXM_DRIVER=1
export G4VIS_BUILD_OPENGLX_DRIVER=1
export G4VIS_BUILD_OIX_DRIVER=1
# G4 use flags :
export G4UI_USE_XM=1
export G4VIS_USE_OPENGLXM=1
export G4VIS_USE_OPENGLX=1
export G4VIS_USE_OIX=1
# Specific :
export CLHEP_BASE_DIR=/geant4/HP-UX
export OGLHOME=/geant4/HP-UX
export OIVFLAGS="-I/geant4/HP-UX/include/SoFree"
export OIVLIBS="-L/geant4/HP-UX/lib -lhepvisXt -lhepvis -lSoFreeXt -lSoFree"
export SOFREEUSER=/projects/SoFree/user/
fi
#
if [ `uname -n` = asc ] ; then
export G4USE_OSPACE=1
export CVSROOT=':pserver:barrand@g4cvs.cern.ch:/afs/cern.ch/sw/geant4/cvs'
export G4INSTALL=/geant4/stt/$REF/src/geant4
export G4SYSTEM=OSF1
export G4WORKDIR=/geant4/stt/$REF/$G4SYSTEM/$DEBOPT
export G4DEBUG=1
# G4 build flags :
export G4UI_BUILD_XM_SESSION=1
export G4UI_BUILD_XAW_SESSION=1
export G4VIS_BUILD_OPENGLXM_DRIVER=1
export G4VIS_BUILD_OPENGLX_DRIVER=1
export G4VIS_BUILD_OIX_DRIVER=1
# G4 use flags :
export G4UI_USE_XM=1
export G4UI_USE_XAW=1
export G4VIS_USE_OPENGLXM=1
export G4VIS_USE_OPENGLX=1
export G4VIS_USE_OIX=1
# Specific :
export RWBASE=/geant4/OSF1
export CLHEP_BASE_DIR=/geant4/OSF1
export OGLHOME=/geant4/OSF1
export OIVHOME=/geant4/OpenInventor2.4.1
export OIVFLAGS="-I$OIVHOME/include -I/geant4/OSF1/include"
export OIVLIBS="-L/geant4/OSF1/lib -lhepvisXt -lhepvis  -L$OIVHOME/lib -lInventorXt -lInventor -limage"
export XENVIRONMENT=$OIVHOME/app-defaults/Inventor
fi

if [ `uname -n` = nb-barrand2 ] ; then
#export CVSROOT=':pserver:barrand@g4cvs.cern.ch:/afs/cern.ch/sw/geant4/cvs'
export G4INSTALL=Z:/geant4/geant4-05-00-ref-01
export G4SYSTEM=WIN32-VC
#export G4WORKDIR=$G4INSTALL
export G4WORKDIR=C:/geant4/geant4-05-00-ref-01
export G4STTDIR=$G4WORKDIR/stt
export G4LIB=$G4WORKDIR/lib
export G4DEBUG=1
export CPPVERBOSE=1
# G4 build flags :
export G4UI_BUILD_WIN32_SESSION=1
export G4VIS_BUILD_OPENGLWIN32_DRIVER=1
# G4 use flags :
export G4UI_USE_WIN32=1
export G4VIS_USE_OPENGLWIN32=1
# Specific :
export CLHEP_BASE_DIR=C:/CLHEP/1.8.0.0
fi

# Global environment

export XERCESCROOT=/afs/cern.ch/sw/geant4/dev/XercesC/$G4SYSTEM
