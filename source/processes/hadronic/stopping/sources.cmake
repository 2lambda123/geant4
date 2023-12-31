# - G4hadronic_stop module build definition

# Define the Geant4 Module.
geant4_add_module(G4hadronic_stop
  PUBLIC_HEADERS
    G4ElementSelector.hh
    G4EmCaptureCascade.hh
    G4HadronicAbsorptionFritiof.hh
    G4HadronicAbsorptionFritiofWithBinaryCascade.hh
    G4HadronicAbsorptionINCLXX.hh
    G4HadronStoppingProcess.hh
    G4MuMinusCapturePrecompound.hh
    G4MuonMinusBoundDecay.hh
    G4MuonMinusCapture.hh
    G4HadronicAbsorptionBertini.hh
    G4MuonMinusAtomicCapture.hh
    G4MuonicAtomDecay.hh
  SOURCES
    G4ElementSelector.cc
    G4EmCaptureCascade.cc
    G4HadronicAbsorptionFritiof.cc
    G4HadronicAbsorptionFritiofWithBinaryCascade.cc
    G4HadronicAbsorptionINCLXX.cc
    G4HadronStoppingProcess.cc
    G4MuMinusCapturePrecompound.cc
    G4MuonMinusBoundDecay.cc
    G4MuonMinusCapture.cc
    G4HadronicAbsorptionBertini.cc
    G4MuonMinusAtomicCapture.cc
    G4MuonicAtomDecay.cc)

geant4_module_link_libraries(G4hadronic_stop
  PUBLIC
    G4globman
    G4hadronic_mgt
    G4hadronic_util
    G4heprandom
    G4materials
    G4partman
    G4procman
    G4track
  PRIVATE
    G4baryons
    G4bosons
    G4had_preequ_exciton
    G4had_string_diff
    G4had_string_frag
    G4had_theo_max
    G4hadronic_bert_cascade
    G4hadronic_binary
    G4hadronic_inclxx_interface
    G4hepgeometry
    G4ions
    G4leptons)
