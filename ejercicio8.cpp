#include <iostream>
#include <cmath>
#include <climits>
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
    Ciudad *sol;
    double minDistancia;

    double distanciaEfectiva(Ciudad c1, Ciudad c2)
    {
        return sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)) +
               ((double)abs(c2.poblacion - c1.poblacion) / max(c2.poblacion, c1.poblacion));
    }

    int sumPoblacion(Ciudad c1, Ciudad c2)
    {
        return c1.poblacion + c2.poblacion;
    }

    void ordenarCiudadesX(Ciudad *&ciudades, int cantCiudades)
    {
        // Puedes reemplazar este bubble sort con std::sort para mayor eficiencia
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

    void ordenarCiudadesY(Ciudad *&ciudades, int cantCiudades)
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

    void esSolucionTrivial(Ciudad *ciudades, int cantCiudades)
    {
        for (int i = 0; i < cantCiudades; i++)
        {
            for (int j = i + 1; j < cantCiudades; j++)
            {
                double dist = distanciaEfectiva(ciudades[i], ciudades[j]);
                if (dist < minDistancia ||
                    (dist == minDistancia && sumPoblacion(ciudades[i], ciudades[j]) > sumPoblacion(sol[0], sol[1])))
                {
                    minDistancia = dist;
                    sol[0] = ciudades[i];
                    sol[1] = ciudades[j];
                }
            }
        }
    }

    void revisoFranja(Ciudad *ciudades, int cantCiudades)
    {
        for (int i = 0; i < cantCiudades; i++)
        {
            for (int j = i + 1; j < cantCiudades && (ciudades[j].y - ciudades[i].y) < minDistancia; j++)
            {
                double distanciaActual = distanciaEfectiva(ciudades[i], ciudades[j]);
                if (distanciaActual < minDistancia ||
                    (distanciaActual == minDistancia && sumPoblacion(ciudades[i], ciudades[j]) > sumPoblacion(sol[0], sol[1])))
                {
                    minDistancia = distanciaActual;
                    sol[0] = ciudades[i];
                    sol[1] = ciudades[j];
                }
            }
        }
    }

public:
    Puntos()
    {
        sol = new Ciudad[2];
        minDistancia = numeric_limits<double>::max();
    }

    ~Puntos()
    {
        delete[] sol;
    }

    void DAC(Ciudad *ciudades, int cantCiudades)
    {
        if (cantCiudades <= 3)
        {
            esSolucionTrivial(ciudades, cantCiudades);
        }
        else
        {
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

            DAC(ciudadesIzq, mitad);
            DAC(ciudadesDer, cantCiudades - mitad);

            Ciudad *ciudadesCercanas = new Ciudad[cantCiudades];
            int j = 0;
            for (int i = 0; i < cantCiudades; i++)
            {
                if (abs(ciudades[i].x - ciudades[mitad].x) <= minDistancia)
                {
                    ciudadesCercanas[j] = ciudades[i];
                    j++;
                }
            }
            ordenarCiudadesY(ciudadesCercanas, j);
            revisoFranja(ciudadesCercanas, j);

            delete[] ciudadesIzq;
            delete[] ciudadesDer;
            delete[] ciudadesCercanas;
        }
    }

    Ciudad *getSolucion()
    {
        return sol;
    }

    void ordenarCiudadesXInicial(Ciudad *ciudades, int cantCiudades)
    {
        ordenarCiudadesX(ciudades, cantCiudades);
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

    Puntos puntos;
    puntos.ordenarCiudadesXInicial(ciudades, n);
    puntos.DAC(ciudades, n);

    Ciudad ciudad1 = puntos.getSolucion()[0];
    Ciudad ciudad2 = puntos.getSolucion()[1];

    // Asegurar que la ciudad con menor x se imprima primero
    if (ciudad1.x > ciudad2.x || (ciudad1.x == ciudad2.x && ciudad1.y > ciudad2.y))
    {
        // Intercambiar ciudad1 y ciudad2
        Ciudad temp = ciudad1;
        ciudad1 = ciudad2;
        ciudad2 = temp;
    }

    cout << ciudad1.x << " " << ciudad1.y << " " << ciudad1.poblacion << endl;
    cout << ciudad2.x << " " << ciudad2.y << " " << ciudad2.poblacion << endl;

    delete[] ciudades;
    return 0;
}
