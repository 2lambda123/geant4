/run/verbose 1
# e+ 300MeV 
/gun/particle e+
/gun/energy 300 MeV
/run/beamOn 1000
#
# list the existing physics processes
/process/list
#
# switch off MultipleScattering
/process/inactivate msc
/run/beamOn 1000
#
# switch on MultipleScattering
/process/activate msc 
#
# change detector parameter
/calor/setAbsMat Aluminium
/calor/setGapMat Aerogel
/calor/setAbsThick 2. cm
/calor/setGapThick 5. cm
/calor/setNbOfLayers 30
/calor/setSizeYZ   1.5 m
/calor/update
/gun/particle gamma
/gun/energy 500 MeV
/run/beamOn 1000
exit
