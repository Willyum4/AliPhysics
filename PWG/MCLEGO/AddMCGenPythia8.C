R__LOAD_LIBRARY(liblhapdf)
R__LOAD_LIBRARY(libpythia6)

#include "AliGenerator.h"
#include "AliGenPythia.h"

AliGenerator* CreatePythia8Gen(Float_t e_cms, 
			       Bool_t kCR, 
			       Int_t kF,
			       Int_t kProcess,
			       Double_t ptHardMin,
			       Double_t ptHardMax, 
			       Int_t tune) 
{
    
   gSystem->Setenv("PYTHIA8DATA", gSystem->ExpandPathName("$ALICE_ROOT/PYTHIA8/pythia8/xmldoc"));
   gSystem->Setenv("LHAPDF",      gSystem->ExpandPathName("$ALICE_ROOT/LHAPDF"));
   gSystem->Setenv("LHAPATH",     gSystem->ExpandPathName("$ALICE_ROOT/LHAPDF/PDFsets"));


  AliGenPythiaPlus* gener = new AliGenPythiaPlus(AliPythia8::Instance());

  // set process (MB)
  if (kProcess==0) 
    gener->SetProcess(kPyMbDefault);
  else if(kProcess==1) {
    gener->SetProcess(kPyJets);
    if(ptHardMin>0.)
      gener->SetPtHard(ptHardMin,ptHardMax);
  } 

  //Centre of mass energy 
  gener->SetEnergyCMS(e_cms); // in GeV

  // Event list
  gener->SetEventListRange(-1, 2);

  // color reconnection
  (AliPythia8::Instance())->ReadString(Form("Tune:pp = %d",tune)); 

  //random seed based on time
  AliPythia8::Instance()->ReadString("Random:setSeed = on");
  AliPythia8::Instance()->ReadString("Random:seed = 0");

  if(kCR)             
    (AliPythia8::Instance())->ReadString("ColourReconnection:reconnect = on");
  else
    (AliPythia8::Instance())->ReadString("ColourReconnection:reconnect = off");
  
 
  AliPythia8::Instance()->ReadString(Form("MultipartonInteractions:kFactor = %i", kF));
  
  return gener;
}

AliGenerator* AddMCGenPythia8(Float_t e_cms = 2760., 
                              Bool_t kCR = kTRUE, 
                              Int_t kF = 1,
                              Int_t kProcess=0,
                              Double_t ptHardMin=0,
                              Double_t ptHardMax=1., 
                              Int_t tune=14) 
{
  // Add Pythia 8 generator: 
  //    -kProcess=0  MB generation
  //    -kProcess=1  Jet production, pthard generation
  //    - Color reconnection = ON/OFF
  //    - Set k factor, default = 1; range of possible values in xmldoc/CouplingsAndScales.xml

  AliGenerator *genP = NULL;
  genP = CreatePythia8Gen(e_cms, kCR, kF,kProcess,ptHardMin,ptHardMax,tune);
  
  return genP;
}


