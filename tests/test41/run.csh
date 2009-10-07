#/bin/csh

cd $VFEM/test41
mkdir -p $REFERENCE
cd $REFERENCE
rm -f *.*

set    work = "$G4MY/test41"
set    dir  = "$G4INSTALL/tests/test41/"

setenv PHYSLIST    emstandard
set    phys = "opt0"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& res0.out

setenv PHYSLIST    emstandard_opt3
set    phys = "opt3"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& res3.out

setenv PHYSLIST    emstandard_msc2
set    phys = "msc2"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& res2.out

setenv PHYSLIST    standardIG
set    phys = "optG"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& resG.out

setenv PHYSLIST    standardSS
set    phys = "optS"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& resS.out

source ${dir}plot.csh $1 >& p.out
