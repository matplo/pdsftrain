#ifndef ALIANALYSISTASKEMCALINVARIANTMASS_H
#define ALIANALYSISTASKEMCALINVARIANTMASS_H

// $Id$

//class TH1;
//class TH2;
//class TH3;
class TLorentzVector;
class AliParticleContainer;
class AliClusterContainer;
class THistManager;

#include "AliAnalysisTaskEmcal.h"

class AliAnalysisTaskEmcalInvariantMass  : public AliAnalysisTaskEmcal {
 public:

  AliAnalysisTaskEmcalInvariantMass();
  AliAnalysisTaskEmcalInvariantMass(const char *name);
  virtual ~AliAnalysisTaskEmcalInvariantMass();

  void                        UserCreateOutputObjects();
  void                        Terminate(Option_t *option);

 protected:
  void                        ExecOnce();
  Bool_t                      FillHistograms()   ;
  Bool_t                      Run()              ;
  //void                        CheckClusTrackMatching();
  Double_t                    CalculateInvariantMass(const TLorentzVector & clusterOne, const TLorentzVector & clusterTwo);

  // General histograms
  //TH1                       **fHistTracksPt;            //!Track pt spectrum
  //TH1                       **fHistClustersPt;          //!Cluster pt spectrum
  //TH3                        *fHistPtDEtaDPhiTrackClus; //!track pt, delta eta, delta phi to matched cluster
  //TH3                        *fHistPtDEtaDPhiClusTrack; //!cluster pt, delta eta, delta phi to matched track
  THistManager                 *fInvariantMassHists;      //!<!Contains invariant mass hists

  //AliParticleContainer       *fTracksCont;                 //!Tracks
  AliClusterContainer        *fCaloClustersCont;           //!Clusters  

 private:
  AliAnalysisTaskEmcalInvariantMass(const AliAnalysisTaskEmcalInvariantMass&);            // not implemented
  AliAnalysisTaskEmcalInvariantMass &operator=(const AliAnalysisTaskEmcalInvariantMass&); // not implemented

  ClassDef(AliAnalysisTaskEmcalInvariantMass, 1) // emcal invariant mass analysis task
};
#endif
