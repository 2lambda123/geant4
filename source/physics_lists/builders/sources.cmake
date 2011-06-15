#------------------------------------------------------------------------------
# sources.cmake
# Module : G4phys_builders
# Package: Geant4.src.G4physicslists.G4phys_builders
#
# Sources description for a library.
# Lists the sources and headers of the code explicitely.
# Lists include paths needed.
# Lists the internal granular and global dependencies of the library.
# Source specific properties should be added at the end.
#
# Generated on : 24/9/2010
#
# $Id: sources.cmake,v 1.2 2010-11-29 17:23:53 bmorgan Exp $
# GEANT4 Tag $Name: not supported by cvs2svn $
#
#------------------------------------------------------------------------------

# List external includes needed.
include_directories(${CLHEP_INCLUDE_DIRS})

# List internal includes needed.
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/magneticfield/include)
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/navigation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/volumes/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPGeometry/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPNumerics/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPRandom/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/intercoms/include)
include_directories(${CMAKE_SOURCE_DIR}/source/materials/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/bosons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/barions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/ions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/mesons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/leptons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/shortlived/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/cuts/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/decay/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/dna/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/highenergy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/lowenergy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/muons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/standard/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/utils/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/electromagnetic/xrays/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/cross_sections/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/binary_cascade/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/cascade/cascade/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/chiral_inv_phase_space/body/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/chiral_inv_phase_space/cross_sections/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/chiral_inv_phase_space/fragmentation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/chiral_inv_phase_space/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/chiral_inv_phase_space/processes/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/coherent_elastic/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/evaporation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/fermi_breakup/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/handler/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/multifragmentation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/photon_evaporation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/high_energy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/im_r_matrix/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/incl/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/lend/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/low_energy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/neutron_hp/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/parton_string/diffraction/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/parton_string/hadronization/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/parton_string/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/parton_string/qgsm/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/photolepton_hadron/muon_nuclear/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/pre_equilibrium/exciton_model/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/qmd/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/radioactive_decay/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/theo_high_energy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/processes/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/stopping/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/optical/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/transportation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/run/include)
include_directories(${CMAKE_SOURCE_DIR}/source/track/include)

#
# Define the Geant4 Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME G4phys_builders
    HEADERS
        G4AntiBarionBuilder.hh
        CompileTimeConstraints.hh
        G4BertiniNeutronBuilder.hh
        G4BertiniPiKBuilder.hh
        G4BertiniPionBuilder.hh
        G4BertiniProtonBuilder.hh
        G4BinaryNeutronBuilder.hh
        G4BinaryPiKBuilder.hh
        G4BinaryProtonBuilder.hh
        G4BuilderType.hh
        G4ChargeExchangePhysics.hh
        G4ChipsKaonBuilder.hh
        G4DataQuestionaire.hh
        G4DecayPhysics.hh
        G4ElectroNuclearBuilder.hh
        G4EmDNAPhysics.hh
        G4EmExtraPhysics.hh
        G4EmLivermorePhysics.hh
        G4EmLivermorePolarizedPhysics.hh
        G4EmMessenger.hh
        G4EmPenelopePhysics.hh
        G4EmStandardPhysics.hh
        G4EmStandardPhysics_option1.hh
        G4EmStandardPhysics_option2.hh
        G4EmStandardPhysics_option3.hh
        G4FTFBinaryNeutronBuilder.hh
        G4FTFBinaryPiKBuilder.hh
        G4FTFBinaryProtonBuilder.hh
        G4FTFBuilder.hh
        G4FTFCNeutronBuilder.hh
        G4FTFCPiKBuilder.hh
        G4FTFCProtonBuilder.hh
        G4FTFPAntiBarionBuilder.hh
        G4FTFPNeutronBuilder.hh
        G4FTFPPiKBuilder.hh
        G4FTFPProtonBuilder.hh
        G4HadronDElasticPhysics.hh
        G4HadronElasticPhysics.hh
        G4HadronElasticPhysicsHP.hh
        G4HadronElasticPhysicsLEND.hh
        G4HadronElasticPhysicsLHEP.hh
        G4HadronElasticPhysicsXS.hh
        G4HadronHElasticPhysics.hh
        G4HadronQElasticPhysics.hh
        G4HyperonCHIPSBuilder.hh
	G4HyperonFTFPBuilder.hh
        G4HyperonLHEPBuilder.hh
        G4InclAblaNeutronBuilder.hh
        G4InclAblaPiKBuilder.hh
        G4InclAblaProtonBuilder.hh
        G4IonBinaryCascadePhysics.hh
        G4IonFTFPBinaryCascadePhysics.hh
        G4IonInclAblaPhysics.hh
        G4IonPhysics.hh
        G4IonQMDPhysics.hh
        G4KaonBuilder.hh
        G4LEPNeutronBuilder.hh
        G4LEPPiKBuilder.hh
        G4LEPPionBuilder.hh
        G4LEPProtonBuilder.hh
        G4LHEPAntiBarionBuilder.hh
        G4LHEPNeutronBuilder.hh
        G4LHEPPiKBuilder.hh
        G4LHEPProtonBuilder.hh
        G4LHEPStoppingPhysics.hh
        G4MiscCHIPSBuilder.hh
        G4MiscLHEPBuilder.hh
        G4MiscQGSCBuilder.hh
        G4NeutronBuilder.hh
        G4NeutronCrossSectionXS.hh
        G4NeutronLENDBuilder.hh
        G4NeutronHPBuilder.hh
        G4NeutronTrackingCut.hh
        G4OpticalPhysics.hh
        G4OpticalPhysicsMessenger.hh
        G4PiKBuilder.hh
        G4PionBuilder.hh
        G4PrecoNeutronBuilder.hh
        G4PrecoProtonBuilder.hh
        G4ProtonBuilder.hh
        G4QAtomicPhysics.hh
        G4QCaptureAtRestPhysics.hh
        G4QElasticPhysics.hh
        G4QEmExtraPhysics.hh
        G4QGSBinaryNeutronBuilder.hh
        G4QGSBinaryPiKBuilder.hh
        G4QGSBinaryProtonBuilder.hh
        G4QGSBuilder.hh
        G4QGSC_CHIPSNeutronBuilder.hh
        G4QGSC_CHIPSPiKBuilder.hh
        G4QGSC_CHIPSProtonBuilder.hh
        G4QGSCEflowNeutronBuilder.hh
        G4QGSCEflowPiKBuilder.hh
        G4QGSCEflowProtonBuilder.hh
        G4QGSCNeutronBuilder.hh
        G4QGSCPiKBuilder.hh
        G4QGSCProtonBuilder.hh
        G4QGSC_QGSCNeutronBuilder.hh
        G4QGSC_QGSCPiKBuilder.hh
        G4QGSC_QGSCProtonBuilder.hh
        G4QGSPNeutronBuilder.hh
        G4QGSPPiKBuilder.hh
        G4QGSPPionBuilder.hh
        G4QGSPProtonBuilder.hh
        G4QInelasticCHIPSBuilder.hh
        G4QIonPhysics.hh
        G4QMessenger.hh
        G4QNeutrinoPhysics.hh
        G4QNeutronBuilder.hh
        G4QPhotoNuclearPhysics.hh
        G4QProtonBuilder.hh
        G4QStoppingPhysics.hh
        G4RadioactiveDecayPhysics.hh
        G4StoppingHadronBuilder.hh
        G4VAntiBarionBuilder.hh
        G4VHadronModelBuilder.hh
        G4VKaonBuilder.hh
        G4VNeutronBuilder.hh
        G4VPiKBuilder.hh
        G4VPionBuilder.hh
        G4VProtonBuilder.hh
        G4WarnPLStatus.hh
        TheoModelFactory.hh
    SOURCES
        G4AntiBarionBuilder.cc
        G4BertiniNeutronBuilder.cc
        G4BertiniPiKBuilder.cc
        G4BertiniPionBuilder.cc
        G4BertiniProtonBuilder.cc
        G4BinaryNeutronBuilder.cc
        G4BinaryPiKBuilder.cc
        G4BinaryProtonBuilder.cc
        G4ChargeExchangePhysics.cc
        G4ChipsKaonBuilder.cc
        G4DecayPhysics.cc
        G4ElectroNuclearBuilder.cc
        G4EmDNAPhysics.cc
        G4EmExtraPhysics.cc
        G4EmLivermorePhysics.cc
        G4EmLivermorePolarizedPhysics.cc
        G4EmMessenger.cc
        G4EmPenelopePhysics.cc
        G4EmStandardPhysics.cc
        G4EmStandardPhysics_option1.cc
        G4EmStandardPhysics_option2.cc
        G4EmStandardPhysics_option3.cc
        G4FTFBinaryNeutronBuilder.cc
        G4FTFBinaryPiKBuilder.cc
        G4FTFBinaryProtonBuilder.cc
        G4FTFBuilder.cc
        G4FTFCNeutronBuilder.cc
        G4FTFCPiKBuilder.cc
        G4FTFCProtonBuilder.cc
        G4FTFPAntiBarionBuilder.cc
        G4FTFPNeutronBuilder.cc
        G4FTFPPiKBuilder.cc
        G4FTFPProtonBuilder.cc
        G4HadronDElasticPhysics.cc
        G4HadronElasticPhysics.cc
        G4HadronElasticPhysicsHP.cc
        G4HadronElasticPhysicsLEND.cc
        G4HadronElasticPhysicsLHEP.cc
        G4HadronElasticPhysicsXS.cc
        G4HadronHElasticPhysics.cc
        G4HadronQElasticPhysics.cc
        G4HyperonCHIPSBuilder.cc
        G4HyperonFTFPBuilder.cc
        G4HyperonLHEPBuilder.cc
        G4InclAblaNeutronBuilder.cc
        G4InclAblaPiKBuilder.cc
        G4InclAblaProtonBuilder.cc
        G4IonBinaryCascadePhysics.cc
        G4IonFTFPBinaryCascadePhysics.cc
        G4IonInclAblaPhysics.cc
        G4IonPhysics.cc
        G4IonQMDPhysics.cc
        G4KaonBuilder.cc
        G4LEPNeutronBuilder.cc
        G4LEPPiKBuilder.cc
        G4LEPPionBuilder.cc
        G4LEPProtonBuilder.cc
        G4LHEPAntiBarionBuilder.cc
        G4LHEPNeutronBuilder.cc
        G4LHEPPiKBuilder.cc
        G4LHEPProtonBuilder.cc
        G4LHEPStoppingPhysics.cc
        G4MiscCHIPSBuilder.cc
        G4MiscLHEPBuilder.cc
        G4MiscQGSCBuilder.cc
        G4NeutronBuilder.cc
        G4NeutronCrossSectionXS.cc
        G4NeutronLENDBuilder.cc
        G4NeutronHPBuilder.cc
        G4NeutronTrackingCut.cc
        G4OpticalPhysics.cc
        G4OpticalPhysicsMessenger.cc
        G4PiKBuilder.cc
        G4PionBuilder.cc
        G4PrecoNeutronBuilder.cc
        G4PrecoProtonBuilder.cc
        G4ProtonBuilder.cc
        G4QAtomicPhysics.cc
        G4QCaptureAtRestPhysics.cc
        G4QElasticPhysics.cc
        G4QEmExtraPhysics.cc
        G4QGSBinaryNeutronBuilder.cc
        G4QGSBinaryPiKBuilder.cc
        G4QGSBinaryProtonBuilder.cc
        G4QGSBuilder.cc
        G4QGSC_CHIPSNeutronBuilder.cc
        G4QGSC_CHIPSPiKBuilder.cc
        G4QGSC_CHIPSProtonBuilder.cc
        G4QGSCEflowNeutronBuilder.cc
        G4QGSCEflowPiKBuilder.cc
        G4QGSCEflowProtonBuilder.cc
        G4QGSCNeutronBuilder.cc
        G4QGSCPiKBuilder.cc
        G4QGSCProtonBuilder.cc
        G4QGSC_QGSCNeutronBuilder.cc
        G4QGSC_QGSCPiKBuilder.cc
        G4QGSC_QGSCProtonBuilder.cc
        G4QGSPNeutronBuilder.cc
        G4QGSPPiKBuilder.cc
        G4QGSPPionBuilder.cc
        G4QGSPProtonBuilder.cc
        G4QInelasticCHIPSBuilder.cc
        G4QIonPhysics.cc
        G4QMessenger.cc
        G4QNeutrinoPhysics.cc
        G4QNeutronBuilder.cc
        G4QPhotoNuclearPhysics.cc
        G4QProtonBuilder.cc
        G4QStoppingPhysics.cc
        G4RadioactiveDecayPhysics.cc
        G4StoppingHadronBuilder.cc
        G4VAntiBarionBuilder.cc
        G4VHadronModelBuilder.cc
        G4VKaonBuilder.cc
        G4VNeutronBuilder.cc
        G4VPiKBuilder.cc
        G4VPionBuilder.cc
        G4VProtonBuilder.cc
        G4WarnPLStatus.cc
    GRANULAR_DEPENDENCIES
        G4baryons
        G4bosons
        G4cuts
        G4decay
        G4emdna
        G4emhighenergy
        G4emlowenergy
        G4emstandard
        G4emutils
        G4geometrymng
        G4globman
        G4had_im_r_matrix
        G4had_lend
        G4had_mod_man
        G4had_mod_util
        G4had_muon_nuclear
        G4had_neu_hp
        G4had_preequ_exciton
        G4had_string_diff
        G4had_string_frag
        G4had_string_man
        G4had_theo_max
        G4hadronic_HE
        G4hadronic_LE
        G4hadronic_bert_cascade
        G4hadronic_binary
        G4hadronic_body_ci
        G4hadronic_coherent_elastic
        G4hadronic_crosec_ci
        G4hadronic_deex_evaporation
        G4hadronic_deex_fermi_breakup
        G4hadronic_deex_handler
        G4hadronic_deex_management
        G4hadronic_deex_multifragmentation
        G4hadronic_deex_photon_evaporation
        G4hadronic_deex_util
        G4hadronic_fragm_ci
        G4hadronic_incl_cascade
        G4hadronic_interface_ci
        G4hadronic_mgt
        G4hadronic_proc
        G4hadronic_proc_ci
        G4hadronic_qgstring
        G4hadronic_qmd
        G4hadronic_radioactivedecay
        G4hadronic_stop
        G4hadronic_util
        G4hadronic_xsect
        G4hepnumerics
        G4intercoms
        G4ions
        G4leptons
        G4magneticfield
        G4materials
        G4mesons
        G4muons
        G4navigation
        G4optical
        G4partman
        G4procman
        G4run
        G4shortlived
        G4track
        G4transportation
        G4volumes
        G4xrays
    GLOBAL_DEPENDENCIES
        G4geometry
        G4global
        G4intercoms
        G4materials
        G4particles
        G4processes
        G4run
        G4track
    LINK_LIBRARIES
)

# List any source specific properties here

