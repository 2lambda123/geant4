# - Define datasets known and used by Geant4
# We keep this separate from the Geant4InstallData module for conveniance
# when updating and patching because datasets may change more rapidly.
# It allows us to decouple the dataset definitions from how they are
# checked/installed/configured
#

# - NDL
geant4_add_dataset(
  NAME      G4NDL
  VERSION   4.4
  FILENAME  G4NDL
  EXTENSION tar.gz
  ENVVAR    G4NEUTRONHPDATA
  MD5SUM    cd8cc1d96aeb9b9ed37189edda26f5ad
  )

# - Low energy electromagnetics
geant4_add_dataset(
  NAME      G4EMLOW
  VERSION   6.34
  FILENAME  G4EMLOW
  EXTENSION tar.gz
  ENVVAR    G4LEDATA
  MD5SUM    6a720b8f7eeeb8eadd59946cd4fe6ec1
  )

# - Photon evaporation
geant4_add_dataset(
  NAME      PhotonEvaporation
  VERSION   2.3
  FILENAME  G4PhotonEvaporation
  EXTENSION tar.gz
  ENVVAR    G4LEVELGAMMADATA
  MD5SUM    08848ebdd536280a0629d802040b70be
  )

# - Radioisotopes
geant4_add_dataset(
  NAME      RadioactiveDecay
  VERSION   3.7
  FILENAME  G4RadioactiveDecay
  EXTENSION tar.gz
  ENVVAR    G4RADIOACTIVEDATA
  MD5SUM    039e5f64b0e451eb5c095bf81552cb42
  )

# - Neutron XS
geant4_add_dataset(
  NAME      G4NEUTRONXS
  VERSION   1.3
  FILENAME  G4NEUTRONXS
  EXTENSION tar.gz
  ENVVAR    G4NEUTRONXSDATA
  MD5SUM    ede7c4b3e99cbe1773b672a7404fe0f6
  )

# - PII
geant4_add_dataset(
  NAME      G4PII
  VERSION   1.3
  FILENAME  G4PII
  EXTENSION tar.gz
  ENVVAR    G4PIIDATA
  MD5SUM    05f2471dbcdf1a2b17cbff84e8e83b37
  )

# - Optical Surfaces
geant4_add_dataset(
  NAME      RealSurface
  VERSION   1.0
  FILENAME  RealSurface
  EXTENSION tar.gz
  ENVVAR    G4REALSURFACEDATA
  MD5SUM    0dde95e00fcd3bcd745804f870bb6884
  )

# - SAID
geant4_add_dataset(
  NAME      G4SAIDDATA
  VERSION   1.1
  FILENAME  G4SAIDDATA
  EXTENSION tar.gz
  ENVVAR    G4SAIDXSDATA
  MD5SUM    d88a31218fdf28455e5c5a3609f7216f
  )

