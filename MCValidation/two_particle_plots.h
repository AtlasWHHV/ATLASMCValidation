#ifndef __two_particle_plots__
#define __two_particle_plots__

#include "xAODTruth/TruthParticle.h"
#include "TH1.h"

#include <string>

class two_particle_plots {
 public:

  two_particle_plots(const std::string &name, const std::string &title)
  {
    _deltaR       = new TH1F ((name + "DR"  ).c_str(),     (title + "\\Delta R; \\Delta R").c_str(), 100, 0.0, 5.0);
    _deltaPhi     = new TH1F ((name + "Dphi").c_str(),     (title + "|\\Delta \\phi|; |\\Delta \\phi|").c_str(), 100, 0, 3.5);
    _deltaEta     = new TH1F ((name + "Deta").c_str(),     (title + "|\\Delta \\eta|; |\\Delta \\eta|").c_str(), 100, 0, 3.0 );
    _deltaR_e     = new TH1F ((name + "DR_e"  ).c_str(),   (title + "\\Delta R  electrons; \\Delta R  electrons").c_str(), 100, 0.0, 5.0);
    _deltaPhi_e   = new TH1F ((name + "Dphi_e").c_str(),   (title + "|\\Delta \\phi|  electrons; |\\Delta \\phi|  electrons").c_str(), 100, 0, 3.5);
    _deltaEta_e   = new TH1F ((name + "Deta_e").c_str(),   (title + "|\\Delta \\eta|  electrons; |\\Delta \\eta|  electrons").c_str(), 100, 0, 3.0 );
    _deltaR_mu    = new TH1F ((name + "DR_mu"  ).c_str(),  (title + "\\Delta R  muons; \\Delta R  muons").c_str(), 100, 0.0, 5.0);
    _deltaPhi_mu  = new TH1F ((name + "Dphi_mu").c_str(),  (title + "|\\Delta \\phi|  muons; |\\Delta \\phi|  muons").c_str(), 100, 0, 3.5);
    _deltaEta_mu  = new TH1F ((name + "Deta_mu").c_str(),  (title + "|\\Delta \\eta|  muons; |\\Delta \\eta|  muons").c_str(), 100, 0, 3.0 );
    _deltaR_tau   = new TH1F ((name + "DR_tau"  ).c_str(), (title + "\\Delta R  taus; \\Delta R  taus").c_str(), 100, 0.0, 5.0);
    _deltaPhi_tau = new TH1F ((name + "Dphi_tau").c_str(), (title + "|\\Delta \\phi|  taus; |\\Delta \\phi|  taus").c_str(), 100, 0, 3.5);
    _deltaEta_tau = new TH1F ((name + "Deta_tau").c_str(), (title + "|\\Delta \\eta|  taus; |\\Delta \\eta|  taus").c_str(), 100, 0, 3.0 );
  }

  void addParticle (const xAOD::TruthParticle *p)
  {
    _stash.push_back(p);
  }

  // This is where the work happens
  void EndOfEvent()
  {
    if (_stash.size() < 2) {
      cout << "At end of event, not enough particles" << endl;
    }

    bool electrons=false;
    if ( (_stash[0]->pdgId()==13 && _stash[1]->pdgId()==-13) || (_stash[0]->pdgId()==-13 && _stash[1]->pdgId()==13) ) 
      electrons=true;
    bool muons=false;
    if ( (_stash[0]->pdgId()==11 && _stash[1]->pdgId()==-11) || (_stash[0]->pdgId()==-11 && _stash[1]->pdgId()==11) ) 
      muons=true;
    bool taus=false;
    if ( (_stash[0]->pdgId()==15 && _stash[1]->pdgId()==-15) || (_stash[0]->pdgId()==-15 && _stash[1]->pdgId()==15) ) 
      taus=true;
    
    // Calc the Delta R
    auto h1 = _stash[0]->p4();
    auto h2 = _stash[1]->p4();
    _deltaR      ->Fill(h1.DeltaR(h2));
    _deltaPhi    ->Fill(h1.DeltaPhi(h2));
    _deltaEta    ->Fill(fabs(h1.Eta()-h2.Eta()));
    if ( electrons ) _deltaR_e    ->Fill(h1.DeltaR(h2));
    if ( electrons ) _deltaPhi_e  ->Fill(h1.DeltaPhi(h2));
    if ( electrons ) _deltaEta_e  ->Fill(fabs(h1.Eta()-h2.Eta()));
    if ( muons )     _deltaR_mu   ->Fill(h1.DeltaR(h2));
    if ( muons )     _deltaPhi_mu ->Fill(h1.DeltaPhi(h2));
    if ( muons )     _deltaEta_mu ->Fill(fabs(h1.Eta()-h2.Eta()));
    if ( taus )      _deltaR_tau  ->Fill(h1.DeltaR(h2));
    if ( taus )      _deltaPhi_tau->Fill(h1.DeltaPhi(h2));
    if ( taus )      _deltaEta_tau->Fill(fabs(h1.Eta()-h2.Eta()));
    _stash.clear();
  }

 private:
  vector<const xAOD::TruthParticle *> _stash;
  TH1F *_deltaR;
  TH1F *_deltaPhi;
  TH1F *_deltaEta;
  TH1F *_deltaR_e;
  TH1F *_deltaPhi_e;
  TH1F *_deltaEta_e;
  TH1F *_deltaR_mu;
  TH1F *_deltaPhi_mu;
  TH1F *_deltaEta_mu;
  TH1F *_deltaR_tau;
  TH1F *_deltaPhi_tau;
  TH1F *_deltaEta_tau;
};

#endif
