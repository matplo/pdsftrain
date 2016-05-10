TwoParticleAnalysis *AddTwoParticleAnalysis(){
	AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

	TwoParticleAnalysis *twoparticle = new TwoParticleAnalysis("twoparticleanalysis");
	mgr->AddTask(twoparticle);

	TString filename = mgr->GetCommonFileName();
	filename += ":twoparticlehists";

	mgr->ConnectInput(twoparticle, 0, mgr->GetCommonInputContainer());
	mgr->ConnectOutput(twoparticle, 1, mgr->CreateContainer("histocontainer", TList::Class(), AliAnalysisManager::kOutputContainer, filename.Data()));
	return twoparticle;
}
