
// Auther: Hengne.Li@cern.ch   
//         University of Virginia, 2014

#include <stdio.h>
#include <iostream>
#include <string>
#include "config.hpp"
#include <vector>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"


int main(int argc, char* argv[])
{
  if( argc<2 )
  {
    std::cout << argv[0] << " config_file " << std::endl;
    return 0;
  }

  // open steering file names
  config steer(argv[1]);

  ////////////////////////////////////////
  // reading configuration parameters   //
  ////////////////////////////////////////

  // only print numbers, no plotting
  bool printOnly = steer.getBool("printOnly");
  
  // outputs
  std::string OutputDir = steer.getString("OutputDir");
  std::string OutputRootFile = steer.getString("OutputRootFile");
  std::string OutputPlotFile = steer.getString("OutputPlotFile");

  // fixMe: check OutputDir exists or not

  // Baseline selection 
  std::string BaseSelection = steer.getString("BaseSelection");

  // Base weight to be applied (not including cross-section and not deviding normEvents)
  std::string BaseWeight = steer.getString("BaseWeight");

  // m4l bins
  int nBinsM4l = steer.getInt("nBinsM4l");

  // RunPeriods to be processed: e.g. Run1A, Run1B, Run1, Run2
  std::vector<std::string> RunPeriods = steer.getStringArray("RunPeriods");

  // define sample types  
  std::vector<std::string> SampleTypes = steer.getStringArray("SampleTypes");

  // for Run1A
  double globalLumi_Run1A;
  std::string InputDirectory_Run1A;
  std::vector<std::string> RootFiles_Run1A, RootFiles_Type_Run1A, RootFiles_Weight_Run1A, RootFiles_Tree_Run1A;

  // decide if read Run1A information
  bool readRun1AInfo(false);
  for (int i=0; i<(int)RunPeriods.size(); i++) if (RunPerids.at(i)=="Run1A" || RunPerids.at(i)=="Run1") readRun1AInfo = true;
  // read
  if (readRun1AInfo) 
  {
    globalLumi_Run1A = steer.getDouble("globalLumi_Run1A");
    InputDirectory_Run1A = steer.getDouble("InputDirectory_Run1A");
    RootFiles_Run1A = steer.getStringArray("RootFiles_Run1A");
    RootFiles_Type_Run1A = steer.getStringArray("RootFiles_Type_Run1A");
    RootFiles_Weight_Run1A = steer.getStringArray("RootFiles_Weight_Run1A");
    RootFiles_Tree_Run1A = steer.getStringArray("RootFiles_Tree_Run1A");
  } 

  // for Run1B
  double globalLumi_Run1B;
  std::string InputDirectory_Run1B;
  std::vector<std::string> RootFiles_Run1B, RootFiles_Type_Run1B, RootFiles_Weight_Run1B, RootFiles_Tree_Run1B;

  // decide if read Run1B information
  bool readRun1BInfo(false);
  for (int i=0; i<(int)RunPeriods.size(); i++) if (RunPerids.at(i)=="Run1B" || RunPerids.at(i)=="Run1") readRun1BInfo = true;
  // read
  if (readRun1BInfo)
  {
    globalLumi_Run1B = steer.getDouble("globalLumi_Run1B");
    InputDirectory_Run1B = steer.getDouble("InputDirectory_Run1B");
    RootFiles_Run1B = steer.getStringArray("RootFiles_Run1B");
    RootFiles_Type_Run1B = steer.getStringArray("RootFiles_Type_Run1B");
    RootFiles_Weight_Run1B = steer.getStringArray("RootFiles_Weight_Run1B");
    RootFiles_Tree_Run1B = steer.getStringArray("RootFiles_Tree_Run1B");
  }

  // quit if not processing any run
  if (!readRun1AInfo && !readRun1BInfo)
  {
    std::cout << "Nothing to be processed, please define RunPeriods in your config file." << std::endl; 
    return 0;
  }

  ////////////////////
  // an useful string
  ////////////////////
  char name[3000];

  ////////////////////////////////
  // initialize output files
  ////////////////////////////////
  sprintf(name, "%s/%s", OutputDir.c_str(), OutputRootFile.c_str());
  TFile* outputRootFile = new TFile(name, "recreate");
  sprintf(name, "%s/%s.ps", OutputDir.c_str(), OutputPlotFile.c_str());
  std::string outputPsFile(name);
  sprintf(name, "%s/%s.pdf", OutputDir.c_str(), OutputPlotFile.c_str());
  std::string outputPdfFile(name);
  

  ////////////////////////////////
  //  Initialize the root files
  ////////////////////////////////
 
  std::vector<TFile*> inputFiles_Run1A, inputFiles_Run1B;
  std::vector<TTree*> trees_Run1A, trees_Run1B;
  std::vector<double> normEvents_Run1A, normEvents_Run1B;

  // Run1A
  for (int i=0; i<(int)RootFiles_Run1A.size(); i++)
  {
    sprintf(name,"%s/%s", InputDirectory_Run1A.c_str(), RootFiles_Run1A.at(i).c_str());
    TFile* A_root_file = TFile::Open(name);
    TTree* A_tree = (TTree*)A_root_file->Get(RootFiles_Tree_Run1A.at(i).c_str());
    
    TH1D* A_hist = (TH1D*)A_root_file->Get("AnaAfterHlt/nEvents");
    TH1D* A_hist2; 
    if (!A_hist) 
    {
      A_hist2 = (TH1D*)A_root_file->Get("Ana/nEvents");
      if(!A_hist2) { std::cout << "Cannot find AnaAfterHlt/nEvents." << std::endl; exit(1) }
    } 
    double A_normEvents;
    if (!A_hist) A_normEvents = A_hist2->GetBinContent(1);
    else A_normEvents = A_hist->GetBinContent(1);

    inputFiles_Run1A.push_back(A_root_file);
    trees_Run1A.push_back(A_tree);
    normEvents_Run1A.push_back(A_normEvents); 
  }    

  // Run1B
  for (int i=0; i<(int)RootFiles_Run1B.size(); i++)
  {
    sprintf(name,"%s/%s", InputDirectory_Run1B.c_str(), RootFiles_Run1B.at(i).c_str());
    TFile* A_root_file = TFile::Open(name);
    TTree* A_tree = (TTree*)A_root_file->Get(RootFiles_Tree_Run1B.at(i).c_str());

    TH1D* A_hist = (TH1D*)A_root_file->Get("AnaAfterHlt/nEvents");
    TH1D* A_hist2;
    if (!A_hist)
    {
      A_hist2 = (TH1D*)A_root_file->Get("Ana/nEvents");
      if(!A_hist2) { std::cout << "Cannot find AnaAfterHlt/nEvents." << std::endl; exit(1) }
    }
    double A_normEvents;
    if (!A_hist) A_normEvents = A_hist2->GetBinContent(1);
    else A_normEvents = A_hist->GetBinContent(1);

    inputFiles_Run1B.push_back(A_root_file);
    trees_Run1B.push_back(A_tree);
    normEvents_Run1B.push_back(A_normEvents);
  }


  /////////
  // close fout
  //fout->Close();
  return 0;

}

