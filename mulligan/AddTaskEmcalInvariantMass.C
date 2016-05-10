void AddTaskEmcalInvariantMass()
{
  TString clustersName = "CaloClusters";
  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalInvariantMass.C");
  AliAnalysisTaskEmcalInvariantMass* anaTask = AddTaskEmcalInvariantMass(/*tracksName,*/clustersName, 1);
}
