import numpy as np
import argparse
import random
import os
import sys
	
#Handle de opciones
parser=argparse.ArgumentParser(
    description='''**Genera los archivos de input de Corsika, dependiendo de la ubicación geográfica y el ID de la particula. ''',
    epilog='''**Vr.1.0''')
parser.add_argument("PrjName",help="Nombre del Projecto")
parser.add_argument("-c","--city",help="Utilizar condiciones por defecto para: 1)Bogota (BOG), 2) Okayama (OKJ), 3) Brookhaven (BKH), 4) Napoles (NAP), 5) Tsukuba (TSK), 6) Tel-Aviv (TAV)",action="store")
args=parser.parse_args()

#Funciones auxiliares
def writeFile(nshow,cskID,alpha,emin,thetaMin,thetaMax,seed,obslevel,atm1,atm2,atm3,bx,bz,name):
    data=[  "RUNNR 1",
            "EVTNR	1",
            "NSHOW	%i"%(nshow),
            "PRMPAR	%i"%(cskID),
            "ESLOPE	%s"%(alpha),
            "ERANGE %s	1.E6"%(emin),
            "THETAP	%s  %s"%(thetaMin,thetaMax),
            "PHIP	-180.	180.",
            "SEED    %i	0	0"%(seed[0]),
            "SEED    %i	0	0"%(seed[1]),
            "SEED    %i	0	0"%(seed[2]),
            "SEED    %i	0	0"%(seed[3]),
            "OBSLEV	%s"%(obslevel),
            "%s	%s	%s"%(atm1,atm2,atm3),
            "FIXCHI	0.",
            "MAGNET	%s	%s"%(bx,bz),
            "ECUTS		0.05	0.05	5e-5	5e-5",
            "MUMULT	T",
            "ELMFLG	F	T",
            "STEPFC	1.0",
            "LONGI		F	10.	T	T",
            "ECTMAP	1.E3",
            "MAXPRT	0",
            "DIRECT	./%s"%(name),
            "USER	you",
            "EXIT"]
    return data 

def createFile(flux,cskID,alpha,emin,thetaMin,thetaMax,obslevel,atm1,atm2,atm3,bx,bz,name):
	seed=[random.random()*1e7,random.random()*1e7,random.random()*1e7,random.random()*1e7]
	fname = "./%s/%s-%010d.input"%(args.PrjName,name,flux)
	toFile=writeFile(flux,cskID,alpha,emin,thetaMin,thetaMax,seed,obslevel,atm1,atm2,atm3,bx,bz,name)
	file = open(fname,'w')
	for line in toFile:
		file.write("%s\n" % line)
	file.close()
	
#Main
primary = np.loadtxt("PrimaryFlux.txt",usecols=[2,3,4,5],skiprows=1)
HcskID = 14
HecskID = 402

city =args.city 
if(args.city is None):
	sys.exit("Utilizando modo interactivo")
elif(city=="BOG"):
    print("Utilizando parámetros por defecto para Bogotá...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 270000
    atm2 = 1
    atm1 ="ATMOSPHERE"
    atm3="F"
    bx = 27
    bz = 14.3
elif(city=="OKJ"):
    print("Utilizando parámetros por defecto para Okayama...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 530
    atm2 = 2
    atm1 ="ATMOSPHERE"
    atm3="F"
    bx = 31.3
    bz = 35.4
elif(city=="BKH"):
    print("Utilizando parámetros por defecto para Brookhaven...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 2400
    atm2 = 6
    atm1 ="ATMOSPHERE"
    atm3="F"
    bx = 18.9
    bz = 52.9
elif(city=="NAP"):
    print("Utilizando parámetros por defecto para Napoles...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 1700
    atm2 = 5
    atm1 ="ATMOD"
    atm3=""
    bx = 25.15
    bz = 36.66
elif(city=="TSK"):
    print("Utilizando parámetros por defecto para Tsukuba...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 3000
    atm2 = 5
    atm1 ="ATMOD"
    atm3=""
    bx = 25.3
    bz = 33.13
elif(city=="TAV"):
    print("Utilizando parámetros por defecto para Tel-Aviv...")
    thetaMin = 0
    thetaMax = 88
    obslevel = 500
    atm2 = 5
    atm1 ="ATMOD"
    atm3=""
    bx = 29.88
    bz = 33.46
#Crea el directorio para guardar los archivos
if not os.path.exists("./%s"%(args.PrjName)):
    os.makedirs("./%s"%(args.PrjName))
else:
	sys.exit("El directorio %s ya existe"%(args.PrjName))
	
#Escribe el archivo
for i in primary:
	alpha=i[0]
	cskID=i[1]
	flux=i[2]
	emin = i[3]
	if(cskID==HcskID):
		realFlux=flux/32
		for j in range(1,33):
			realName = "%02d%05d"%(j,cskID) 
			createFile(realFlux,cskID,alpha,emin,thetaMin,thetaMax,obslevel,atm1,atm2,atm3,bx,bz,realName)
	elif(cskID==HecskID):
		realFlux=flux/4
		for j in range(1,5):
			realName = "%02d%05d"%(j,cskID) 
			createFile(realFlux,cskID,alpha,emin,thetaMin,thetaMax,obslevel,atm1,atm2,atm3,bx,bz,realName)
	else:
		name = "%07d"%(cskID)
		createFile(flux,cskID,alpha,emin,thetaMin,thetaMax,obslevel,atm1,atm2,atm3,bx,bz,name)
	
