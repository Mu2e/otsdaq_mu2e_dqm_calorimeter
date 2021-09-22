//Author: S Middleton
//Date: 2020
//Purpose: prototype Analyzer fir DQM trigger rates. Based on Offline module Trigger/srs/ots::ProtoType. Much of the code was copied over from that module but has been adapted for Online vicualization.

//Art:
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "canvas/Utilities/InputTag.h"
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "canvas/Persistency/Common/TriggerResults.h"
#include "art/Framework/Services/System/TriggerNamesService.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"

#include "artdaq-core/Data/ContainerFragment.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "artdaq/DAQdata/Globals.hh"

#include "cetlib_except/exception.h"

//OTS:
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

//ROOT:
//#include "art/Framework/Services/Optional/TFileService.h"
#include "art_root_io/TFileService.h" 
#include <TBufferFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH1.h>
#include <TProfile.h>

//Offline:
#include <Offline/ConditionsService/inc/AcceleratorParams.hh>
#include <Offline/ConditionsService/inc/ConditionsHandle.hh>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits" 
#pragma GCC diagnostic ignored "-Wpedantic"
#include <Offline/BFieldGeom/inc/BFieldManager.hh>
#include <Offline/TrackerGeom/inc/Tracker.hh>
//#include <GlobalConstantsService/inc/GlobalConstantsHandle.hh">
#pragma GCC diagnostic pop

#include <Offline/GeometryService/inc/GeomHandle.hh>
#include <Offline/GeometryService/inc/DetectorSystem.hh>
#include <Offline/RecoDataProducts/inc/CaloCluster.hh>
#include <Offline/RecoDataProducts/inc/CaloTrigSeed.hh>
#include <Offline/RecoDataProducts/inc/HelixSeed.hh>
#include <Offline/RecoDataProducts/inc/KalSeed.hh>
#include <Offline/RecoDataProducts/inc/TriggerInfo.hh>
#include <Offline/RecoDataProducts/inc/ComboHit.hh>
#include <Offline/RecoDataProducts/inc/StrawDigi.hh>
#include <Offline/RecoDataProducts/inc/StrawDigiCollection.hh>
#include <Offline/RecoDataProducts/inc/CaloDigi.hh>

#include <Offline/MCDataProducts/inc/ProtonBunchIntensity.hh>

#include <Offline/GlobalConstantsService/inc/ParticleDataTable.hh>

//OTS:
#include "otsdaq-dqm-calorimeter/ArtModules/CaloTestHistos.h"
#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/ProcessorPluginMacros.h"
#include "otsdaq/NetworkUtilities/TCPSendClient.h"
//C++:
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

#define TRACE_NAME "CaloTest"

namespace ots
{
	class CaloTest : public art::EDAnalyzer
	{
	public:
		explicit CaloTest(fhicl::ParameterSet const& pset);
		virtual ~CaloTest();

		void analyze(art::Event const& e) override;
		void beginRun(art::Run const&) override;
		void beginJob() override;
		void endJob() override;

		void PlotRate(art::Event const& e);
	private:
		art::RunNumber_t current_run_;
		std::string outputFileName_;
		art::ServiceHandle<art::TFileService> tfs;
		bool writeOutput_;
		bool doStreaming_;
		bool overwrite_mode_;

		art::InputTag _trigAlgTag;
		art::InputTag _sdMCTag;
		art::InputTag _sdTag;

		const mu2e::Tracker* _tracker;

		const art::Event* _event;
		CaloTestHistos* histos = new CaloTestHistos("hCalo");
	  std::unique_ptr<TCPSendClient> tcp;

	};
}

ots::CaloTest::CaloTest(fhicl::ParameterSet const& pset)
	: art::EDAnalyzer(pset),
	current_run_(0),
	outputFileName_(pset.get<std::string>("fileName", "DQMCaloTest.root")),
	writeOutput_(pset.get<bool>("write_to_file", true)),
	doStreaming_(pset.get<bool>("stream_to_screen", true)),
	overwrite_mode_(pset.get<bool>("overwrite_output_file", true)),
	tcp(new TCPSendClient(pset.get<std::string>("address"),pset.get<int>("port", 6000)))
{
	TLOG(TLVL_INFO) << "CaloTest construction is beginning ";
	TLOG(TLVL_DEBUG) << "CaloTest construction complete";
}

ots::CaloTest::~CaloTest() {}



void ots::CaloTest::beginJob() {
	TLOG(TLVL_INFO) << "Started";
	tcp->connect();
	histos->BookHistos(tfs);
}

void ots::CaloTest::analyze(art::Event const& event)
{
	TLOG(TLVL_INFO) << "CaloTest Module is Analyzing Event #  " << event.event();
	double value = 100.;
	histos->CaloTest._FirstCaloHist->Fill(value);
	TBufferFile message(TBuffer::kWrite);
	message.WriteObject(histos->CaloTest._FirstCaloHist);

	//__CFG_COUT__ << "Broadcasting!" << std::endl;
	tcp->sendPacket(message.Buffer(), message.Length());

}


void ots::CaloTest::endJob() {
	TLOG(TLVL_INFO) << "Completed";
}

void ots::CaloTest::beginRun(const art::Run& run) {

}




DEFINE_ART_MODULE(ots::CaloTest)
