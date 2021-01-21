#include "TROOT.h"
#include "TFile.h"
#include "observable.cc"
#include "TApplication.h"

int analysis(TString file, TString output){

  cout<< " "<<endl;
  cout<< file << endl;
  observable ss3(file);
  ss3.process=output;
  ss3.dEdx(-1);

  
  gSystem->Exit(0);

  return 0;
}
