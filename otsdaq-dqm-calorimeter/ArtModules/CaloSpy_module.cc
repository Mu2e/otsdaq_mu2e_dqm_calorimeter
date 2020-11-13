// ======================================================================
//
// CaloSpy:  Monitoring of calorimeter readout channels
//
// ======================================================================

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "fhiclcpp/ParameterSet.h"

#include "art/Framework/Principal/Handle.h"
#include "mu2e-artdaq-core/Overlays/CalorimeterFragment.hh"

#include <artdaq-core/Data/Fragment.hh>

// ROOT includes
//#include "art/Framework/Services/Optional/TFileService.h"
//#include "art_root_io/TFileService.h" // Include moved here since art v3_02_04
//#include <TH1F.h>
//#include <TH1.h>
//#include <TProfile.h>

#include <iostream>

#include <string>

#include <memory>

int Ntot = 0;
const int nCryDisk = 674;
const int nCryTot = 2 * nCryDisk;

// Notation: 0/1 disk number; A/B readout side

struct cryStru
{
	float WFpeakA = 0.;
	float WFpeakB = 0.;
	float WFratio = 0.;
	float TriseA = 0.;
	float TriseB = 0.;
	float TdecayA = 0.;
	float TdecayB = 0.;
};

//TH1F     *_hCaloOccupancy0    = nullptr;
//TH1F     *_hCaloOccupancy1    = nullptr;
//TProfile *_hMaxWaveForm0      = nullptr;
//TProfile *_hMaxWaveForm1      = nullptr;
//TProfile *_hMaxWFratio0       = nullptr;
//TProfile *_hMaxWFratio1       = nullptr;
//TProfile *_hTrise0            = nullptr;
//TProfile *_hTrise1            = nullptr;
//TProfile *_hTdecay0           = nullptr;
//TProfile *_hTdecay1           = nullptr;
//TProfile *_hWave[6][28][20]   = {};

// Temporary, waiting for ROOT fixing                                                                    
float OccupancyVec[nCryTot] = {};

namespace art {
	class CaloSpy;
}

using art::CaloSpy;

// ======================================================================

class art::CaloSpy
	: public EDAnalyzer
{

public:

	using EventNumber_t = art::EventNumber_t;
	using adc_t = uint16_t;

	// --- C'tor/d'tor:
	explicit  CaloSpy(fhicl::ParameterSet const& pset);
	virtual  ~CaloSpy() { }

	// --- Production:
	virtual void beginJob();
	virtual void analyze(Event const&);
	virtual void endJob();

private:
	int   diagLevel_;

	int   parseCAL_;
	int   parseTRK_;

	art::InputTag trkFragmentsTag_;
	art::InputTag caloFragmentsTag_;

};  // CaloSpy

// ======================================================================

CaloSpy::CaloSpy(fhicl::ParameterSet const& pset)
	: EDAnalyzer(pset)
	, diagLevel_(pset.get<int>("diagLevel", 0))
	, parseCAL_(pset.get<int>("parseCAL", 1))
	, trkFragmentsTag_(pset.get<art::InputTag>("trkTag", "daq:trk"))
	, caloFragmentsTag_(pset.get<art::InputTag>("caloTag", "daq:calo"))
{
}

// ======================================================================
// Begin Job - Histogram booking
// ======================================================================

void CaloSpy::beginJob() {
	/*
	art::ServiceHandle<art::TFileService> tfs;

	char *hName = new char[20];;

	_hCaloOccupancy0 = tfs->make<TH1F>("hCaloOccupancy0","Calo Occupancy - Disk 0",nCryTot,0.,nCryTot);
	_hCaloOccupancy1 = tfs->make<TH1F>("hCaloOccupancy1","Calo Occupancy - Disk 1",nCryTot,0.,nCryTot);
	_hMaxWaveForm0 = tfs->make<TProfile>("hMaxWaveForm0","Max of WaveForm - Disk 0",nCryTot,0.,nCryTot,0.,2000.);
	_hMaxWaveForm1 = tfs->make<TProfile>("hMaxWaveForm1","Max of WaveForm - Disk 1",nCryTot,0.,nCryTot,0.,2000.);
	_hMaxWFratio0  = tfs->make<TProfile>("hMaxWFratio0"," Ratio of Max WF - Disk 0",nCryDisk,0.,nCryDisk,0.,2000.);
	_hMaxWFratio1  = tfs->make<TProfile>("hMaxWFratio1"," Ratio of Max WF - Disk 1",nCryDisk,0.,nCryDisk,0.,2000.);
	_hTrise0  = tfs->make<TProfile>("hTrise0","Rise Time - Disk 0",nCryTot,0.,nCryTot,0.,2000.);
	_hTrise1  = tfs->make<TProfile>("hTrise1","Rise Time - Disk 1",nCryTot,0.,nCryTot,0.,2000.);
	_hTdecay0 = tfs->make<TProfile>("hTdecay0","Decay Time - Disk 0",nCryTot,0.,nCryTot,0.,2000.);
	_hTdecay1 = tfs->make<TProfile>("hTdecay1","Decay Time - Disk 1",nCryTot,0.,nCryTot,0.,2000.);

	for( int iRoc=0; iRoc<6; iRoc++){
	  for( int iDtc=0; iDtc<28; iDtc++){
	for( int iCha=0; iCha<20; iCha++){
	  sprintf(hName,"Wave_%d_%d_%d",iRoc,iDtc,iCha);
	  _hWave[iRoc][iDtc][iCha] = tfs->make<TProfile>(hName,"Wave",30,0.,30,-100.,2000.);
	}}}
	*/
}

// ======================================================================
// Event Analyzer
// ======================================================================

void
CaloSpy::
analyze(Event const& event)
{
	art::EventNumber_t eventNumber = event.event();

	++Ntot;
	struct cryStru crystal[nCryTot];

	art::Handle<artdaq::Fragments> trkFragments, calFragments;
	size_t numTrkFrags(0), numCalFrags(0);
	if (parseTRK_) {
		event.getByLabel(trkFragmentsTag_, trkFragments);
		if (!trkFragments.isValid()) {
			return;
		}
		numTrkFrags = trkFragments->size();
	}
	if (parseCAL_) {
		event.getByLabel(caloFragmentsTag_, calFragments);
		if (!calFragments.isValid()) {
			return;
		}
		numCalFrags = calFragments->size();
	}
	// size_t numTrkFrags = trkFragments->size();
	// size_t numCalFrags = calFragments->size();

	if (diagLevel_ > 1) {
		std::cout << std::dec << "[CaloSpy] Run " << event.run() << ", subrun " << event.subRun()
			<< ", event " << eventNumber << " has " << std::endl;
		std::cout << numTrkFrags << " TRK fragments, and ";
		std::cout << numCalFrags << " CAL fragments." << std::endl;

		size_t totalSize = 0;
		for (size_t idx = 0; idx < numTrkFrags; ++idx) {
			auto size = ((*trkFragments)[idx]).size() * sizeof(artdaq::RawDataType);
			totalSize += size;
			//      std::cout << "\tTRK Fragment " << idx << " has size " << size << std::endl;
		}
		for (size_t idx = 0; idx < numCalFrags; ++idx) {
			auto size = ((*calFragments)[idx]).size() * sizeof(artdaq::RawDataType);
			totalSize += size;
			//      std::cout << "\tCAL Fragment " << idx << " has size " << size << std::endl;
		}

		std::cout << "\tTotal Size: " << (int)totalSize << " bytes." << std::endl;
	}

	std::string curMode = "TRK";

	// Loop over the TRK and CAL fragments
	for (size_t idx = 0; idx < numTrkFrags + numCalFrags; ++idx) {

		auto curHandle = trkFragments;
		size_t curIdx = idx;
		if (idx >= numTrkFrags) {
			curIdx = idx - numTrkFrags;
			curHandle = calFragments;
		}
		const auto& fragment((*curHandle)[curIdx]);

		mu2e::ArtFragment cc(fragment);

		if (diagLevel_ > 1) {
			std::cout << std::endl;
			std::cout << "ArtFragmentReader: ";
			std::cout << "\tBlock Count: " << std::dec << cc.block_count() << std::endl;
			std::cout << std::endl;
			std::cout << "\t" << "====== Example Block Sizes ======" << std::endl;
			for (size_t i = 0; i < 10; i++) {
				if (i < cc.block_count()) {
					std::cout << "\t" << i << "\t" << "\t" << cc.blockSizeBytes(i) << std::endl;
				}
			}
			std::cout << "\t" << "=========================" << std::endl;
		}

		std::string mode_;

		for (size_t curBlockIdx = 0; curBlockIdx < cc.block_count(); curBlockIdx++) {


			adc_t const* pos = reinterpret_cast<adc_t const*>(cc.dataAtBlockIndex(curBlockIdx));

			if (diagLevel_ > 1) {
				// Print binary contents the first 3 packets starting at the current position
				// In the case of the tracker simulation, this will be the whole tracker
				// DataBlock. In the case of the calorimeter, the number of data packets
				// following the header packet is variable.
				cc.printPacketAtByte(curBlockIdx, 16 * (0));
				cc.printPacketAtByte(curBlockIdx, 16 * (1));
				cc.printPacketAtByte(curBlockIdx, 16 * (2));

				// Print out decimal values of 16 bit chunks of packet data
				for (int i = 7; i >= 0; i--) {
					std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << (adc_t) * (pos + i) << std::dec << std::setw(0);
					std::cout << " ";
				}
				std::cout << std::endl;
			}

			auto data = cc.dataAtBlockIndex(curBlockIdx);
			if (data == nullptr) {
				mf::LogError("CaloSpy") << "Unable to retrieve data from block " << curBlockIdx << "!" << std::endl;
				continue;
			}
			auto hdr = data->GetHeader();

			if (diagLevel_ > 1) {


				std::cout << "timestamp: " << static_cast<int>(hdr.GetTimestamp().GetTimestamp(true)) << std::endl;
				std::cout << "hdr->SubsystemID: " << static_cast<int>(hdr.GetSubsystemID()) << std::endl;
				std::cout << "dtcID: " << static_cast<int>(hdr.GetID()) << std::endl;
				std::cout << "rocID: " << static_cast<int>(hdr.GetRingID()) << std::endl;
				std::cout << "packetCount: " << static_cast<int>(hdr.GetPacketCount()) << std::endl;
				std::cout << "EVB mode: " << static_cast<int>(hdr.GetEVBMode()) << std::endl;

				for (int i = 7; i >= 0; i--) {
					std::cout << (adc_t) * (pos + 8 + i);
					std::cout << " ";
				}
				std::cout << std::endl;
			}

			if (idx < numTrkFrags) {
				mode_ = "TRK";
			}
			else {
				mode_ = "CAL";
			}

			int CalPoi = 0;

			if (mode_ == "CAL" && hdr.GetPacketCount() > 0 && parseCAL_ > 0) {// Parse phyiscs information from CAL packets

				mu2e::CalorimeterFragment calFrag(fragment);
				auto calData = calFrag.GetCalorimeterData(curBlockIdx);
				if (calData == nullptr) {
					mf::LogError("CaloSpy") << "Error retrieving Calorimeter data from block " << curBlockIdx << "! Aborting processing of this block!";
					continue;
				}

				if (diagLevel_ > 0) {
					std::cout << "[CaloSpy] NEW CALDATA: NumberOfHits " << calData->NumberOfHits << std::endl;
				}

				auto hitCounter = 0;
				auto hits = calFrag.GetCalorimeterHits(curBlockIdx);
				for (auto& hitData : hits) {

					auto hitPkt = hitData.first;
					// Fill the CaloDigiCollection
					if (diagLevel_ > 0) {
						std::cout << "[CaloSpy] calo hit " << hitCounter << std::endl;
						std::cout << "[CaloSpy] \tChNumber   " << (int)hitPkt.ChannelNumber << std::endl;
						std::cout << "[CaloSpy] \tDIRACA     " << (int)hitPkt.DIRACA << std::endl;
						std::cout << "[CaloSpy] \tDIRACB     " << (int)hitPkt.DIRACB << std::endl;
						std::cout << "[CaloSpy] \tErrorFlags " << (int)hitPkt.ErrorFlags << std::endl;
						std::cout << "[CaloSpy] \tTime       " << (int)hitPkt.Time << std::endl;
						std::cout << "[CaloSpy] \tNSamples   " << (int)hitPkt.NumberOfSamples << std::endl;
						std::cout << "[CaloSpy] \tIndexMax   " << (int)hitPkt.IndexOfMaxDigitizerSample << std::endl;
					}

					//the second argument is not included in the vector, so we need to add "+1"
					// because we want the "last" item included
					auto cwf = hitData.second;

					// IMPORTANT NOTE: we don't have a final
					// mapping yet so for the moment, the BoardID field (described in docdb 4914) is just a
					// placeholder. Because we still need to know which crystal a hit belongs to, we are
					// temporarily storing the 4-bit apdID and 12-bit crystalID in the Reserved DIRAC A slot.
					// Also, note that until we have an actual map, channel index does not actually correspond
					// to the physical readout channel on a ROC.
					adc_t crystalID = hitPkt.DIRACB & 0x0FFF;
					adc_t apdID = hitPkt.DIRACB >> 12;
					adc_t numSamples = hitPkt.NumberOfSamples;
					adc_t peakIdx = hitPkt.IndexOfMaxDigitizerSample;
					adc_t channelID = crystalID % 20;  // *** TEMPORARY until final mapping ***
					if (diagLevel_ > 1) {
						// Until we have the final mapping, the BoardID is just a placeholder
						// adc_t BoardId    = cc.DBC_BoardID(pos,channelIdx);

						std::cout << "Waveform: {";
						for (size_t i = 0; i < cwf.size(); i++) {
							std::cout << cwf[i];
							if (i < cwf.size() - 1) {
								std::cout << ",";
							}
						}
						std::cout << "}" << std::endl;
						std::cout << "Crystal ID: " << (int)crystalID << std::endl;
						std::cout << "APD ID: " << (int)apdID << std::endl;
						//std::cout << "Time: " << (int)hitPkt->Time << std::endl;
						//std::cout << "NumSamples: " << (int)hitPkt->NumberOfSamples << std::endl;

					//-------------------------
					// Fill crystal structure
					//-------------------------

						if (apdID == 0) {
							crystal[crystalID].WFpeakA = cwf[peakIdx];
							crystal[crystalID].TriseA = 5. * (peakIdx - 4);
							crystal[crystalID].TdecayA = 5. * (numSamples - peakIdx);
						}
						else if (apdID == 1) {
							crystal[crystalID].WFpeakB = cwf[peakIdx];
							crystal[crystalID].TriseB = 5. * (peakIdx - 4);
							crystal[crystalID].TdecayB = 5. * (numSamples - peakIdx);
						}
						else {
							std::cout << "mu2e::CaloSpy::analyze eventNumber=" << (int)(event.event()) <<
								"Unknown SiPM id" << apdID << std::endl;
							exit(0);
						}

						//-------------------------------
						// Fill single readout histograms
						//-------------------------------

						// use DMAP to extract CalPoi
						CalPoi = 2 * crystalID + apdID;
						std::cout << "CalPoi: " << CalPoi << std::endl;

						if (CalPoi < 2 * nCryDisk) {                      // Two readouts per crystal
							OccupancyVec[CalPoi]++;

							///_hCaloOccupancy0->Fill(CalPoi);
							///_hMaxWaveForm0->Fill(CalPoi,cwf[peakIdx]);
							///_hTrise0->Fill(CalPoi,5.*(peakIdx-4));
							///_hTdecay0->Fill(CalPoi,5.*(numSamples-peakIdx));
							//std::cout << " Filling histos for Disk 0" << std::endl;
						}
						else {
							int DiskPoi = CalPoi - 2 * nCryDisk;
							///_hCaloOccupancy1->Fill(DiskPoi);
							///_hMaxWaveForm1->Fill(DiskPoi,cwf[peakIdx]);
							///_hTrise1->Fill(DiskPoi,5.*(peakIdx-4));
							///_hTdecay1->Fill(DiskPoi,5.*(numSamples-peakIdx));
							//std::cout << " Filling histos for Disk 1" << std::endl;
						}

						// Text format: timestamp crystalID roID time nsamples samples...
						// Example: 1 201 402 660 18 0 0 0 0 1 17 51 81 91 83 68 60 58 52 42 33 23 16
						std::cout << "GREPMECAL: " << hdr.GetTimestamp().GetTimestamp(true) << " ";
						std::cout << crystalID << " ";
						std::cout << apdID << " ";
						std::cout << hitPkt.Time << " ";
						std::cout << cwf.size() << " ";
						for (size_t i = 0; i < cwf.size(); i++) {
							std::cout << cwf[i];
							if (i < cwf.size() - 1) {
								std::cout << " ";
							}
						}
						std::cout << std::endl;
					} // End debug output

				} // End loop over readout channels in DataBlock

			} // End Cal Mode

		} // End loop over DataBlocks within fragment 

	} // Close loop over fragments

	//  }  // Close loop over the TRK and CAL collections

	if (diagLevel_ > 0) {
		std::cout << "mu2e::CaloSpy::analyze exiting eventNumber=" << (int)(event.event()) << " / timestamp=" << (int)eventNumber << std::endl;
	}

}  // analyze()

// ======================================================================
// End Job - Normalize histograms
// ============================================================================

void CaloSpy::endJob() {
	std::cout << "CaloSpy: Normalizing histos to number of events:" << Ntot << std::endl;
	// Temporary, waiting for ROOT fixing
	for (int iCryId = 0; iCryId < nCryDisk; iCryId++) {
		OccupancyVec[iCryId] = OccupancyVec[iCryId] / Ntot;
		std::cout << "Disk CryId Occupancy: 0 " << iCryId << " " << OccupancyVec[iCryId] << std::endl;
	}
	/*
	// Get Ntot from art?
	if( Ntot>0 ){
	  _hCaloOccupancy0->Sumw2();
	  _hCaloOccupancy1->Sumw2();
	  _hCaloOccupancy0->Scale(1./Ntot);
	  _hCaloOccupancy1->Scale(1./Ntot);
	}
	*/
}

// ============================================================================

DEFINE_ART_MODULE(CaloSpy)

// ======================================================================
