void set_plot_style1(TString filename)
{
  ifstream colors;
  colors.open(filename);
    const Int_t NRGBs = 256;
    const Int_t NCont = 255;
    Double_t steps = 1.0/NCont;
    Double_t stops[NRGBs],red[NRGBs],green[NRGBs],blue[NRGBs];
 
    Int_t cont = 0;
    while(colors>>red[cont]>>green[cont]>>blue[cont])
      {
	stops[cont]=cont*steps;
	cont++;
      }

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

/***************************************************************************
Main
 ***************************************************************************/
void plotTimePrediction()
{
  //Color bar configuration
  TString filename = "/hpcfs/home/js.useche10/ScientificColourMaps3/lajolla/lajolla.txt";
  set_plot_style1(filename);
  gStyle->SetOptStat(0);

  
  TFile fRates("TrateAll_v1.root","READ");
  TFile fOS("OSdistr.root","READ");

  TH2F *hSR,*hMax,*hMin,*hOS;
  TH2F *htSR,*htMax,*htMin;
  TGraph *gr;

  fRates.GetObject("rhoSR",hSR);
  fRates.GetObject("rhoMax",hMax);
  fRates.GetObject("rhoMin",hMin);


  
  fRates.GetObject("mtn",gr);
  gr->SetLineWidth(4); 
  gr->SetLineColorAlpha(kBlack,0.5);
  
  fOS.GetObject("hOS",hOS);

  htSR = (TH2F*)hOS->Clone("htSR");
  htMax = (TH2F*)hOS->Clone("htMax");
  htMin = (TH2F*)hOS->Clone("htMin");

  htSR->SetTitle("Time Prediction");
  htSR->GetYaxis()->SetTitle("#frac{#pi}{2}-#theta [deg]");
  htSR->GetXaxis()->SetTitle("#phi [deg]");
  htSR->GetZaxis()->SetTitle("Time [Hours]");
  
  htSR->Multiply(hSR);
  htMax->Multiply(hMax);
  htMin->Multiply(hMin);
  
  Double_t cons = 24000;
  
  TH2F *hUnitary;
  hUnitary = (TH2F*)htSR->Clone("hU");
  hUnitary->Reset();
  for(int i = 1; i<=hUnitary->GetXaxis()->GetNbins();i++)
    {
      for(int j = 1; j<=hUnitary->GetYaxis()->GetNbins();j++)
	hUnitary->SetBinContent(i,j,cons);
    }

  hUnitary->Divide(htSR);
  hUnitary->SetMinimum(1e2);
  hUnitary->SetMaximum(1e7);
  hUnitary->SetTitle("");
  
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogz();
  hUnitary->Draw("COLZ");
  gr->Draw("same");

  auto txt=Form("%.0f",1.0);
  
  TH2F *histos[3]={htSR,htMax,htMin};
  TH2F *histos1[3]={hSR,hMax,hMin};
  Int_t align[3]={23,22,21};
  Double_t offset[3]={1.5,0,-1.5};
  Double_t colors[3]={1,2,4};
  
  for(int k = 0; k<3; k++)
    {
      for(int i = 1; i<=histos[k]->GetXaxis()->GetNbins();i++)
  	{
  	  for(int j = 1; j<=histos[k]->GetYaxis()->GetNbins();j++)
  	    {
  	      bool draw = true;
  	      Double_t content = histos[k]->GetBinContent(i,j);
	      Double_t content1 = histos1[k]->GetBinContent(i,j);
  	      if(content1 == 1 ||  content1 == 0)
  		{
		  if(content==0)
		    txt=Form("%.1f",0.0);
		  else
		    txt=Form("%.1f",cons/content);
		  if(k!=0)
  		    draw=false;
  		}
  	      else
  		txt=Form("%1.1e",cons/content);

  	      auto lbl = new TText(histos[k]->GetXaxis()->GetBinCenter(i),
  				   histos[k]->GetYaxis()->GetBinCenter(j)+offset[k],
  				   txt); 
  	      lbl->SetTextAlign(align[k]);
  	      lbl->SetTextSize(0.025);
  	      lbl->SetTextColor(colors[k]);
  	      if(draw)
  		lbl->Draw();
  	    }
  	}
    }
   
  gPad->SetRightMargin(0.13);

  c1->SetGrid();
  c1->Print("TimePred_v1.pdf");
}
