art_make( BASENAME_ONLY
 LIBRARY_NAME otsdaq-trigger_ArtModules
 SUBDIRS detail
 NO_PLUGINS
 EXCLUDE anatka.C 
 DICT_DICTIONARIES
 ${ART_PERSISTENCY_PROVENANCE}
 ${CANVAS_PERSISTENCY_PROVENANCE}
 LIB_LIBRARIES
canvas
 otsdaq-trigger_Overlays
 #otsdaq-utilities_ECLWriter
 artdaq-core_Utilities
 artdaq_ArtModules
 artdaq-core_Data
 artdaq-core_Core
  art_Framework_Core
  art_Framework_IO_Sources
  art_Framework_Principal
#  canvas_Persistency_Common
  art_Persistency_Provenance
#  canvas_Persistency_Provenance
# canvas_Utilities
  MF_MessageLogger
  ${ROOT_CINTEX}
  ${ROOT_RIO}
  ${ROOT_CORE}
  ${ROOT_NET}
  ${ROOT_REFLEX}
  cetlib
  cetlib_except
  DTCInterface
)


# simple_plugin(DummyAnalyzer "module"
# otsdaq-trigger_Overlays
# artdaq_DAQrate
# artdaq_DAQdata
# artdaq-core_Data
# artdaq-core_Core
# MF_MessageLogger
# ${Boost_SYSTEM_LIBRARY}
# ${Boost_THREAD_LIBRARY}
# )

 simple_plugin(CaloTest "module"
 artdaq-core_Utilities
 NetworkUtilities
artdaq-core_Data
artdaq-core_Core
${ROOT_CORE}
${ROOT_GPAD}
${ROOT_HIST}
${ROOT_GUI}
${ROOT_RIO}
${ROOT_NET}
${CETLIB_EXCEPT}
art_Framework_Services_Registry  
art_root_io
art_root_io_tfile_support
art_root_io_TFileService_service
MF_MessageLogger
# $ENV{OFFLINE_LIB}/libmu2e_ConditionsService.so
# $ENV{OFFLINE_LIB}/libmu2e_Mu2eInterfaces.so
# $ENV{OFFLINE_LIB}/libmu2e_RecoDataProducts.so
# $ENV{OFFLINE_LIB}/libmu2e_MCDataProducts.so
# $ENV{OFFLINE_LIB}/libmu2e_BFieldGeom.so
# $ENV{OFFLINE_LIB}/libmu2e_GeometryService.so
# $ENV{OFFLINE_LIB}/libmu2e_TrackerGeom.so
# $ENV{OFFLINE_LIB}/libmu2e_GlobalConstantsService.so
# $ENV{OFFLINE_LIB}/libmu2e_Mu2eUtilities.so
# $ENV{OFFLINE_LIB}/libmu2e_DataProducts.so
# $ENV{OFFLINE_LIB}/libmu2e_TrackerConditions.so
# $ENV{OFFLINE_LIB}/libmu2e_GlobalConstantsService_GlobalConstantsService_service.so
# $ENV{OFFLINE_LIB}/libmu2e_TrackerMC.so
# $ENV{OFFLINE_LIB}/libmu2e_CaloReco.so
 )

 simple_plugin(CaloSpy "module"
 mu2e_artdaq_core::mu2e-artdaq-core_Overlays
 pcie_linux_kernel_module::DTCInterface
 artdaq_DAQrate
 artdaq_DAQdata
 artdaq-core_Data
 artdaq-core_Core
 MF_MessageLogger
 ${Boost_SYSTEM_LIBRARY}
 ${Boost_THREAD_LIBRARY}
 )

# simple_plugin(CaloCosmics "module"
# otsdaq-trigger_Overlays
# artdaq_DAQrate
# artdaq_DAQdata
# artdaq-core_Data
# artdaq-core_Core
# MF_MessageLogger
# ${Boost_SYSTEM_LIBRARY}
# ${Boost_THREAD_LIBRARY}
# )

# Simple_plugin(PrescaleEvent "module"
# otsdaq-trigger_Overlays
# artdaq_DAQrate
# artdaq_DAQdata
# artdaq-core_Data
# artdaq-core_Core
# MF_MessageLogger
# ${Boost_SYSTEM_LIBRARY}
# ${Boost_THREAD_LIBRARY}
# )

# simple_plugin(OfflineFragmentsDumper "module"
#   mu2e-artdaq-core_Overlays
#   ${CETLIB_EXCEPT}
# )

# simple_plugin(OfflineFragmentReader "source"
#   MF_MessageLogger
#   artdaq_DAQdata
#   otsdaq-trigger_ArtModules
#   )

