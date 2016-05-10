// $Id$
//
// Emcal invariant mass analysis task. Derived from the EMCAL sample task
//
// Author: R. Ehlers, J. Mulligan 

#include <TClonesArray.h>
#include <THashList.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMath.h>

#include "AliVCluster.h"
#include "AliAODCaloCluster.h"
#include "AliESDCaloCluster.h"
#include "AliVTrack.h"
#include "AliLog.h"
#include "AliParticleContainer.h"
#include "AliClusterContainer.h"
#include "AliPicoTrack.h"

#include <THistManager.h>

#include "AliAnalysisTaskEmcalInvariantMass.h"

ClassImp(AliAnalysisTaskEmcalInvariantMass)

//________________________________________________________________________
AliAnalysisTaskEmcalInvariantMass::AliAnalysisTaskEmcalInvariantMass() : 
    AliAnalysisTaskEmcal("AliAnalysisTaskEmcalInvariantMass", kTRUE),
    /*fHistTracksPt(0),
    fHistClustersPt(0),
    fHistPtDEtaDPhiTrackClus(0),
    fHistPtDEtaDPhiClusTrack(0),
    fTracksCont(0),*/
    fInvariantMassHists(NULL),
    fCaloClustersCont(0)
{
    // Default constructor.

    /*fHistTracksPt       = new TH1*[fNcentBins];
    fHistClustersPt     = new TH1*[fNcentBins];

    for (Int_t i = 0; i < fNcentBins; i++) {
        fHistTracksPt[i] = 0;
        fHistClustersPt[i] = 0;
    }*/

    SetMakeGeneralHistograms(kTRUE);
}

//________________________________________________________________________
AliAnalysisTaskEmcalInvariantMass::AliAnalysisTaskEmcalInvariantMass(const char *name) : 
    AliAnalysisTaskEmcal(name, kTRUE),
    /*fHistTracksPt(0),
    fHistClustersPt(0),
    fHistPtDEtaDPhiTrackClus(0),
    fHistPtDEtaDPhiClusTrack(0),
    fTracksCont(0),*/
    fInvariantMassHists(NULL),
    fCaloClustersCont(0)
{
    // Standard constructor.

    /*fHistTracksPt       = new TH1*[fNcentBins];
    fHistClustersPt     = new TH1*[fNcentBins];

    for (Int_t i = 0; i < fNcentBins; i++) {
        fHistTracksPt[i] = 0;
        fHistClustersPt[i] = 0;
    }*/

    SetMakeGeneralHistograms(kTRUE);
}

//________________________________________________________________________
AliAnalysisTaskEmcalInvariantMass::~AliAnalysisTaskEmcalInvariantMass()
{
    // Destructor.
}

//________________________________________________________________________
void AliAnalysisTaskEmcalInvariantMass::UserCreateOutputObjects()
{
    // Create user output.
    AliAnalysisTaskEmcal::UserCreateOutputObjects();

    //fTracksCont       = GetParticleContainer(0);
    fCaloClustersCont = GetClusterContainer(0);
    //fTracksCont->SetClassName("AliVTrack");
    fCaloClustersCont->SetClassName("AliVCluster");

    //TString histname;

    fInvariantMassHists = new THistManager("InvariantMass");

    for (Int_t i = 0; i < fNcentBins; i++) {
        /*if (fParticleCollArray.GetEntriesFast()>0) {
          histname = "fHistTracksPt_";
          histname += i;
          fHistTracksPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins / 2, fMinBinPt, fMaxBinPt / 2);
          fHistTracksPt[i]->GetXaxis()->SetTitle("p_{T,track} (GeV/c)");
          fHistTracksPt[i]->GetYaxis()->SetTitle("counts");
          fOutput->Add(fHistTracksPt[i]);
          }*/

        if (fClusterCollArray.GetEntriesFast()>0) {
            fInvariantMassHists->CreateTH1(Form("hInvariantMassLow_%i", i), Form("Invariant Mass for centrality bin %i;p_{T};Entries", i), 10000, 0, 10);
            fInvariantMassHists->CreateTH1(Form("hInvariantMass_%i", i), Form("Invariant Mass for centrality bin %i;p_{T};Entries", i), 10000, 0, 100);
            
        }
    }

    Int_t bins[4] = {100, 50, 50, 50};
    Double_t xmin[4] = {0., -5, -5, 0.};
    Double_t xmax[4] = {100., 5, 5, 5000.};
    fInvariantMassHists->CreateTHnSparse("hMixedEvent", "Mixed Event Distributions;p_{T};#eta;#phi;Num Cl;", 4, bins, xmin, xmax);

    /*histname = "fHistPtDEtaDPhiTrackClus";
      fHistPtDEtaDPhiTrackClus = new TH3F(histname.Data(),Form("%s;#it{p}_{T}^{track};#Delta#eta;#Delta#varphi",histname.Data()),100,0.,100.,100,-0.1,0.1,100,-0.1,0.1);
      fOutput->Add(fHistPtDEtaDPhiTrackClus);*/

    /*histname = "fHistPtDEtaDPhiClusTrack";
    fHistPtDEtaDPhiClusTrack = new TH3F(histname.Data(),Form("%s;#it{p}_{T}^{clus};#Delta#eta;#Delta#varphi",histname.Data()),100,0.,100.,100,-0.1,0.1,100,-0.1,0.1);
    fOutput->Add(fHistPtDEtaDPhiClusTrack);*/

    fInvariantMassHists->CreateTH1("hClusterPt", "Cluster p_{T} Spectrum;p_{T};Entries", 150, 0, 150);

    fOutput->Add(fInvariantMassHists->GetListOfHistograms());
    PostData(1, fOutput); // Post data for ALL output slots > 0 here.
}

//________________________________________________________________________
Double_t AliAnalysisTaskEmcalInvariantMass::CalculateInvariantMass(const TLorentzVector & clusterOne, const TLorentzVector & clusterTwo)
{
    // Cosh and cos are an even functions, so the sign doesn't matter
    Double_t deltaEta = clusterOne.Eta() - clusterTwo.Eta();
    Double_t deltaPhi = clusterOne.Phi() - clusterTwo.Phi();
    Double_t invariantMass = 2*clusterOne.Pt()*clusterTwo.Pt()*(TMath::CosH(deltaEta) - TMath::Cos(deltaPhi));
    return TMath::Sqrt(invariantMass);
}

//________________________________________________________________________
Bool_t AliAnalysisTaskEmcalInvariantMass::FillHistograms()
{
    // Fill histograms.

    /*if (fTracksCont) {
        AliVTrack *track = static_cast<AliVTrack*>(fTracksCont->GetNextAcceptParticle(0)); 
        while(track) {
            fHistTracksPt[fCentBin]->Fill(track->Pt()); 
            track = static_cast<AliVTrack*>(fTracksCont->GetNextAcceptParticle());
        }
    }*/

    if (fCaloClustersCont) {
        /*AliVCluster *cluster = fCaloClustersCont->GetNextAcceptCluster(0); 
        while(cluster) {
            TLorentzVector nPart;
            cluster->GetMomentum(nPart, fVertex);
            fHistClustersPt[fCentBin]->Fill(nPart.Pt());
            cluster = fCaloClustersCont->GetNextAcceptCluster();
        }*/

        AliVCluster * clusterOne = 0;
        AliVCluster * clusterTwo = 0;
        TLorentzVector vectorOne;
        TLorentzVector vectorTwo;
        Int_t numClusters = fCaloClustersCont->GetNClusters();
        for (Int_t i = 0; i < numClusters; i++)
        {
            clusterOne = fCaloClustersCont->GetCluster(i);
            if (fCaloClustersCont->AcceptCluster(clusterOne) == kFALSE)
            {
                continue;
            }
            fCaloClustersCont->GetMomentum(vectorOne, i);

            // Fill pt spectrum
            fInvariantMassHists->FillTH1("hClusterPt", vectorOne.Pt());
            
            // Fill mixed event histogram
            Double_t val[4] = {vectorOne.Pt(), vectorOne.Eta(), vectorOne.Phi(), numClusters};
            fInvariantMassHists->FillTHnSparse("hMixedEvent", val);
            //fInvariantMassHists->FillTHnSparse(Form("hMixedEvent_%i", fCentBin), val);
            
            // Fill invariant mass histogram
            for (Int_t j = i+1; j < numClusters; j++)
            {
                clusterTwo = fCaloClustersCont->GetCluster(j);
                if (fCaloClustersCont->AcceptCluster(clusterTwo) == kFALSE)
                {
                    continue;
                }
                // Both clusters are accepted. Now we calculate the invariant mass 
                fCaloClustersCont->GetMomentum(vectorTwo, j);

                // Fill invariant mass histogram
                fInvariantMassHists->FillTH1(Form("hInvariantMassLow_%i", fCentBin), CalculateInvariantMass(vectorOne, vectorTwo));
                fInvariantMassHists->FillTH1(Form("hInvariantMass_%i", fCentBin), CalculateInvariantMass(vectorOne, vectorTwo));
            }

            // Reset the vector
            vectorOne.SetPxPyPzE(0,0,0,0);
            vectorTwo.SetPxPyPzE(0,0,0,0);
        }
    }

    //CheckClusTrackMatching();

    return kTRUE;
}

/*
//________________________________________________________________________
void AliAnalysisTaskEmcalInvariantMass::CheckClusTrackMatching()
{

    if(!fTracksCont || !fCaloClustersCont)
        return;

    Double_t deta = 999;
    Double_t dphi = 999;

    //Get closest cluster to track
    AliVTrack *track = static_cast<AliVTrack*>(fTracksCont->GetNextAcceptParticle(0)); 
    while(track) {
        //Get matched cluster
        Int_t emc1 = track->GetEMCALcluster();
        if(fCaloClustersCont && emc1>=0) {
            AliVCluster *clusMatch = fCaloClustersCont->GetCluster(emc1);
            if(clusMatch) {
                AliPicoTrack::GetEtaPhiDiff(track, clusMatch, dphi, deta);
                fHistPtDEtaDPhiTrackClus->Fill(track->Pt(),deta,dphi);
            }
        }
        track = static_cast<AliVTrack*>(fTracksCont->GetNextAcceptParticle());
    }

    //Get closest track to cluster
    AliVCluster *cluster = fCaloClustersCont->GetNextAcceptCluster(0); 
    while(cluster) {
        TLorentzVector nPart;
        cluster->GetMomentum(nPart, fVertex);
        fHistClustersPt[fCentBin]->Fill(nPart.Pt());

        //Get matched track
        AliVTrack *mt = NULL;      
        AliAODCaloCluster *acl = dynamic_cast<AliAODCaloCluster*>(cluster);
        if(acl) {
            if(acl->GetNTracksMatched()>1)
                mt = static_cast<AliVTrack*>(acl->GetTrackMatched(0));
        }
        else {
            AliESDCaloCluster *ecl = dynamic_cast<AliESDCaloCluster*>(cluster);
            Int_t im = ecl->GetTrackMatchedIndex();
            if(fTracksCont && im>=0) {
                mt = static_cast<AliVTrack*>(fTracksCont->GetParticle(im));
            }
        }
        if(mt) {
            AliPicoTrack::GetEtaPhiDiff(mt, cluster, dphi, deta);
            fHistPtDEtaDPhiClusTrack->Fill(nPart.Pt(),deta,dphi);*/

            /* //debugging
               if(mt->IsEMCAL()) {
               Int_t emc1 = mt->GetEMCALcluster();
               Printf("current id: %d  emc1: %d",fCaloClustersCont->GetCurrentID(),emc1);
               AliVCluster *clm = fCaloClustersCont->GetCluster(emc1);
               AliPicoTrack::GetEtaPhiDiff(mt, clm, dphi, deta);
               Printf("deta: %f dphi: %f",deta,dphi);
               }
               */
        /*
        }
        cluster = fCaloClustersCont->GetNextAcceptCluster();
    }
}
*/

//________________________________________________________________________
void AliAnalysisTaskEmcalInvariantMass::ExecOnce() {

    AliAnalysisTaskEmcal::ExecOnce();

    //if (fTracksCont && fTracksCont->GetArray() == 0) fTracksCont = 0;
    if (fCaloClustersCont && fCaloClustersCont->GetArray() == 0) fCaloClustersCont = 0;

}

//________________________________________________________________________
Bool_t AliAnalysisTaskEmcalInvariantMass::Run()
{
    // TEMP
    //Printf("Inside invariant mass task!");
    // Run analysis code here, if needed. It will be executed before FillHistograms().

    return kTRUE;  // If return kFALSE FillHistogram() will NOT be executed.
}

//________________________________________________________________________
void AliAnalysisTaskEmcalInvariantMass::Terminate(Option_t *) 
{
    // Called once at the end of the analysis.
}
