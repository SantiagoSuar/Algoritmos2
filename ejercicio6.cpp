#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Ciudad
{
    int id;
    string nombre;
};

struct Arista
{
    Ciudad origen;
    Ciudad destino;
    int peso;
    Arista *sig;
};

class MinHeap
{
private:
    struct Nodo
    {
        int idCiudad;
        int costo;
    };
    Nodo *vec;
    int *vecPosiciones;
    int primeroLibre;
    int capacidad;

    bool comparar(const Nodo &a, const Nodo &b)
    {
        if (a.costo == b.costo)
            return a.idCiudad > b.idCiudad;
        return a.costo < b.costo;
    }

    void intercambiar(int pos1, int pos2)
    {
        Nodo temp = vec[pos1];
        vec[pos1] = vec[pos2];
        vec[pos2] = temp;

        vecPosiciones[vec[pos1].idCiudad] = pos1;
        vecPosiciones[vec[pos2].idCiudad] = pos2;
    }

    void flotar(int pos)
    {
        while (pos > 1)
        {
            int posPadre = pos / 2;
            if (comparar(vec[pos], vec[posPadre]))
            {
                intercambiar(pos, posPadre);
                pos = posPadre;
            }
            else
            {
                break;
            }
        }
    }

    void hundir(int pos)
    {
        while (2 * pos < primeroLibre)
        {
            int hijoIzq = 2 * pos;
            int hijoDer = 2 * pos + 1;
            int menor = pos;

            if (hijoIzq < primeroLibre && comparar(vec[hijoIzq], vec[menor]))
                menor = hijoIzq;
            if (hijoDer < primeroLibre && comparar(vec[hijoDer], vec[menor]))
                menor = hijoDer;

            if (menor != pos)
            {
                intercambiar(pos, menor);
                pos = menor;
            }
            else
            {
                break;
            }
        }
    }

public:
    MinHeap(int tam)
    {
        capacidad = tam;
        vec = new Nodo[capacidad + 1];
        vecPosiciones = new int[capacidad + 1]();
        primeroLibre = 1;
    }

    ~MinHeap()
    {
        delete[] vec;
        delete[] vecPosiciones;
    }

    bool esVacio()
    {
        return primeroLibre == 1;
    }

    void insertar(int idCiudad, int costo)
    {
        if (vecPosiciones[idCiudad] == 0)
        {
            vec[primeroLibre].idCiudad = idCiudad;
            vec[primeroLibre].costo = costo;
            vecPosiciones[idCiudad] = primeroLibre;
            flotar(primeroLibre);
            primeroLibre++;
        }
        else
        {
            int pos = vecPosiciones[idCiudad];
            if (costo < vec[pos].costo)
            {
                vec[pos].costo = costo;
                flotar(pos);
            }
        }
    }

    Nodo extraerMin()
    {
        Nodo minNodo = vec[1];
        vecPosiciones[minNodo.idCiudad] = 0;

        vec[1] = vec[primeroLibre - 1];
        vecPosiciones[vec[1].idCiudad] = 1;
        primeroLibre--;

        hundir(1);
        return minNodo;
    }
};

class GrafoLista
{

private:
    Arista **vertices;
    int cantidadV;
    bool dirigido;
    bool ponderado;
    string *vecCiudades;

public:
    GrafoLista(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
        vecCiudades = new string[cantV + 1]();
    }

    ~GrafoLista()
    {
        for (int i = 1; i <= cantidadV; i++)
        {
            Arista *actual = vertices[i];
            while (actual)
            {
                Arista *temp = actual;
                actual = actual->sig;
                delete temp;
            }
        }
        delete[] vertices;
        delete[] vecCiudades;
    }

    GrafoLista *copiarGrafo(bool d)
{
    GrafoLista *nuevoGrafo = new GrafoLista(cantidadV, d, ponderado);
    for (int i = 1; i <= cantidadV; i++)
    {
        nuevoGrafo->setNombreCiudad(i, vecCiudades[i]);

        Arista *actual = vertices[i];
        while (actual)
        {
            if (d || actual->destino.id > actual->origen.id)
            {
                Ciudad origen = {actual->origen.id, actual->origen.nombre};
                Ciudad destino = {actual->destino.id, actual->destino.nombre};
                int peso = actual->peso;
                nuevoGrafo->agregarArista(origen, destino, peso);
            }

            actual = actual->sig;
        }
    }
    return nuevoGrafo;
}


    void agregarArista(Ciudad origen, Ciudad destino, int peso)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->origen = origen;
        nuevaArista->destino = destino;
        nuevaArista->peso = peso;
        nuevaArista->sig = vertices[origen.id];
        vertices[origen.id] = nuevaArista;

        Arista *inversa = new Arista();
        inversa->origen = destino;
        inversa->destino = origen;
        inversa->peso = peso;
        inversa->sig = vertices[destino.id];
        vertices[destino.id] = inversa;
    }

    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    void multiplicoArista(int destino, int *vengoDe)
    {
        while (vengoDe[destino] != destino)
        {
            Arista *aux = vertices[vengoDe[destino]];
            while (aux && aux->destino.id != destino)
            {
                aux = aux->sig;
            }
            if (aux)
            {
                aux->peso *= 2;
            }

            Arista *inverso = vertices[destino];
            while (inverso && inverso->destino.id != vengoDe[destino])
            {
                inverso = inverso->sig;
            }
            if (inverso)
            {
                inverso->peso *= 2;
            }

            destino = vengoDe[destino];
        }
        return;
    }

    string camino(int destino, int *vengoDe)
    {
        if (vengoDe[destino] == destino)
        {
            return vecCiudades[destino] + " -> ";
        }
        else
        {
            return camino(vengoDe[destino], vengoDe) + vecCiudades[destino] + " -> ";
        }
    }

    void setNombreCiudad(int id, string nombre)
    {
        vecCiudades[id] = nombre;
    }

    string *getVecCiudades()
    {
        return vecCiudades;
    }

};

class caminoDijkstra
{

private:
    int cantciudades;

public:
    caminoDijkstra(int numCiudades)
    {
        cantciudades = numCiudades;
    }

    void dijsktra(Ciudad &origen, GrafoLista *g, int *&costos, int *&vengo, bool *&visitados)
    {
        MinHeap h(cantciudades + 1);
        h.insertar(origen.id, 0);
        costos[origen.id] = 0;
        vengo[origen.id] = origen.id;

        while (!h.esVacio())
        {
            int ciudadActual = h.extraerMin().idCiudad;

            if (visitados[ciudadActual])
                continue;

            visitados[ciudadActual] = true;

            Arista *aux = g->adyacentes(ciudadActual);
            while (aux)
            {
                int destino = aux->destino.id;
                int nuevoCosto = costos[ciudadActual] + aux->peso;
                if (!visitados[destino] && nuevoCosto < costos[destino])
                {
                    costos[destino] = nuevoCosto;
                    vengo[destino] = ciudadActual;
                    h.insertar(destino, nuevoCosto);
                }
                aux = aux->sig;
            }
        }
    }
};

void reinicio(int *&pesos, int *&vengoDe, bool *&visitados, int cantCiudades)
{
    for (int i = 1; i <= cantCiudades; i++)
    {
        pesos[i] = 999999;
        vengoDe[i] = -1;
        visitados[i] = false;
    }
}

int main()
{
    int cantCiudades;
    cin >> cantCiudades;
    string *ciudades = new string[cantCiudades + 1]();
    for (int i = 0; i < cantCiudades; i++)
    {
        int id;
        string nombre;
        cin >> id >> nombre;
        ciudades[id] = nombre;
    }

    GrafoLista grafo(cantCiudades, false, true);
    for (int i = 1; i <= cantCiudades; i++)
    {
        grafo.setNombreCiudad(i, ciudades[i]);
    }

    int morfeo;
    cin >> morfeo;
    Ciudad ciudadMorfeo;
    ciudadMorfeo.id = morfeo;
    ciudadMorfeo.nombre = ciudades[morfeo];

    int entidad;
    cin >> entidad;
    Ciudad ciudadEntidad;
    ciudadEntidad.id = entidad;
    ciudadEntidad.nombre = ciudades[entidad];

    int equipo;
    cin >> equipo;
    Ciudad ciudadEquipo;
    ciudadEquipo.id = equipo;
    ciudadEquipo.nombre = ciudades[equipo];

    int extraccion;
    cin >> extraccion;
    Ciudad ciudadExtraccion;
    ciudadExtraccion.id = extraccion;
    ciudadExtraccion.nombre = ciudades[extraccion];

    int numAristas;
    cin >> numAristas;

    for (int i = 0; i < numAristas; i++)
    {
        int origenId, destinoId, peso;
        cin >> origenId >> destinoId >> peso;
        Ciudad origen = {origenId, ciudades[origenId]};
        Ciudad destino = {destinoId, ciudades[destinoId]};
        grafo.agregarArista(origen, destino, peso);
    }

    GrafoLista *grafo1 = grafo.copiarGrafo(false);
    GrafoLista *grafo2 = grafo.copiarGrafo(false);

    int *pesosEntidad = new int[cantCiudades + 1];
    int *vengoDeEntidad = new int[cantCiudades + 1];
    bool *visitadosEntidad = new bool[cantCiudades + 1]();
    for (int i = 1; i <= cantCiudades; i++)
    {
        pesosEntidad[i] = 999999;
        vengoDeEntidad[i] = -1;
    }

    int *pesosEquipo = new int[cantCiudades + 1];
    int *vengoDeEquipo = new int[cantCiudades + 1];
    bool *visitadosEquipo = new bool[cantCiudades + 1]();
    for (int i = 1; i <= cantCiudades; i++)
    {
        pesosEquipo[i] = 999999;
        vengoDeEquipo[i] = -1;
    }

    caminoDijkstra c1(cantCiudades);
    caminoDijkstra c2(cantCiudades);

    c1.dijsktra(ciudadMorfeo, grafo1, pesosEntidad, vengoDeEntidad, visitadosEntidad);
    int destino1 = entidad;
    string cam1paso1 = grafo1->camino(destino1, vengoDeEntidad);
    grafo1->multiplicoArista(destino1, vengoDeEntidad);

    int costo1 = pesosEntidad[destino1];

    reinicio(pesosEntidad, vengoDeEntidad, visitadosEntidad, cantCiudades);

    c1.dijsktra(ciudadEntidad, grafo1, pesosEntidad, vengoDeEntidad, visitadosEntidad);
    destino1 = equipo;
    string cam1paso2 = grafo1->camino(destino1, vengoDeEntidad);
    grafo1->multiplicoArista(destino1, vengoDeEntidad);
    costo1 += pesosEntidad[destino1];

    reinicio(pesosEntidad, vengoDeEntidad, visitadosEntidad, cantCiudades);

    c1.dijsktra(ciudadEquipo, grafo1, pesosEntidad, vengoDeEntidad, visitadosEntidad);
    destino1 = extraccion;
    string cam1paso3 = grafo1->camino(destino1, vengoDeEntidad);
    grafo1->multiplicoArista(destino1, vengoDeEntidad);
    costo1 += pesosEntidad[destino1];

    c2.dijsktra(ciudadMorfeo, grafo2, pesosEquipo, vengoDeEquipo, visitadosEquipo);
    destino1 = equipo;
    string cam2paso1 = grafo2->camino(destino1, vengoDeEquipo);
    grafo2->multiplicoArista(destino1, vengoDeEquipo);
    int costo2 = pesosEquipo[destino1];

    reinicio(pesosEquipo, vengoDeEquipo, visitadosEquipo, cantCiudades);

    c2.dijsktra(ciudadEquipo, grafo2, pesosEquipo, vengoDeEquipo, visitadosEquipo);
    destino1 = entidad;
    string cam2paso2 = grafo2->camino(destino1, vengoDeEquipo);
    grafo2->multiplicoArista(destino1, vengoDeEquipo);
    costo2 += pesosEquipo[destino1];

    reinicio(pesosEquipo, vengoDeEquipo, visitadosEquipo, cantCiudades);

    c2.dijsktra(ciudadEntidad, grafo2, pesosEquipo, vengoDeEquipo, visitadosEquipo);
    destino1 = extraccion;
    string cam2paso3 = grafo2->camino(destino1, vengoDeEquipo);
    grafo2->multiplicoArista(destino1, vengoDeEquipo);
    costo2 += pesosEquipo[destino1];

    string tarea1 = "Desactivar la Entidad";
    string tarea2 = "buscar equipo";
    string paso11 = "Desactivar la Entidad";
    string paso12 = "Buscar equipo";
    string paso1 = cam1paso1;
    string paso2 = cam1paso2;
    string paso3 = cam1paso3;

    int opcion1 = costo1;
    int opcion2 = costo2;

    if (costo1 > costo2)
    {
        tarea1 = "buscar equipo";
        tarea2 = "Desactivar la Entidad";
        paso11 = "Buscar equipo";
        paso12 = "Desactivar la Entidad";
        paso1 = cam2paso1;
        paso2 = cam2paso2;
        paso3 = cam2paso3;
        opcion1 = costo2;
        opcion2 = costo1;
    }

    cout << "BE11, la mejor ruta es " << tarea1 << ", " << tarea2 << " y punto de extraccion con un costo de " << opcion1 << endl;
    cout << "La otra opcion tiene un costo de " << opcion2 << endl;
    cout << "Paso 1: " << paso1 << paso11 << endl;
    cout << "Paso 2: " << paso2 << paso12 << endl;
    cout << "Paso 3: " << paso3 << "Ir a Punto de extraccion" << endl;

    return 0;
}