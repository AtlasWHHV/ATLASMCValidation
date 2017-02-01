#ifndef __lifetime_plots__
#define __lifetime_plots__

#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"

#include "TH1F.h"
#include "TH2F.h"

#include <string>

class lifetime_plots {
 public:
  // Book the plots
  lifetime_plots (const std::string &name, const std::string &title, int maxN = 10)
    {
      _deltaT = new TH1F((name + "DeltaT").c_str(), (title + " Delta t; t_{dec} - t_{prod} [ns]").c_str(),260,-6,20);
    }

  // Fill the plots.
  void Process(const xAOD::TruthParticle *p) {
    if (p->hasProdVtx() && p->hasDecayVtx()) {
      _deltaT->Fill(p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001);
      if ( (p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001)<0 ) {
	cout << " - Decay = " << p->decayVtx()->v4().T()*0.001 << endl;
	cout << " - Prod  = " << p->prodVtx()->v4().T()*0.001 << endl;
	cout << "         -- diff = " << p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001 << endl;	
      }
    }
  }
  void EndOfEvent()
  {
    _counter = 0;
  }
 private:
  int _counter;
  TH1F *_deltaT;

};

#endif
