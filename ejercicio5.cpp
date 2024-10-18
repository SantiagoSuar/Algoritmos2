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

struct Mision
{
    string nombre;
    int id;
    Ciudad ciudad;
};

struct NodoLista
{
    Mision *mision;
    NodoLista *sig;
    NodoLista() : mision(NULL), sig(NULL) {}
};
class MinHeap
{
public:
    Ciudad *vec;
    int *costos;
    int capacidad;
    int primeroLibre;

    bool estaLleno()
    {
        return primeroLibre > capacidad;
    }

    bool estaVacio()
    {
        return primeroLibre == 1;
    }

    int padre(int pos)
    {
        return pos / 2;
    }

    int hijoIzq(int pos)
    {
        return 2 * pos;
    }

    int hijoDer(int pos)
    {
        return 2 * pos + 1;
    }
    bool comparar(const Ciudad &padre, const Ciudad &hijo)
    {
        // Prioridad por grado, si los grados son iguales, por id mayor
        if (costos[padre.id] == costos[hijo.id])
        {
            return padre.id < hijo.id;
        }
        return costos[padre.id] > costos[hijo.id]; // Menor precio tiene prioridad
    }
    void intercambiar(int pos1, int pos2)
    {
        Ciudad aux = vec[pos1];
        vec[pos1] = vec[pos2];
        vec[pos2] = aux;
    }
    void flotar(int pos)
    {
        if (pos == 1)
            return; // Ya estamos en la raíz
        int posPadre = padre(pos);
        if (comparar(vec[posPadre], vec[pos]))
        {
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }
    void hundir(int pos)
    {
        int posIzq = hijoIzq(pos);
        int posDer = hijoDer(pos);
        int menor = pos;
        if (posIzq < primeroLibre && comparar(vec[menor], vec[posIzq]))
        {
            menor = posIzq;
        }
        if (posDer < primeroLibre && comparar(vec[menor], vec[posDer]))
        {
            menor = posDer;
        }
        if (menor != pos)
        {
            intercambiar(pos, menor);
            hundir(menor);
        }
    }

public:
    MinHeap(int tam)
    {
        Ciudad *vec = new Ciudad[tam + 1]; // Posición 0 no se usa
        primeroLibre = 1;
        capacidad = tam;
    }
    ~MinHeap()
    {
        delete[] vec;
    }
    void insertar(Ciudad obj)
    {
        if (!estaLleno())
        {
            vec[primeroLibre] = obj;
            flotar(primeroLibre);
            primeroLibre++;
        }
    }
    Ciudad extraerMin()
    {
        Ciudad min = vec[1];
        vec[1] = vec[primeroLibre - 1];
        primeroLibre--;
        hundir(1);
        return min;
    }
};
class List
{
private:
    NodoLista *ppio;
    int tam;

public:
    List() : ppio(nullptr), tam(0) {}

    void InsertPpio(Mision *m)
    {
        NodoLista *nuevo = new NodoLista();
        nuevo->sig = ppio;
        nuevo->mision = m;
        ppio = nuevo;
        tam++;
    }

    bool esVacia()
    {
        return this->cantidadElementos() == 0;
    }

    unsigned int cantidadElementos()
    {
        return this->tam;
    }

    int cantidadElementos0()
    {
        int cant = 0;
        NodoLista *aux = ppio;
        while (aux != nullptr)
        {
            if (aux->mision->id == 0)
            {
                cant++;
            }
            aux = aux->sig;
        }
        return cant;
    }

    int extraerElem()
    {
        if (!esVacia())
        {
            int id = ppio->mision->id;
            NodoLista *aux = ppio;
            ppio = ppio->sig;
            delete aux;
            tam--;
            return id;
        }
        return -1;
    }
};
struct Arista
{
    Ciudad origen;
    Ciudad destino;
    int peso;
    Arista *sig;
};
class GrafoLista
{
private:
    Arista **vertices;
    int cantidadV;
    bool dirigido;
    bool ponderado;

public:
    GrafoLista(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
    }

    ~GrafoLista()
    {
        delete[] vertices;
    }

    void agregarArista(Ciudad origen, Ciudad destino, int peso)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->destino = destino;
        nuevaArista->peso = peso;
        nuevaArista->sig = vertices[origen.id];
        vertices[origen.id] = nuevaArista;

        if (!dirigido)
        {
            Arista *inversa = new Arista();
            inversa->destino = origen;
            inversa->peso = peso;
            inversa->sig = vertices[destino.id];
            vertices[destino.id] = inversa;
        }
    }

    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    int cantidadVertices()
    {
        return cantidadV;
    }
};

class camino
{
private:
    int cantMisiones;
    int cantciudades;
    Mision misiones[100]; // Asume un tamaño máximo de 100 misiones, ajusta según tus necesidades.

public:
    camino(int numCiudades)
    {
        cantciudades = numCiudades;
    }

    void LlenarVectorGrados(GrafoLista *graph, int *in_degree)
    {
        for (int i = 0; i < graph->cantidadVertices(); i++)
        {
            Arista *adyacentes = graph->adyacentes(i);
            while (adyacentes)
            {
                in_degree[adyacentes->destino.id]++;
                adyacentes = adyacentes->sig;
            }
        }
    }

    void agregarMision(int id, string nombre, Ciudad ciudad)
    {
        misiones[id].id = id;
        misiones[id].nombre = nombre;
        misiones[id].ciudad = ciudad;
    }

    string obtenerNombreMision(int id)
    {
        if (id >= 0 && id < 100)
        {
            return misiones[id].nombre;
        }
        return "Misión desconocida";
    }

    void OrdenTopologic(GrafoLista *grafo, int *in_degree, List &ordenTopologico)
    {
        List cola;

        for (int i = 0; i < grafo->cantidadVertices(); i++)
        {
            if (in_degree[i] == 0)
            {
                Mision *nueva = new Mision();
                nueva->id = i;
                nueva->nombre = obtenerNombreMision(i);
                ordenTopologico.InsertPpio(nueva);
            }
        }
    }

    void caminoMasCorto(int *costos, int *vengo, Ciudad destino, Ciudad origen, List &OrdenTopologico, GrafoLista *g, int *indegree)
    {
        bool *visitados = new bool[cantciudades + 1]();
        int costoTot = 0;
        Ciudad ciudadActual = origen;

        LlenarVectorGrados(g, indegree);
        OrdenTopologic(g, indegree, OrdenTopologico);

        while (!OrdenTopologico.esVacia())
        {
            int misionId = 0;
            LlenarVectorGrados(g, indegree);
            OrdenTopologic(g, indegree, OrdenTopologico);
            int cantOrden0 = OrdenTopologico.cantidadElementos0();

            // Llamada a Dijkstra para obtener el camino más corto desde ciudadActual
            dijsktra(ciudadActual, g, costos, vengo, visitados);

            for (int i = 1; i <= cantOrden0; i++)
            {
                int misionDisp = OrdenTopologico.extraerElem();
                int costoMin = INT16_MAX;

                if (costos[misionDisp] < costoMin)
                {
                    costoMin = costos[misionDisp];
                    misionId = misionDisp;
                }

                string camino = camino1(vengo, misionDisp, ciudadActual.id);
                costoTot += costos[misionDisp];
                ciudadActual = misiones[misionDisp].ciudad;

                cout << camino << " -> Misión: " << misiones[misionId].nombre
                     << " - " << misiones[misionId].ciudad.nombre
                     << " - Tiempo de viaje: " << costos[misionDisp] << endl;
            }
        }

        delete[] visitados;
    }

    // Método para construir el camino
    string camino1(int *vengo, int destino, int origen)
    {
        string camino;
        int actual = destino;

        while (actual != origen)
        {
            camino = " -> " + to_string(actual) + camino;
            actual = vengo[actual];
        }

        return to_string(origen) + camino;
    }

    void dijsktra(Ciudad origen, GrafoLista *g, int *costos, int *vengo, bool *visitados)
    {
        MinHeap h(cantciudades + 1);
        h.insertar(origen);
        while (!h.estaVacio())
        {
            Ciudad v = h.extraerMin();
            visitados[v.id] = true;
            Arista *w = g->adyacentes(v.id);
            while (w)
            {
                if (!visitados[w->destino.id] && costos[w->destino.id] > costos[v.id] + w->peso)
                {
                    costos[w->destino.id] = costos[v.id] + w->peso;
                    vengo[w->destino.id] = v.id;
                    h.insertar(w->destino);
                }
            }
        }
    }
};

int main()
{
    int numMisiones;
    cin >> numMisiones;
    GrafoLista grafo(numMisiones, true, false);
    Mision *misiones = new Mision[numMisiones];
    for (int i = 0; i < numMisiones; ++i)
    {
        int id, ciudadId;
        string nombre;
        cin >> id >> nombre >> ciudadId;

        misiones[id] = Mision{nombre, id, Ciudad{ciudadId, ""}};
        int i = 0;
        while (i != 0)
        {
            int misionsig;
            cin >> misionsig;
        }
        grafo.agregarArista()
    }

    int numCiudades;
    cin >> numCiudades;

    Ciudad *ciudades = new Ciudad[numCiudades];
    for (int i = 1; i <= numCiudades; ++i)
    {
        int id;
        string nombre;
        cin >> id >> nombre;
        ciudades[id] = {id, nombre};
    }

    GrafoLista grafo(numCiudades, true, true);

    int numAristas;
    cin >> numAristas;

    for (int i = 0; i < numAristas; ++i)
    {
        int origenId, destinoId, peso;
        cin >> origenId >> destinoId >> peso;
        Ciudad origen = {origenId, ciudades[origenId].nombre};
        Ciudad destino = {destinoId, ciudades[destinoId].nombre};
        grafo.agregarArista(origen, destino, peso);
    }

    // Ciudad inicial (Punta_Del_Este es la ciudad con id 6)
    int ciudadInicial = 6;
    cout << "Ciudad inicial: " << ciudades[ciudadInicial].nombre << endl;

    int tiempoTotal = 0;
    int ciudadActual = ciudadInicial;

    for (Mision *mision : misiones)
    {
        vector<int> distancias = grafo.dijkstra(ciudadActual);

        int costoViaje = distancias[mision.ciudadId];
        tiempoTotal += costoViaje;

        cout << ciudades[ciudadActual].nombre << " -> " << ciudades[mision.ciudadId].nombre
             << " -> Mision: " << mision.nombre
             << " - " << ciudades[mision.ciudadId].nombre
             << " - Tiempo de viaje: " << costoViaje << endl;

        ciudadActual = mision.ciudadId;
    }

    cout << "Misiones ejecutadas con exito." << endl;
    cout << "Tiempo total de viaje: " << tiempoTotal << endl;

    return 0;
}
