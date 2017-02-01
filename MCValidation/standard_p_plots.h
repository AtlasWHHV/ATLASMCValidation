#ifndef __standard_p_plots__
#define __standard_p_plots__

#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"

#include "TH1F.h"
#include "TH2F.h"

#include <string>

class standard_p_plots {
 public:
  // Book the plots
  standard_p_plots (const std::string &name, const std::string &title, int maxN = 10)
    {
      _mass = new TH1F((name + "mass").c_str(), (title + "mass; mass [GeV]").c_str(), 6000, 0.0, 6000.0);
      _pz = new TH1F((name + "pz").c_str(), (title + "|p_{z}|; |p_{z}| [GeV]").c_str(), 200, 0.0, 3000.0);
      _pt = new TH1F((name + "pt").c_str(), (title + "p_{T}; p_{T} [GeV]").c_str(), 250, 0.0, 2000.0);
      _eta = new TH1F((name + "eta").c_str(), (title + "\\eta; \\eta").c_str(), 200, -5.0, 5.0);
      _phi = new TH1F((name + "phi").c_str(), (title + "\\phi; \\phi [rad]").c_str(), 200, -3.15, 3.15);
      _E = new TH1F((name + "E").c_str(), (title + "E; E [GeV]").c_str(), 300, 0.0, 4500.0);
      _N = new TH1F((name + "N").c_str(), (title + "N; N").c_str(), maxN, 0, maxN);
      _status = new TH1F((name + "StatusCode").c_str(), (title + "status; status").c_str(), 100, 0, 100);
      _decayTime = new TH1F((name + "DecayTime").c_str(), (title + "Time; Time [ns]").c_str(), 300, 0, 30);
      _ctau = new TH1F((name + "ctau").c_str(), (title + "c*tau; c*tau [m]").c_str(), 600, 0, 30);
      _lxy = new TH1F((name + "Lxy").c_str(), (title + " L_{xy}; L_{xy} [m]").c_str(), 200, 0.0, 20);
      _lxy_B = new TH1F((name + "Lxy_Barr").c_str(), (title + " L_{xy} barrel; L_{xy} barrel [m]").c_str(), 200, 0.0, 20);
      _lxy_E = new TH1F((name + "Lxy_EndC").c_str(), (title + " L_{xy} end-caps; L_{xy} end-caps [m]").c_str(), 200, 0.0, 20);
      _lz = new TH1F((name + "Lz").c_str(), (title + " L_{z}; L_{z} [m]").c_str(), 200, 0.0, 20);
      _lz_B = new TH1F((name + "Lz_Barr").c_str(), (title + " L_{z} barrel; L_{z} barrel [m]").c_str(), 200, 0.0, 20);
      _lz_E = new TH1F((name + "Lz_EndC").c_str(), (title + " L_{z} end-caps; L_{z} end-caps [m]").c_str(), 200, 0.0, 20);
      _beta = new TH1F((name + "Beta").c_str(), (title + " Beta; #beta").c_str(), 220, 0.0, 1.1);
      _counter = 0;
      _BRs = new TH1F((name + "BR").c_str(), (title + " Braching ratios; branching ratio [pdgid]").c_str(),50,0,50);
      _deltaT = new TH1F((name + "DeltaT").c_str(), (title + " Delta t; t_{dec} - t_{prod} [ns]").c_str(),260,-6,20);
      _evtsID = new TH2F((name + "EvtsID").c_str(), (title + " Events in the ID").c_str(),40,0,0.4,28,0,0.28);
      _evtsMS_B = new TH2F((name + "EvtsMS_B").c_str(), (title + " Events in the MS (barrel)").c_str(),60,0,15,20,3,8);
      _evtsMS_E = new TH2F((name + "EvtsMS_E").c_str(), (title + " Events in the MS (end-cap)").c_str(),20,5,15,20,0,10);
      _evtsID_lxy = new TH1F((name + "EvtsID_Lxy").c_str(), (title + " Events in the ID vs Lxy; L_{xy} [m]").c_str(),28,0,0.28);
      _evtsMS_B_lxy = new TH1F((name + "EvtsMS_B_Lxy").c_str(), (title + " Events in the MS (barrel) vs Lxy; L_{xy} [m]").c_str(),20,3,8);
      _evtsMS_E_lz = new TH1F((name + "EvtsMS_E_Lz").c_str(), (title + " Events in the MS (end-cap) vs Lz; L_{z} [m]").c_str(),20,5,15);
    }

  // Fill the plots.
  void Process(const xAOD::TruthParticle *p) {
    _mass->Fill(p->m()*0.001);
    _pz->Fill(TMath::Abs(p->pz())*0.001);
    _pt->Fill(p->pt()*0.001);
    _eta->Fill(p->eta());
    _phi->Fill(p->phi());
    _E->Fill(p->e()*0.001);
    _beta->Fill(p->p4().Beta());
    _counter++;
    _status->Fill(p->status());
    if( p->nChildren() == 2){ 
      if( TMath::Abs(p->child(0)->pdgId()) == TMath::Abs(p->child(1)->pdgId()) ){ 
     	_BRs->Fill(TMath::Abs(p->child(0)->pdgId())); 
      } else std::cout << "pdgIds aren't equal! Decay is to: " << p->child(0)->pdgId() <<", " << p->child(1)->pdgId() << std::endl; 
    } else std::cout << "doesn't have two children!" << std::endl; 
    // Lxyz means getting the parent verticies
    //cout << "p: " << p->hasProdVtx() << " d: " << p->hasDecayVtx() << endl;
    if (p->hasProdVtx() && p->hasDecayVtx()) {
      auto vp = p->prodVtx()->v4();
      auto vd = p->decayVtx()->v4();

      auto delta = vp-vd;
      auto dist_xy = delta.Perp();

      _lxy->Fill(dist_xy*0.001);
      if (fabs(p->eta())< 1.5) _lxy_B->Fill(dist_xy*0.001);
      if (fabs(p->eta())>=1.5) _lxy_E->Fill(dist_xy*0.001);
      _lz->Fill(delta.Z()*0.001);
      if (fabs(p->eta())< 1.0) _lz_B->Fill(delta.Z()*0.001);
      if (fabs(p->eta())>=1.0) _lz_E->Fill(delta.Z()*0.001);
      _decayTime->Fill(p->decayVtx()->v4().T()*0.001);
      _ctau->Fill(p->decayVtx()->v4().T()*0.001/p->p4().Gamma());
      _deltaT->Fill(p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001);
      //std::cout << "  deltaT = " << p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001 << std::endl;
      //if ( p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001 ) cout << "                                                                                                                 deltaT = " << p->decayVtx()->v4().T()*0.001 - p->prodVtx()->v4().T()*0.001 << endl;
      if ( dist_xy*0.001 < 0.28 && fabs(delta.Z()*0.001)<0.4 && fabs(p->eta())<2.5 )   _evtsID->Fill(fabs(delta.Z()*0.001),dist_xy*0.001);
      if ( dist_xy*0.001 > 3    && dist_xy*0.001 < 8 && fabs(p->eta())<1.0 )           _evtsMS_B->Fill(fabs(delta.Z()*0.001),dist_xy*0.001);
      if ( dist_xy*0.001 < 10 && fabs(delta.Z()*0.001)>5 && fabs(delta.Z()*0.001)<15 ) _evtsMS_E->Fill(fabs(delta.Z()*0.001),dist_xy*0.001);
      if ( dist_xy*0.001 < 0.28 && fabs(delta.Z()*0.001)<0.4 && fabs(p->eta())<2.5 )   _evtsID_lxy->Fill(dist_xy*0.001);
      if ( dist_xy*0.001 > 3    && dist_xy*0.001 < 8 && fabs(p->eta())<1.0 )           _evtsMS_B_lxy->Fill(dist_xy*0.001);
      if ( dist_xy*0.001 < 10 && fabs(delta.Z()*0.001)>5 && fabs(delta.Z()*0.001)<15 ) _evtsMS_E_lz->Fill(fabs(delta.Z()*0.001));
    }

  }
  void EndOfEvent()
  {
    _N->Fill(_counter);
    _counter = 0;
  }
 private:
  int _counter;
  TH1F *_N;
  TH1F *_status;
  TH1F *_mass;
  TH1F *_pz;
  TH1F *_pt;
  TH1F *_eta;
  TH1F *_phi;
  TH1F *_E;
  TH1F *_lxy;
  TH1F *_lxy_B;
  TH1F *_lxy_E;
  TH1F *_lz;
  TH1F *_lz_B;
  TH1F *_lz_E;
  TH1F *_beta;
  TH1F *_decayTime;
  TH1F *_ctau;
  TH1F *_BRs;
  TH1F *_deltaT;
  TH2F *_evtsID;
  TH2F *_evtsMS_B;
  TH2F *_evtsMS_E;
  TH1F *_evtsID_lxy;
  TH1F *_evtsMS_B_lxy;
  TH1F *_evtsMS_E_lz;

};

#endif
