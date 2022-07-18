#ifndef _CaloTestHistos_h_
#define _CaloTestHistos_h_


#include "otsdaq/NetworkUtilities/TCPPublishServer.h"
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include <string>
#include <TH1F.h>

namespace ots
{

class CaloTestHistos
{
    public:
	CaloTestHistos(const std::string Title) : _title(Title){};
        CaloTestHistos(){};
	virtual ~CaloTestHistos(void){};
        struct  summaryInfoHist_  {
            TH1F *_FirstCaloHist;
            summaryInfoHist_() {
            _FirstCaloHist = NULL;
            }
         };
        const std::string _title;
        summaryInfoHist_ CaloTest;

        void BookHistos(art::ServiceHandle<art::TFileService>  tfs){
            art::TFileDirectory CaloDir = tfs->mkdir("CaloTestHistos");
            this->CaloTest._FirstCaloHist = CaloDir.make<TH1F>("hCalo","hCalo",100,0,500);
        }

        void BookHistos(TDirectory *dir){
            dir->mkdir("CaloTestHistos", "CaloTestHistos");
            this->CaloTest._FirstCaloHist = new TH1F("hCalo","hCalo",100,0,500);
        }
};

}

#endif
