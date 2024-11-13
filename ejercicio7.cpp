#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
struct Cancion
{
    int duracion;
};
class greedy
{
public:
    int *canciones;
    int estudiantes;
    int n;
    int tiempoprom()
    {
        int promedio = 0;
        for (int i = 0; i < estudiantes; i++)
        {
            promedio += canciones[i];
        }
        return promedio / estudiantes;
    }
    void distribuir()
    {
        int promedio = tiempoprom();
        int est = 0;
        int tiempo_total = 0;
        int inicio = 1;
        for (int i = 0; i < n+1 ; i++)
        {
            tiempo_total += canciones[i];
            if (tiempo_total > promedio)
            {
                cout << "Estudiante " << est + 1 << " escucha canciones desde " << inicio << " hasta " << i << " con un tiempo total de " << tiempo_total - canciones[i] << " minutos" << endl;
                tiempo_total = canciones[i];
                inicio = i + 1;
                est++;
            }
            if (est == estudiantes - 1)
            {
                break;
            }
        }
        cout << "Estudiante " << est + 1 << " escucha canciones desde " << inicio << " hasta " << estudiantes << " con un tiempo total de " << tiempo_total << " minutos" << endl;
    }

bool esSolucion(int tiempo) {
    int est = 1;
    int tiempo_total = 0;
    for (int i = 1; i < n; i++) {
        if (canciones[i] > tiempo) {
            return false;
        }
        if (tiempo_total + canciones[i] > tiempo) {
            est++;
            tiempo_total = canciones[i];
            if (est > estudiantes) {
                return false;
            }
        } else {
            tiempo_total += canciones[i];
        }
    }
    return true;
}

int busquedaBinaria() {
    int low = 0;
    int high = 0;
    for (int i = 1; i < n; i++) {
        high += canciones[i];
    }
    int result = high;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (esSolucion(mid)) {
            result = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return result;
}

void imprimirDistribucion(int tiempo) {
    int fin = 0;
    int tiempo_total = 0;
    int inicio = 1;
    for (int i = 1; i < n; i++) {
        if (tiempo_total + canciones[i] > tiempo) {
            cout << tiempo_total  << " " << inicio  << " " << fin<< endl;
            tiempo_total = canciones[i];
            fin++;
            inicio = i;
        } else {
            tiempo_total += canciones[i];
            fin++;
        }
    }
    
            cout << tiempo_total  << " " << inicio  << " " << fin << endl;
}
};
int main()
{
    int n;
    cin >> n;
    n++;
    int canciones[n];
    for (int i = 1; i < n; i++)
    {
        cin >> canciones[i];
    }
    int m;
    cin >> m;
    greedy g;
    g.canciones = canciones;
    g.estudiantes = m;
    g.n = n;
    
    int tiempo_minimo_maximo = g.busquedaBinaria();
    g.imprimirDistribucion(tiempo_minimo_maximo);
    cout << tiempo_minimo_maximo << endl;
    return 0;
}