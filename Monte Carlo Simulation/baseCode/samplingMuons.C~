#include "rockMuon.C"

/****************************************************************************
 *Funciones auxiliares
 *****************************************************************************/
Double_t * fillArray(Int_t n,Double_t thMax,Double_t thMin)
{

   Double_t *x = new Double_t[n];
   Double_t dAngle = (thMax-thMin)/(n-1);
   for(Int_t i=0; i<n; i++) x[i] = thMin+i*dAngle;
   return x;
}

int placeSearch(double arr[], int l, int r, double dangle,double x)
{
  while (l <= r)
    {
      if(x<arr[l]+dangle && x>=arr[l]-dangle)
	return l;
      l++;
    }
 
    // if we reach here, then element was
    // not present
    return -1;
}

void loadFile(Double_t theta[],Double_t phi[],Double_t dMtn[],Double_t dAir[])
{
  ifstream file;
  file.open(Form("distanciaPromedioMuones.txt"));
  string header;
  getline(file,header);
  int ndata = 0;
  while(file>>theta[ndata]>>phi[ndata]>>dMtn[ndata]>>dAir[ndata])
    {//cout<<theta[ndata]<<" "<<phi[ndata]<<" "<<dMtn[ndata]<<" "<<dAir[ndata]<<endl;
      ndata++;}
}

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

/****************************************************************************
 *Clases auxiliares
 *****************************************************************************/
/*
 *Clase que modela  un muon identificado por una tripleta (Theta,Phi,P)
 *teniendo en cuenta las distribuciones en el archivo "Distribuciones.root"
 */
class Muon
{
  Double_t theta,phi,p,dMtn;
 public:
  Muon();
  Double_t getZenith(){return theta;}
  Double_t getAzimuth(){return phi;}
  Double_t getMomentum(){return p;}
  Double_t getDMtn(){return dMtn;}
};
Muon::Muon()
{
   TFile *MyFile = new TFile("EnergyDistributions_limits.root","READ");
   TFile *MyFile1 = new TFile("DistanceDistr.root","READ");

   TH1F *h_th,*h_p;
   TH2F *h_mtn;
   
   MyFile->GetObject("Thpdf",h_th);

   phi = gRandom->Uniform(-25,25);//Asignación del valor del angulo azimuth
   
   //theta =h_th->GetRandom();//Valor ángulo zenith
   theta = gRandom->Uniform(60,90);
   
   //Valor de energía respecto al ángulo zenith obtenido
   const Int_t nAngles = 6;
   Double_t thMax = 90;
   Double_t thMin = 60;
   Double_t dAngle = (thMax-thMin)/(nAngles-1);
   Double_t *ang;
  
   ang = fillArray(nAngles,thMax,thMin);

   Int_t posTh = placeSearch(ang,0,nAngles,dAngle/2.0,theta);
   
   MyFile->GetObject(Form("EnergyScld #theta=%1.1f",ang[posTh]),h_p);

   p = h_p->GetRandom();
   
   //Asignación distancia en la montaña
   MyFile1->GetObject("h2DdMtn",h_mtn);
   Int_t binX = h_mtn->GetXaxis()->FindBin(phi);
   Int_t binY = h_mtn->GetYaxis()->FindBin(90-theta);
   dMtn = h_mtn->GetBinContent(binX,binY);

   //cout<<"Theta="<<theta<<" Phi="<<phi<<" dMtn="<<dMtn<<endl;
   
   MyFile->Close();
   MyFile1->Close();
}

/****************************************************************************
 *Main
 *****************************************************************************/

void samplingMuons( Int_t maxMuons)
{
  gRandom = new TRandom3(0);

  TStopwatch t;
  t.Start();
 
  //Configuración colores
  const char filename[] = "/hpcfs/home/js.useche10/ScientificColourMaps3/roma/roma.txt";
  set_plot_style1(filename);
  gStyle->SetOptStat(0);

  //Histogramas
  TH2F * h_OS = new TH2F ("hist_OS","Open Sky",10,-25,25,6,0,30);
  TH2F * h_dist = new TH2F ("hist_dist","Mtn",10,-25,25,6,0,30);
  h_dist->GetYaxis()->SetTitle("#frac{#pi}{2}-#theta [deg]");
  h_dist->GetXaxis()->SetTitle("#phi [deg]");
  h_dist->GetZaxis()->SetTitle("Flux [# Muons]");

  Int_t muons = 0;
  Double_t minimum = 0;

  while(minimum<maxMuons)
    {
      Muon mu;
      h_OS->Fill(mu.getAzimuth(),90-mu.getZenith());
      minimum = h_OS->GetMinimum();

      Bool_t passMtn = rockMuon(mu.getMomentum()*1000,mu.getDMtn()*100,2.65);
      if(passMtn)
	h_dist->Fill(mu.getAzimuth(),90-mu.getZenith());
    }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);

  c1->cd(1);
  h_OS->Draw("COLZ TEXT");

  c1->cd(2);
  h_dist->Draw("COLZ TEXT");

  c1->Print(Form("results_v1_%1.0d.pdf",maxMuons));

  t.Stop();
  t.Print(); 
}
