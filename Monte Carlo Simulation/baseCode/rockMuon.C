Bool_t rockMuon(Double_t energy, Double_t distM, Double_t rho)
{
   ifstream dEdxSR,dEdxAir;
   dEdxSR.open("muE_standard_rock.dat");

   double T[1000],p[1000],ion[1000], brems[1000],pair[1000];
   double photonu[1000], radloss[1000], dedx[1000], csda[1000];
   double delta[1000], beta[1000], dedx_r[1000];
   double error[1000];

   //Standard Rock dEdx Data
   int ndata=0;
   while (dEdxSR>>T[ndata]>>p[ndata]>>ion[ndata]>>brems[ndata]>>pair[ndata]>>photonu[ndata]>>radloss[ndata]>>dedx[ndata]>>csda[ndata]>>delta[ndata]>>beta[ndata]>>dedx_r[ndata]){
     ndata++;
   }

    //Interpolation
 double Tin=energy; // initial energy MeV
 double TTmin=1;
 double lengthM=distM;// legnth in cm
 double dlength= 100.0;// delta length in cm
 
 double elost;
 double Tfinal= Tin;
 double deltae;
 double densityM=rho; // g/cm3 for standard rock 2.65
 
 int j1, j2;
 int initialmuons=1;
 int finalmuons=initialmuons;

 int nbinsM=lengthM/dlength;
 Tfinal=Tin;
 bool passMountain = true; 

 for (int i=0;i<nbinsM;i++){
   //first find the correct value fo de_dx given initial energy
   bool found =false;
   int j=1;
   while(!found&&j<ndata){
     //cout<<"Tfinal="<<Tfinal<<" ; T["<<j<<"]="<<T[j]<<endl;
     if (Tfinal<T[j]){
       found=true;
       j1=j-1;
       j2=j;
     }
     j++;
   }
   if (found) {
     elost=dlength*densityM*(dedx[j1]+dedx[j2])/2.0;	
     //cout<<"Tfinal="<<Tfinal<<" ; de_dx="<<dedx[j2]<<"  ; elost="<<elost<<" ; length="<<i*dlength<<endl;
     Tfinal-=elost;
   }
   else {
     cout<<"cannot interpolate: abort the program 1"<<endl;
     return 0;
   }
   if (Tfinal<TTmin) {
     finalmuons-=1;
     passMountain=false;
     //cout<<" nbins="<<nbinsM<<" ; i="<<i<<endl;
     break;
   }   
 }

 return passMountain;
}
