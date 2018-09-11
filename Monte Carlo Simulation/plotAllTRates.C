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
void plotAllTRates()
{
  //Color bar configuration
  TString filename = "/hpcfs/home/js.useche10/ScientificColourMaps3/vik/vik.txt";
  set_plot_style1(filename);
  gStyle->SetOptStat(0);
  
  TFile file("TrateAll_v1.root","READ");

  TH2F *hSR,*hMax,*hMin;
  TGraph *gr;
  
  file.GetObject("rhoSR",hSR);
  file.GetObject("rhoMax",hMax);
  file.GetObject("rhoMin",hMin);
  file.GetObject("mtn",gr);

  hSR->SetMaximum(1e-1);
  hSR->SetMinimum(1e-6);
  hSR->SetTitle("");
  gr->SetLineWidth(4); 
  gr->SetLineColorAlpha(kBlack,0.5);
  for (int i=0;i<gr->GetN();i++) gr->GetY()[i] += 0.0;
  
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogz();
  hSR->Draw("COLZ");
  gr->Draw("same");
  
  auto txt=Form("%.0f",1.0);
  
  TH2F* histos[3]={hSR,hMax,hMin};
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
	      if(content == 1 ||  content == 0)
		{
		  txt=Form("%.0f",content);
		  if(k!=0)
		    draw=false;
		}
	      else
		txt=Form("%1.1e",content);

	      auto lbl = new TText(histos[k]->GetXaxis()->GetBinCenter(i),
				   histos[k]->GetYaxis()->GetBinCenter(j)+offset[k],
				   txt); 
	      lbl->SetTextAlign(align[k]);
	      lbl->SetTextSize(0.03);
	      lbl->SetTextColor(colors[k]);
	      if(draw)
		lbl->Draw();
	    }
	}
    }
   
  gPad->SetRightMargin(0.13);

  c1->Print("TRateAll_NS.pdf");
}
