#ifndef TWOPARTICEANALYSIS_H
#define TWOPARTICEANALYSIS_H

class AliESDtrackCuts;
class TTreeSRedirector;
class TList;
class TH1;

#include <AliAnalysisTaskSE.h>

class ParticleStruct {
public:
	ParticleStruct() : fPt(-1), fEta(-1), fPhi(-1), fLabel(0), fReconstructed(false) {}
	ParticleStruct(Double_t pt, Double_t eta, Double_t phi, Int_t label) : fPt(pt), fEta(eta), fPhi(phi), fLabel(label), fReconstructed(false) {}
	~ParticleStruct() {}

	double Pt() const { return fPt; }
	double Eta() const { return fEta; }
	double Phi() const { return fPhi; }
	int Label() const { return fLabel; }
	bool Reconstructed() const { return fReconstructed; }

	void SetReconstructed(bool doset = true) { fReconstructed = doset; }

	bool operator<(const ParticleStruct &other) const { return fPt < other.fPt; }
	bool operator==(const ParticleStruct &other) const { return fLabel == other.fLabel; }

private:
	double				fPt;
	double				fEta;
	double				fPhi;
	int					fLabel;
	bool				fReconstructed;
};

class TwoParticleAnalysis : public AliAnalysisTaskSE {
public:
	TwoParticleAnalysis();
	TwoParticleAnalysis(const char *name);
	virtual ~TwoParticleAnalysis();

	void UserCreateOutputObjects();
	void UserExec(Option_t *opt);

private:
	AliESDtrackCuts 			*fTrackCuts;
	TList 								*fHistos;
	TH1										*fEventCount;
	TTreeSRedirector			*fTreeStream;

	ClassDef(TwoParticleAnalysis, 1);
};
#endif
