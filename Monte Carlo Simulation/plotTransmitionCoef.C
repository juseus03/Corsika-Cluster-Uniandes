void set_plot_style1(const char *filename)
{
  ifstream colors;
  colors.open(Form(filename));
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

void plotTransmitionCoef()
{
  //Color bar configuration
  const char filename[] = "/hpcfs/home/js.useche10/ScientificColourMaps3/roma/roma.txt";
  set_plot_style1(filename);
  gStyle->SetOptStat(0);
  
  TFile file("results_1-100_rhoMax_v1.root","READ");

  TH2F *h_SR,*h_ratio;
  TGraph *gr_mtn;
  
  file.GetObject("h_SR",h_SR);
  file.GetObject("frontProfile",gr_mtn);
  
  gr_mtn->SetLineWidth(4); 
  gr_mtn->SetLineColorAlpha(kBlack,0.5);
  for (int i=0;i<gr_mtn->GetN();i++) gr_mtn->GetY()[i] += 1.3;

  //Double_t scales[6]={0.00105367,0.00193922,0.00409709,0.0103702,0.0362038,0.0364264};
  Double_t scales[6]={0.00523396,0.00938994,0.00114344,0.000451792,0.00420371,0.00665528};
  
  h_ratio = (TH2F*) h_SR->Clone("h_ratio");
  h_ratio->Scale(1.0/1e5);
  h_ratio->SetMinimum(1e-6);
  h_ratio->SetMaximum(1e-1);
  h_ratio->SetTitle("Transmission Coefficient");

  for(int i = 1; i<=h_ratio->GetXaxis()->GetNbins();i++)
    {
      for(int j = 1; j<=h_ratio->GetYaxis()->GetNbins();j++)
  	{
	  Double_t content = h_ratio->GetBinContent(i,j);
  	  if(content == 1 && h_ratio->GetYaxis()->GetBinCenter(j)<=20)
	    {
	      h_ratio->SetBinContent(i,j,content*scales[6-j]);
	    }
	  else if(content!=1)
	    h_ratio->SetBinContent(i,j,content*scales[6-j]);
	}
    }
  
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetLogz();
  h_ratio->Draw("COLZ");
  gr_mtn->Draw("same");
  
  auto txt=Form("%.0f",1.0);
  
  for(int i = 1; i<=h_ratio->GetXaxis()->GetNbins();i++)
    {
      for(int j = 1; j<=h_ratio->GetYaxis()->GetNbins();j++)
  	{
  	  Double_t content = h_ratio->GetBinContent(i,j);
  	  if(content == 1 ||  content == 0)
	    {
	      txt=Form("%.0f",content);
	    }
  	  else
	    {
	      txt=Form("%1.1e",content);
	    }
  	  auto lbl = new TText(h_ratio->GetXaxis()->GetBinCenter(i),
  			       h_ratio->GetYaxis()->GetBinCenter(j),
			       txt); 
  	  lbl->SetTextAlign(22);
	  lbl->SetTextSize(0.03);
	  lbl->SetTextColor(kBlack);
  	  lbl->Draw();
  	}
    }
  gPad->SetRightMargin(0.13);

  
  TFile fres("TrateAll_v1.root","UPDATE");
  h_ratio->Write("rhoMax");
  //gr_mtn->Write("mtn");
  
  c1->Print("ratioTransmition_MaxV2.pdf");
}
