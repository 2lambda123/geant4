#!/usr/local/bin/bash
######################################
# Submit remote build&run 
######################################

############################################################
# 'account' MUST have next in PATH:
# /afs/cern.ch/sw/geant4/stt/prod[dev]/src/geant4/tests/tools/bin 
# This is no longer required - but leave because there are side effects
#   to solve.
############################################################
account=stesting
 
while read host devprod debopt tag action actarg1 actarg2 actarg3 nonincremental
do
    sym_devprod=`echo $devprod|cut -c 1`
    if [ X$sym_devprod = Xd ]
    then
	REF=dev
    else
	REF=prod
    fi

    g4sbr=g4sbr.sh
#    g4sbr=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4/tests/tools/bin/g4sbr.sh

# This was intended to help the g4status command.  Let's think of another way.
#    sym_debopt=`echo $debopt|cut -c 1`
#    if [ X$sym_debopt = Xo ]
#    then
#	g4sbr=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4/tests/tools/bin/g4sbr.o.sh
#    else
#	g4sbr=/afs/cern.ch/sw/geant4/stt/$REF/src/geant4/tests/tools/bin/g4sbr.d.sh
#    fi

    command="rsh -l $account $host $g4sbr $devprod $debopt $tag $action $actarg1 $actarg2 $actarg3 $nonincremental"
    if [ X$host = X\# ]
    then
	echo "String is comment!"
    else
	echo $PATH >>  $host.$devprod.$debopt.log 2>&1 
	echo "______________________" >>  $host.$devprod.$debopt.log 2>&1 
	echo $command >>  $host.$devprod.$debopt.log 2>&1 
	$command >> $host.$devprod.$debopt.log 2>&1 &
     fi	
done
