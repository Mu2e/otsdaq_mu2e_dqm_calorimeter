art_make( BASENAME_ONLY
 LIBRARY_NAME otsdaq-trigger_ArtModules
 SUBDIRS detail
 NO_PLUGINS
 EXCLUDE anatka.C 
 DICT_DICTIONARIES
 ${ART_PERSISTENCY_PROVENANCE}
 ${CANVAS_PERSISTENCY_PROVENANCE}
 LIB_LIBRARIES
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
 artdaq_core_mu2e::artdaq-core-mu2e_Overlays
 artdaq::DAQdata
 otsdaq::NetworkUtilities
 art_root_io::TFileService_service
 ${ROOT_BASIC_LIB_LIST}
 )

 simple_plugin(CaloSpy "module"
 artdaq_core_mu2e::artdaq-core-mu2e_Overlays
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
# otsdaq-mu2e-trigger_Overlays
# artdaq_DAQrate
# artdaq_DAQdata
# artdaq-core_Data
# artdaq-core_Core
# MF_MessageLogger
# ${Boost_SYSTEM_LIBRARY}
# ${Boost_THREAD_LIBRARY}
# )

# simple_plugin(OfflineFragmentsDumper "module"
#   artdaq-core-mu2e_Overlays
#   ${CETLIB_EXCEPT}
# )

# simple_plugin(OfflineFragmentReader "source"
#   MF_MessageLogger
#   artdaq_DAQdata
#   otsdaq-mu2e-trigger_ArtModules
#   )

