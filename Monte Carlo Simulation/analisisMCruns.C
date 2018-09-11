void analisisMCruns(Int_t maxRuns)
{
  //Archivo de salida
  TString fname = "results_1-";//Nombre
  fname+=Form("%i",maxRuns);
  fname+="_rhoMax_v1.root";
  
  TFile fresults(fname,"CREATE");//Creación del archivo
  
  if(fresults.IsZombie())//Caso de error cuando el archivo ya existe
    {
      cout<<"File already exist,it won't be updated"<<endl;
      return ;
    }

  TH2F *h_SR=NULL; //Histogramas donde se guarda la suma de todos los datos
  TGraph *gr_mtn;
  Int_t nFiles = 1;//Contador de archivos procesados
 
  while(nFiles<=maxRuns)//Recorrido de archivos .root
    {
      TString name = "rs_1000_";//Formato de nombre del archivo a procesar
      name+=Form("%i",nFiles);
      name+=".root";

      TFile *file1 = new TFile(name,"READ");//Archivo a procesar
      cout<<"File Read: "<<name<<endl;
      TH2F *h_temp;//Histogramas temporales

      file1->GetObject("h_SR",h_temp);//Lectura de histogramas en el archivo actual
      file1->GetObject("frontProfile",gr_mtn);
      
      //Lee y guarda la informacion del archivo procesado
      if(h_SR==NULL)
      	{
      	  h_SR=(TH2F*)h_temp->Clone("h_SR1");
       	}
      else
      	{
	  h_SR->Add(h_temp,1);
      	}
      nFiles+=1;
    }

  //Guardar los datos leidos
  fresults.WriteObject(gr_mtn,"frontProfile");
  fresults.WriteObject(h_SR,"h_SR");
  fresults.Close();
}
