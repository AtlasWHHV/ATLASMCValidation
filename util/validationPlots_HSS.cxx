//
// Dump out plots for LLP MC validation
//

#include <iostream>

using namespace std;
//#include "MCValidation/template_access.h"
#include "MCValidation/two_particle_plots.h"
#include "MCValidation/standard_p_plots.h"
#include "MCValidation/lifetime_plots.h"
#include "MCValidation/truth_helpers.h"

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/tools/ReturnCheck.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODTruth/TruthEventContainer.h"
#include "AsgTools/StatusCode.h"

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"

using namespace xAOD;

// Config
const char *APP_NAME = "validationPlots";
const char *OutputFile = "validation.root";

// Parse the command lnie
TChain *getFilesFromCommandLine(int argc, char* argv[]);

// You know it!
void badNews(StatusCode &code);

int main (int argc, char *argv[]) {
  
  // Get the xAOD system up and running
  RETURN_CHECK (APP_NAME, xAOD::Init(APP_NAME));

  // Parse the input files.
  auto input = getFilesFromCommandLine(argc, argv);
  if (input == nullptr)
    return 1;

  // Create the global TEvent object, and connect it to
  // the input chain of files.
  TEvent event(TEvent::kBranchAccess);
  RETURN_CHECK(APP_NAME, event.readFrom(input));

  // Output file.
  auto *fhis = TFile::Open(OutputFile, "RECREATE");
  if (!fhis->IsOpen()) {
    return 1;
  }

  // Book the histos
  lifetime_plots     all ("all", "all ");
  standard_p_plots   hs ("hs", "#h_{s} ");
  standard_p_plots   hhiggs ("higgs", "Higgs ");
  two_particle_plots twohs ("twoHS", "Two HSs ");
  two_particle_plots twojets ("TwoJets", "Two jets ");

  //auto mcevents = looper<TruthEventContainer>("TruthEvents");
  //auto mcparticles = mcevents.SelectMany<TruthParticle>([] (const TruthEvent * evt) { return truth_as_range(evt); });
  //auto vpions = mcparticles.Where([] (const TruthParticle *p) { return pp->pdgId() == 36;});
  //vpions.Plot1D<TH1F>("hv_pt1", "HV Pion p_{T}; p_{T} [GeV]", 100, 0.0, 200.0,
  //[] (const TruthParticle *p, TH1F *h) { h->Fill(p->pt()/1000.0);});

  // Loop over each entry.
  auto nEntries = input->GetEntries();
  for (decltype(nEntries) i = 0; i < nEntries; i++) {
    event.getEntry(i);
    //mcevents.ProcessEvents (event);

    // Get the truth info
    const TruthEventContainer *truth = nullptr;
    RETURN_CHECK (APP_NAME, event.retrieve(truth, "TruthEvents"));
    bool isHiggs62 = false;
    // Loop over all the truth particles in there
    for (auto evt : *truth) {
      for (auto p : truth_as_range(evt)) {
	if (p != nullptr) {
	  all.Process(p);
	  if (p->pdgId() == 35) {
	    hs.Process(p);
	    twohs.addParticle(p);
	    if(p->nParents() > 1) std::cout << "event " << i << ": this scalar has " << p->nParents()  << " parents! " << std::endl;
	    if(i < 5){
	      std::cout << "h_s, particle status: " << p->status() << std::endl;
	      std::cout << "h_s has " << p->nChildren() << std::endl;
	      
	      for(unsigned int k=0; k < p->nChildren(); k++){
		std::cout <<" h_s child status, id: " << p->child(k)->status() << ", " << p->child(k)->pdgId() << std::endl;
	      }
	    }
	    
	    // ** Fill plots for two final state jets
	    for(unsigned int k=0; k < p->nChildren(); k++){
	      if ( fabs(p->child(k)->pdgId())==11 || fabs(p->child(k)->pdgId())==13 || fabs(p->child(k)->pdgId())==15 || 
		   (fabs(p->child(k)->pdgId())>0 && fabs(p->child(k)->pdgId())<=6) )
		twojets.addParticle(p->child(k));
	    }
	  }
	  if (p->pdgId() == 25 && p->status() == 62) {
	    hhiggs.Process(p);
            isHiggs62 = true; 
	  }

	}
      }
      all.EndOfEvent();
      hs.EndOfEvent();
      hhiggs.EndOfEvent();
      twohs.EndOfEvent();
      twojets.EndOfEvent();
    }
    if(!isHiggs62) std::cout << "no higgs with status 62 in event " << i << std::endl;
  }

  // Close and write file.
  fhis->Write();
  fhis->Close();

  return 0;
}

// Utilities

// Fail if something went wrong.
void badNews (StatusCode &code) {
  if (code.isFailure()) {
    throw runtime_error ("Failed call");
  }
}

TChain *getFilesFromCommandLine(int argc, char* argv[])
{
  if (argc < 2) {
    ::Error(APP_NAME, "Usage: %s <xAOD file> [xAOD file2]...", APP_NAME);
    return nullptr;
  }
  
  auto chain = new TChain("CollectionTree");
  for (int i = 1; i < argc; i++) {
    chain->Add(argv[i]);
  }

  return chain;
}
