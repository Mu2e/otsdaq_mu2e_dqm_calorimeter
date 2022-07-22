 cet_build_plugin(CaloTest art::module LIBRARIES REG
 artdaq_core_mu2e::Overlays
 artdaq::DAQdata
 otsdaq::NetworkUtilities
 art_root_io::TFileService_service
 ROOT::RIO
 ROOT::Gui
 )

 cet_build_plugin(CaloSpy art::module LIBRARIES REG
 artdaq_core_mu2e::Overlays
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
