#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <cmath>
 
using namespace std;
 
struct Ciudad
{
    int x;
    int y;
    int poblacion;
};
 
class Puntos
{
private:
    int cantCiudades;
    Ciudad *ciudades;
    Ciudad *sol;
 
    int distanciaEfectiva(Ciudad c1, Ciudad c2)
    {
        return sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)) + (abs(c2.poblacion - c1.poblacion) / max(c2.poblacion, c1.poblacion));
    }
 
    void ordenarCiudadesX()
    {
        int n = cantCiudades;
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (ciudades[j].x > ciudades[j + 1].x)
                {
                    Ciudad temp = ciudades[j];
                    ciudades[j] = ciudades[j + 1];
                    ciudades[j + 1] = temp;
                }
            }
        }
    }
 
   
 void ordenarCiudadesY()
    {
        int n = cantCiudades;
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (ciudades[j].y > ciudades[j + 1].y)
                {
                    Ciudad temp = ciudades[j];
                    ciudades[j] = ciudades[j + 1];
                    ciudades[j + 1] = temp;
                }
            }
        }
    }

    int esSolucionTrivial(Ciudad * vec){
        if(cantCiudades == 2){
            return distanciaEfectiva(vec[0], vec[1]);
        }
        else if (cantCiudades == 3){
            return max(distanciaEfectiva(vec[0], vec[1]),distanciaEfectiva(vec[1], vec[2]) ,distanciaEfectiva(vec[2], vec[0]));
        }
        else{
            return false;
        }

        
    }
    
    int revisoFranja(Ciudad *ciudades, int cantCiudades, Ciudad *ciudadesOrdenadasY, int distF){
        for(int i = 0; i < cantCiudades; i++){
            for(int j = i+1; j < cantCiudades && abs(ciudadesOrdenadasY[i].y - ciudadesOrdenadasY[j].y) < distF; j++){ // && j < cantCiudades && ciudadesOrdenadasY[j].y - ciudadesOrdenadasY[i].y < distF
                    double distanciaActual = distanciaEfectiva(ciudadesOrdenadasY[i], ciudadesOrdenadasY[j]);
                    if(distanciaActual < distF){
                        distF = distanciaActual;
                    }
                }
            }
        
        return distF;
    }


public:
    int DAC(Ciudad *ciudades, int cantCiudades){
        if(cantCiudades==2 || cantCiudades==3){
            return esSolucionTrivial(ciudades);
        }
        else{
            ordenarCiudadesX();
            int mitad = cantCiudades/2;
            Ciudad *ciudadesIzq = new Ciudad[mitad];
            Ciudad *ciudadesDer = new Ciudad[cantCiudades-mitad];
            for(int i = 0; i < mitad; i++){
                ciudadesIzq[i] = ciudades[i];
            }
            for(int i = mitad; i < cantCiudades; i++){
                ciudadesDer[i-mitad] = ciudades[i];
            }
            int distanciaIzq = DAC(ciudadesIzq, mitad);
            int distanciaDer = DAC(ciudadesDer, cantCiudades-mitad);
            int distanciaMinima = min(distanciaIzq, distanciaDer);
            Ciudad *ciudadesCercanas = new Ciudad[cantCiudades];
            int j = 0;
            for(int i = 0; i < cantCiudades; i++){
                if(abs(ciudades[i].x - ciudades[mitad].x) < distanciaMinima){
                    ciudadesCercanas[j] = ciudades[i];
                    j++;
                }
            }
            ordenarCiudadesY();
            int distanciaF = revisoFranja(ciudadesCercanas, j, ciudades, distanciaMinima);
            return min(distanciaMinima, distanciaF);
            
            
        }
 
       
    }
 
 
 
};
 
int main()
{
    // TODO
    return 0;
}