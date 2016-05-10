void AddTaskInvariantMass()
{
    // Should probably change this to TRAIN_ROOT
    const char * trainRoot = gSystem->Getenv("TRAIN_ROOT");
    //const char * trainRoot = gSystem->Getenv("PWD");
    Printf("Train root: %s", trainRoot);
    if (trainRoot != 0)
    {
    	/*
        Printf("Added rehlers user directory to includes and compiling.");
        gSystem->AddIncludePath(Form("-I%s/rehlers", trainRoot));
        gSystem->AddIncludePath(Form("-I%s/include", gSystem->Getenv("ALICE_ROOT")));
        gSystem->AddIncludePath(Form("-I%s/include", gSystem->Getenv("ALICE_PHYSICS")));
        gROOT->LoadMacro(Form("%s/rehlers/AliAnalysisTaskEmcalInvariantMass.cxx++", trainRoot));
        */
        
        // Use ++ to only compile if necessary
        TString clustersName = "CaloClusters";
        gROOT->LoadMacro(Form("%s/rehlers/AddTaskEmcalInvariantMass.C", trainRoot));
        AliAnalysisTaskEmcalInvariantMass* anaTask = AddTaskEmcalInvariantMass(/*tracksName,*/clustersName, 1);
        anaTask->SetUseNewCentralityEstimation(kTRUE);
        anaTask->SetNCentBins(4);
    }

    else
    {
        Printf("Failed to include rehlers user directory");
    }

    //std::exit(0);
}
