#!/usr/bin/env bash
#
source /products/setup
setup root v5_34_01 -q "e2:prof"

# setup G4 datasets
#
source /home/g4p/pbs/g4-had-validation/env-setup/g4-datasets-setup.sh

# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
G4WORKDIR=${PBS_O_WORKDIR}/.. 
else
    echo "Variable says: $G4WORKDIR"
    echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
fi

cd ${G4WORKDIR}


# JobID=1
JobID=${PBS_ARRAYID}
seed=$((1234+${JobID}))

printf "#verbose \n0 \n#rad \n" >> piminus.N.${JobID}
printf "#events \n31250 \n" >> piminus.N.${JobID}
printf "#randomSeed\n" >> piminus.N.${JobID}
printf "%d\n" ${seed}  >> piminus.N.${JobID}
printf "#jobID\n" >> piminus.N.${JobID}
printf "%d\n" ${JobID} >> piminus.N.${JobID}
printf "//--------Piminus_processes \n" >> piminus.N.${JobID}
printf "#particle \npi- \n#position(mm) \n0. 0. 0. \n#direction \n0. 0. 1. \n//-------- \n" >> piminus.N.${JobID}

printf "#material \nN \n" >> piminus.N.${JobID}

printf "#momentum(MeV/c) \n0.0 \n" >> piminus.N.${JobID}
printf "// --- \n#generator \nBertiniPreCo \n#run \n#generator \nCHIPS \n#run \n#generator \nstopping \n#run \n" >> piminus.N.${JobID}

printf "#exit\n" >> piminus.N.${JobID}


${G4EXE}/test48 piminus.N.${JobID}
