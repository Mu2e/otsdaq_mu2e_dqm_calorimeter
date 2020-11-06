#ifndef _ots_DQMCaloHistoConsumer_h_
#define _ots_DQMCaloHistoConsumer_h_

#include "otsdaq/DataManager/DQMHistosConsumerBase.h"
#include "otsdaq/Configurable/Configurable.h"
#include "otsdaq-dqm-calorimeter/ArtModules/ProtoTypeHistos.h"
#include <string>

class TFile;
class TDirectory;
class TH1F;
class TTree;

namespace ots
{

	class DQMCaloHistoConsumer : public DQMHistosConsumerBase, public Configurable
	{
		public:
	  		DQMCaloHistoConsumer(std::string supervisorApplicationUID, std::string bufferUID, std::string processorUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& configurationPath);
			virtual ~DQMCaloHistoConsumer(void);

			void startProcessingData (std::string runNumber) override;
			void stopProcessingData  (void) override;
			void pauseProcessingData (void) override;
			void resumeProcessingData(void) override;
			void load                (std::string fileName){;}

		private:
			bool workLoopThread(toolbox::task::WorkLoop* workLoop);
			void fastRead(void);
			void slowRead(void);
	
			//For fast read
			std::string*                       dataP_;
			std::map<std::string,std::string>* headerP_;

			bool                               saveFile_; //yes or no
			std::string                        filePath_;
			std::string                        radixFileName_;

			TH1F*                              hGauss_;//TODO need to add our own class here
			TTree*                             testTree_;
			ProtoTypeHistos                    testHistos_;
	};
}

#endif
