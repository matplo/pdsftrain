#include <algorithm>
#include <vector>

#include <TH1.h>
#include <TMath.h>
#include <TList.h>
#include <TVector3.h>

#include "AliESDtrackCuts.h"
#include "AliESDtrack.h"
#include "AliMCEvent.h"
#include "AliStack.h"
#include "TTreeStream.h"

#include "TwoParticleAnalysis.h"

ClassImp(TwoParticleAnalysis)

TwoParticleAnalysis::TwoParticleAnalysis():
		AliAnalysisTaskSE(),
		fTrackCuts(NULL),
		fHistos(NULL),
		fEventCount(0),
		fTreeStream(NULL)
{
}

TwoParticleAnalysis::TwoParticleAnalysis(const char *name):
		AliAnalysisTaskSE(name),
		fTrackCuts(NULL),
		fHistos(NULL),
		fEventCount(0),
		fTreeStream(0)
{
	DefineOutput(1, TList::Class());
}

TwoParticleAnalysis::~TwoParticleAnalysis(){
	if(fTreeStream) delete fTreeStream;
}

void TwoParticleAnalysis::UserCreateOutputObjects() {
  fTrackCuts = AliESDtrackCuts::GetStandardITSTPCTrackCuts2011(true, 1);
	fTrackCuts->DefineHistograms(kRed);
	fTrackCuts->SetName("Standard Track cuts");
	fTrackCuts->SetMinNCrossedRowsTPC(120);
	fTrackCuts->SetMaxDCAToVertexXYPtDep("0.0182+0.0350/pt^1.01");
	fTrackCuts->DefineHistograms(kRed);

	fHistos = new TList;
	fHistos->SetOwner(kTRUE);
	fHistos->Add((fEventCount = new TH1F("hEventCount", "This histogram is so stupid", 1, 0.5, 1.5)));
	fHistos->Add(fTrackCuts);


	fTreeStream = new TTreeSRedirector("trackingDebug.root", "recreate");
	PostData(1, fHistos);
}

void TwoParticleAnalysis::UserExec(Option_t *opt){
	if(!MCEvent()) printf("MC event not available\n");
	if(!InputEvent()) printf("Rec event not available\n");
	fEventCount->Fill(1.);

	AliVParticle *part = NULL;
	std::vector<ParticleStruct> particles;
	for(int ipart = 0; ipart < MCEvent()->GetNumberOfTracks(); ++ipart){
		part = MCEvent()->GetTrack(ipart);
		if(!MCEvent()->Stack()->IsPhysicalPrimary(ipart)) continue;
		if(!part->Charge()) continue;
		particles.push_back(ParticleStruct(part->Pt(), part->Eta(), part->Phi(), part->GetLabel()));
	}

	std::sort(particles.begin(), particles.end());
	ParticleStruct &leadingpart = particles[particles.size()-1], &subleadingpart = particles[particles.size()-2];

	AliESDtrack *track = NULL;
	for(int itrk = 0; itrk < InputEvent()->GetNumberOfTracks(); itrk++){
		track = static_cast<AliESDtrack *>(InputEvent()->GetTrack(itrk));
		int tracklabel = TMath::Abs(track->GetLabel());
		if(tracklabel != TMath::Abs(leadingpart.Label()) || tracklabel != TMath::Abs(subleadingpart.Label())) continue;
		if(!fTrackCuts->AcceptTrack(track)) continue;
		if(tracklabel == TMath::Abs(leadingpart.Label())) leadingpart.SetReconstructed();
		else subleadingpart.SetReconstructed();
	}

	// fill tree
	TVector3 lv, sv;
	Double_t 	ld[3] = {leadingpart.Pt(), leadingpart.Eta(), leadingpart.Phi()},
				 		sd[3] = {subleadingpart.Pt(), subleadingpart.Eta(), subleadingpart.Phi()};
	lv.SetPtEtaPhi(leadingpart.Pt(), leadingpart.Eta(), leadingpart.Phi());
	sv.SetPtEtaPhi(subleadingpart.Pt(), subleadingpart.Eta(), subleadingpart.Phi());
	Double_t dr = lv.DeltaR(sv),
	deta = subleadingpart.Eta() - leadingpart.Eta(),
	dphi = subleadingpart.Phi() - leadingpart.Phi();
	Int_t lr = leadingpart.Reconstructed() ? 1 : 0, sr = subleadingpart.Reconstructed() ? 1 : 0;
	if(!fTreeStream) printf("Tree stream not initialized\n");
	(*fTreeStream) << "TwoParticleRes"
			<< "ptleading=" << ld[0]
			<< "etaleading=" << ld[1]
			<< "phileading=" << ld[2]
			<< "ptsubleading=" << sd[0]
			<< "etasubleading=" << sd[1]
			<< "phisubleading=" << sd[2]
			<< "leadingrec=" << lr
			<< "subleadingrec=" << sr
			<< "deta=" << deta
			<< "dphi=" << dphi
			<< "dr=" << dr
			<< "\n";
	PostData(1, fHistos);
}
