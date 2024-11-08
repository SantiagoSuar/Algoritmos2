#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <cmath>
#include <climits>


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

    void ordenarCiudadesX(Ciudad *ciudades, int cantCiudades)
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

    void ordenarCiudadesY(Ciudad *ciudades, int cantCiudades)
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

    double esSolucionTrivial(Ciudad *ciudades, int cantCiudades)
    {
        double minDistancia = INT_MAX;
        for (int i = 0; i < cantCiudades; ++i)
        {
            for (int j = i + 1; j < cantCiudades; ++j)
            {
                double dist = distanciaEfectiva(ciudades[i], ciudades[j]);
                if (dist < minDistancia)
                {
                    minDistancia = dist;
                }
            }
        }
        return minDistancia;
    }

    double revisoFranja(Ciudad *ciudades, int cantCiudades, Ciudad *ciudadesOrdenadasY, int distF)
    {
        for (int i = 0; i < cantCiudades; i++)
        {
            for (int j = i + 1; j < cantCiudades && abs(ciudadesOrdenadasY[i].y - ciudadesOrdenadasY[j].y) < distF; j++)
            { // && j < cantCiudades && ciudadesOrdenadasY[j].y - ciudadesOrdenadasY[i].y < distF
                double distanciaActual = distanciaEfectiva(ciudadesOrdenadasY[i], ciudadesOrdenadasY[j]);
                if (distanciaActual < distF)
                {
                    distF = distanciaActual;
                }
            }
        }

        return distF;
    }

public:
    double DAC(Ciudad *ciudades, int cantCiudades)
    {
        if (cantCiudades <= 3)
        {
            return esSolucionTrivial(ciudades, cantCiudades);
        }
        else
        {

           ordenarCiudadesX(ciudades, cantCiudades);

            int mitad = cantCiudades / 2;
            Ciudad *ciudadesIzq = new Ciudad[mitad];
            Ciudad *ciudadesDer = new Ciudad[cantCiudades - mitad];
            for (int i = 0; i < mitad; i++)
            {
                ciudadesIzq[i] = ciudades[i];
            }
            for (int i = mitad; i < cantCiudades; i++)
            {
                ciudadesDer[i - mitad] = ciudades[i];
            }

            int distanciaIzq = DAC(ciudadesIzq, mitad);
            int distanciaDer = DAC(ciudadesDer, cantCiudades - mitad);
            int distanciaMinima = min(distanciaIzq, distanciaDer);
            Ciudad *ciudadesCercanas = new Ciudad[cantCiudades];
            int j = 0;
            for (int i = 0; i < cantCiudades; i++)
            {
                if (abs(ciudades[i].x - ciudades[mitad].x) < distanciaMinima)
                {
                    ciudadesCercanas[j] = ciudades[i];
                    j++;
                }
            }
            ordenarCiudadesY(ciudadesCercanas, j);
            int distanciaF = revisoFranja(ciudadesCercanas, j, ciudades, distanciaMinima);
            return min(distanciaMinima, distanciaF);
        }
    }
};

int main()
{
    int n;
    cin >> n;
    Ciudad *ciudades = new Ciudad[n];

    for (int i = 0; i < n; ++i)
    {
        cin >> ciudades[i].x >> ciudades[i].y >> ciudades[i].poblacion;
    }

    int minDistancia = INT_MAX;
    Puntos puntos;
    minDistancia = puntos.DAC(ciudades, n);

    Ciudad ciudad1 = ciudades[0];
    Ciudad ciudad2 = ciudades[1];
    cout << ciudad1.x << " " << ciudad1.y << " " << ciudad1.poblacion << endl;
    cout << ciudad2.x << " " << ciudad2.y << " " << ciudad2.poblacion << endl;
    return 0;
}