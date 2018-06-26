#Librerias Importadas
import numpy as np
import argparse

#Handle de opciones
parser=argparse.ArgumentParser(
    description='''**Calcula la composicion de primarios en Rayos Cosmicos para un intervalo de tiempo. ''',
    epilog='''**Vr.1.0''')
parser.add_argument("time", help="Tiempo (seg) de simulacion",type=int)
args=parser.parse_args()


#Global Variables
Emax=1e6/1000.0 #TeV
pi = 3.141516
area = 1e4 #Conversion cm2 a m2
tiempo = args.time #s
N0 = 2*pi*area*tiempo*(0.5*np.sin(90*pi/180))

file = open("PrimaryFlux.txt","w") #Archivo para escribir los resultados
file.write("Name\tZ\tAlpha\tCskID\tFlux[Nucleus*m^-2]\t Emin\t Tiempo=%is\n"%tiempo)

# masses 
#partCskID= [0, 14, 402, 703, 904, 1105, 1206, 1407, 1608, 1909, 2010, 2311, 2412, 2713, 2814, 3115, 3216, 3517, 3919, 4018, 4020, 4521, 4822, 5123, 5224, 5525, 5626]
partMass = [0., 0.938272, 3.73338244805557, 6.53478032991106, 8.39429044902688, 10.2536061929541, 11.1787903246739, 13.045071978869, 14.898326507629, 17.6899146520668, 18.6173579550734, 21.4080199431823, 22.3362803688324, 25.1263356296296, 26.0553153433303, 28.8449660324983, 29.7745989328225, 32.5639816988633, 36.2834316370329, 37.2107457840596, 37.2142385732562, 41.8605295331555, 44.6483661801865, 47.4401999906342, 48.3681334024753, 51.1598095147594, 52.0885229269484]

#Functions
def Emin(Z): 
	p0 = Z*5.0
	return np.sqrt(p0**2+partMass[Z]**2)/1000.0
	
#Flujo de nucleos (Nucleos/(cm2*s*sr))
def flujo(E,j,a):
    diff=Emax**(a+1)-(E)**(a+1)
    return (N0*j0/(a+1))*diff
	
#Main Program
data = np.loadtxt("IntensitySpectralIndex.txt",dtype={'names':('element','Z','A','j0','alpha'),'formats':('S2','i4','i4','f8','f8')},skiprows=3)

for i in range(1,27):
	Z=i
	name = data[Z-1][0].decode('utf-8')
	j0=data[Z-1][3]
	a=data[Z-1][4]
	eng = Emin(Z)
	flux = flujo(eng,j0,a)
	cskId= 14 if Z==1 else data[Z-1][2]*100+Z
	
	#file.write("%s\t%.0f\t%.0f\t%.2e\t%.2f\t%.2f\t%.2e\t%.0f\n" %(name,Z,cskId,j0,a,flux,eng ,Emax)) #Debug
	file.write("%s\t%.0f\t%.2f\t%.0f\t%.2f\t%.2e\n" %(name,Z,a,cskId,flux,eng*1000))

#Cierra el archivo
file.close()