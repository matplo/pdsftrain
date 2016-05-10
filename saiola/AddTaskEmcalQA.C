void AddTaskEmcalQA()
{
  enum {
    kMinBias,
    
    //kEMCalL0,
    kEMCalL1G1,
    //kEMCalL1G2,
    kEMCalL1J1,
    //kEMCalL1J2,
    
    //kDCalL0,
    kDCalL1G1,
    //kDCalL1G2,
    kDCalL1J1,
    //kDCalL1J2,
    
    kLastTrig
  };

  TString gCaloTriggerNames[kLastTrig];
  gCaloTriggerNames[kMinBias] = "CINT7-B-NOPF-CENT";
  //gCaloTriggerNames[kEMCalL0] = "CEMC7-B-NOPF-CENTNOPMD";
  gCaloTriggerNames[kEMCalL1G1] = "CINT7EG1-B-NOPF-CENTNOPMD";
  //gCaloTriggerNames[kEMCalL1G2] = "CINT7EG2-B-NOPF-CENTNOPMD";
  gCaloTriggerNames[kEMCalL1J1] = "CINT7EJ1-B-NOPF-CENTNOPMD";
  //gCaloTriggerNames[kEMCalL1J2] = "CINT7EJ2-B-NOPF-CENTNOPMD";
  //gCaloTriggerNames[kDCalL0] = "CDMC7-B-NOPF-CENTNOPMD";
  gCaloTriggerNames[kDCalL1G1] = "CINT7DG1-B-NOPF-CENTNOPMD";
  //gCaloTriggerNames[kDCalL1G2] = "CINT7DG2-B-NOPF-CENTNOPMD";
  gCaloTriggerNames[kDCalL1J1] = "CINT7DJ1-B-NOPF-CENTNOPMD";
  //gCaloTriggerNames[kDCalL1J2] = "CINT7DJ2-B-NOPF-CENTNOPMD";

  TString gCaloTriggerLabels[kLastTrig];
  gCaloTriggerLabels[kMinBias] = "INT7";
  //gCaloTriggerLabels[kEMCalL0] = "EMC7";
  gCaloTriggerLabels[kEMCalL1G1] = "EMCEG1";
  //gCaloTriggerLabels[kEMCalL1G2] = "EMCEG2";
  gCaloTriggerLabels[kEMCalL1J1] = "EMCEJ1";
  //gCaloTriggerLabels[kEMCalL1J2] = "EMCEJ2";
  //gCaloTriggerLabels[kDCalL0] = "DMC7";
  gCaloTriggerLabels[kDCalL1G1] = "DMCEG1";
  //gCaloTriggerLabels[kDCalL1G2] = "DMCEG2";
  gCaloTriggerLabels[kDCalL1J1] = "DMCEJ1";
  //gCaloTriggerLabels[kDCalL1J2] = "DMCEJ2";

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskSAQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskSAJF.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalTriggerQA.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C");
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskRho.C");

  AliEmcalJetTask* ktJetTask = AddTaskEmcalJet(AliEmcalJetTask::kKT | AliEmcalJetTask::kFullJet | AliEmcalJetTask::kR020Jet,
					       "", "CaloClusters", 0., 0.30, 0.005);

  AliEmcalJetTask* aktJetTask = AddTaskEmcalJet(AliEmcalJetTask::kAKT | AliEmcalJetTask::kFullJet | AliEmcalJetTask::kR020Jet,
						"", "CaloClusters", 0., 0.30, 0.005);

  AliAnalysisTaskRho* rhoTaskEMCal = AddTaskRho(ktJetTask->GetName(), "", "CaloClusters", "NeutralRhoEMCal", 0.2, "EMCAL", 0.01, 0, 0, 1, kTRUE, "EMCal");
  rhoTaskEMCal->SetUseNewCentralityEstimation(kTRUE);
  rhoTaskEMCal->SetNCentBins(5);
  rhoTaskEMCal->SetHistoBins(50, 0, 100);
  rhoTaskEMCal->GetJetContainer(0)->SetJetPtCut(0.1);

  AliAnalysisTaskRho* rhoTaskDCal = AddTaskRho(ktJetTask->GetName(), "", "CaloClusters", "NeutralRhoDCal", 0.2, "DCAL", 0.01, 0, 0, 1, kTRUE, "DCal");
  rhoTaskDCal->SetUseNewCentralityEstimation(kTRUE);
  rhoTaskDCal->SetNCentBins(5);
  rhoTaskDCal->SetHistoBins(50, 0, 100);
  rhoTaskDCal->GetJetContainer(0)->SetJetPtCut(0.1);

  for (Int_t i = 0; i < kLastTrig; i++) {
    TString suffix;

    AliEmcalTriggerQATask* pTriggerQA = AddTaskEmcalTriggerQA("EmcalTriggers", 0, 0, gCaloTriggerLabels[i]);
    pTriggerQA->SetEMCalTriggerMode(AliAnalysisTaskEmcal::kNoSpecialTreatment);
    pTriggerQA->SetTrigClass(gCaloTriggerNames[i]);
    pTriggerQA->SetUseNewCentralityEstimation(kTRUE);
    pTriggerQA->SetNCentBins(5);
    pTriggerQA->SetADCperBin(15);

    if (1) {
      // QA task
      if (0) {
	suffix = "BeforeTender_";
	suffix += gCaloTriggerLabels[i];
	AliAnalysisTaskSAQA *pQATaskBefore = AddTaskSAQA("", "CaloClusters", "EMCALCells", "", "",
							 0, 0, 0, 0., 0., "TPC", suffix);
	pQATaskBefore->GetClusterContainer(0)->SetClusECut(0.15);
	pQATaskBefore->GetClusterContainer(0)->SetClusPtCut(0.);
	pQATaskBefore->GetClusterContainer(0)->SetExoticCut(kFALSE);
	pQATaskBefore->SetHistoBins(100, 0, 100);
	pQATaskBefore->SetEMCalTriggerMode(AliAnalysisTaskEmcal::kNoSpecialTreatment);
	pQATaskBefore->SetTrigClass(gCaloTriggerNames[i]);
	pQATaskBefore->SetVzRange(-999,999);
	pQATaskBefore->SetUseNewCentralityEstimation(kTRUE);
	pQATaskBefore->SetNCentBins(5);
      }

      //suffix = "AfterTender_";
      suffix = gCaloTriggerLabels[i];
      AliAnalysisTaskSAQA *pQATaskAfter = AddTaskSAQA("", "CaloClusters", "EMCALCells", "", "",
						      0, 0, 0, 0., 0., "TPC", suffix);
      pQATaskAfter->GetClusterContainer(0)->SetClusECut(0.);
      pQATaskAfter->GetClusterContainer(0)->SetClusPtCut(0.);
      pQATaskAfter->GetClusterContainer(0)->SetClusNonLinCorrEnergyCut(0.15);
      pQATaskAfter->SetDefaultClusterEnergy(AliVCluster::kNonLinCorr);
      pQATaskAfter->SetHistoBins(150, 0, 150);
      pQATaskAfter->SetEMCalTriggerMode(AliAnalysisTaskEmcal::kNoSpecialTreatment);
      pQATaskAfter->SetTrigClass(gCaloTriggerNames[i]);
      pQATaskAfter->SetVzRange(-999,999);
      pQATaskAfter->SetUseNewCentralityEstimation(kTRUE);
      pQATaskAfter->SetNCentBins(5);

      if (gCaloTriggerLabels[i] == "INT7" || gCaloTriggerLabels[i].BeginsWith("EMCE")) {
	suffix = gCaloTriggerLabels[i];
	AliAnalysisTaskSAJF *pJFTask = AddTaskSAJF("", "CaloClusters", aktJetTask->GetName(), "NeutralRhoDCal", 0.2, 1, 0., "EMCALfid", 1, suffix);
	pJFTask->SetEMCalTriggerMode(AliAnalysisTaskEmcal::kNoSpecialTreatment);
	pJFTask->SetTrigClass(gCaloTriggerNames[i]);
	pJFTask->SetVzRange(-999,999);
	pJFTask->SetUseNewCentralityEstimation(kTRUE);
	pJFTask->SetNCentBins(5);
	pJFTask->SetHistoBins(200,0,200);
      }

      if (gCaloTriggerLabels[i] == "INT7" || gCaloTriggerLabels[i].BeginsWith("DMCE")) {
	suffix = gCaloTriggerLabels[i];
	AliAnalysisTaskSAJF *pJFTask = AddTaskSAJF("", "CaloClusters", aktJetTask->GetName(), "NeutralRhoEMCal", 0.2, 1, 0., "DCALfid", 1, suffix);
	pJFTask->SetEMCalTriggerMode(AliAnalysisTaskEmcal::kNoSpecialTreatment);
	pJFTask->SetTrigClass(gCaloTriggerNames[i]);
	pJFTask->SetVzRange(-999,999);
	pJFTask->SetUseNewCentralityEstimation(kTRUE);
	pJFTask->SetNCentBins(5);
	pJFTask->SetHistoBins(200,0,200);
      }
    }
  }
}
