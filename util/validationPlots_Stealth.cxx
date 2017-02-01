//
// Dump out plots for LLP MC validation
//

#include <iostream>

using namespace std;
//#include "MCValidation/template_access.h"
#include "MCValidation/two_particle_plots.h"
#include "MCValidation/standard_p_plots.h"
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
  standard_p_plots sf ("sf", "singlino");
  standard_p_plots ss ("ss", "singlet");
  standard_p_plots go ("go", "gluino ");

  two_particle_plots twoss ("two_ss", "Two Singlet ");
  two_particle_plots twosf ("two_sf", "Two Singlinos ");

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
	  if (p->pdgId() == 3000001 && p->nChildren() == 2) {
	    sf.Process(p);
	    twosf.addParticle(p);
	    if(i < 10){
		 std::cout << "singlino, particle status: " << p->status() << std::endl;
		std::cout << "singlino has " << p->nChildren() << std::endl;
	    
	      for(unsigned int k=0; k < p->nChildren(); k++){
		 std::cout <<" singlino child: " << p->child(k)->status() << std::endl;
		 std::cout <<" singlino child: " << p->child(k)->pdgId() << std::endl;
	       }
            }
	    //if(p->nParents() > 1) std::cout << "event " << i << ": this scalar has " << p->nParents()  << " parents! " << std::endl;
	  }
	  if (p->pdgId() == 3000002 && p->status() == 22) {
	    ss.Process(p);
	    twoss.addParticle(p);
	    if(i < 10) std::cout << "singlet, particle status: " << p->status() << std::endl;
	     
	  }
	  if (p->pdgId() == 1000021 && p->status()==62 ) {
	    if(i < 10) std::cout << "gluino, particle status: " << p->status() << std::endl;
	    go.Process(p);
            isHiggs62 = true;
	  }
	}
      }
      ss.EndOfEvent();
      sf.EndOfEvent();
      go.EndOfEvent();
      twosf.EndOfEvent();
      twoss.EndOfEvent();
    }
    if(!isHiggs62) std::cout << "no gluino with status 62 in event " << i << std::endl;
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
