#/bin/csh

setenv REFERENCE 20081221
setenv PHYSLIST QBBC
source run.csh
setenv PHYSLIST QBBCG
#source run.csh
setenv PHYSLIST QBBCF
#source run.csh
setenv PHYSLIST QGSP_BERT
source run.csh
setenv PHYSLIST QGSP_BERT_EMV
source run.csh
setenv PHYSLIST FTFP_BERT_EMV
source run.csh

#
