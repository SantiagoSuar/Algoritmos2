#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
struct Arista
{
    Ciudad origen;
    Ciudad destino;
    int peso;
    Arista *sig;
};
struct Ciudad
{
    string nombre;
    int id;
};

struct Mision
{
    string nombre;
    int id;
};
struct NodoLista 
{
    Mision* mision;
    int tam;
    NodoLista *sig;
    NodoLista() : mision(NULL), sig(NULL) {}
};
class List
{
private:
    NodoLista *ppio;

public:
    void InsertPpio(Mision* m)
    {
        NodoLista *nuevo = new NodoLista();
        nuevo->sig = ppio;
        nuevo -> mision = m; 
        ppio = nuevo;
        ppio->tam++;
    }
    bool esVacia(NodoLista *l)
    {
        return l->tam == 0;
    }
    unsigned int cantidadElementos(NodoLista *l)
    {
        return l->tam;
    }
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

    void imprimir()
    {
        cout << endl << "Lista de adyacencia:" << endl;
        for (int i = 1; i <= cantidadV; i++)
        {
            cout << i << ": ";
            Arista *actual = adyacentes(i);
            while (actual->sig != NULL)
            {
                cout << actual->destino.nombre << " ";
                if (ponderado)
                {
                    cout << "(" << actual->peso << ") ";
                }
                if (actual->sig->sig)
                {
                    cout << "-> ";
                }
                actual = actual->sig;
            }
            cout << endl;
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

class Cola
{
private:
    NodoLista* ppio;
    NodoLista* fin;

public:
    Cola()
    {
        ppio = NULL;
        fin = NULL;
    }

    void push(int d, string s)
    {
        NodoLista* nuevo = new NodoLista();
        nuevo->mision->id = d;
        if (ppio == NULL)
        {
            ppio = nuevo;
            fin = nuevo;
        }
        else
        {
            fin->sig = nuevo;
            fin = nuevo;
        }
    }

    bool empty()
    {
        return ppio == NULL;
    }

    int front()
    {
        if (ppio == NULL)
        {
            return -1;
        }
        return ppio->mision->id;
    }

    void pop()
    {
        if (ppio == NULL)
        {
            return;
        }
        NodoLista* temp = ppio;
        ppio = ppio->sig;
        delete temp;
    }
};

class MinHeap
{
public:
    Ciudad *vec; 
    int* costos;
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
        Ciudad* vec = new Ciudad[tam + 1]; // Posición 0 no se usa
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

class camino
{
private:
    int cantMisiones;
    int cantciudades;
    int *posiciones = new int[51]();

public:
    void LlenarVectorGrados(GrafoLista *&graph, int *in_degree)
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
    Mision misiones[100];  // Asume un tamaño máximo de 100 misiones, ajusta según tus necesidades.

void agregarMision(int id, string nombre) {
    misiones[id].id = id;
    misiones[id].nombre = nombre;
}

string obtenerNombreMision(int id) {
    if (id >= 0 && id < 100) {  // Verifica que el ID esté dentro del rango
        return misiones[id].nombre;
    }
    return "Misión desconocida";
}


void OrdenTopologic(GrafoLista *&grafo, int* in_degree)
{
    Cola cola;
    List ordenTopologico;

    // Agregar todos los vértices con grado de entrada 0 a la cola
    for (int i = 0; i < grafo->cantidadVertices(); i++)
    {
        if (in_degree[i] == 0)
        {
            cola.push(i);
        }
    }

    while (!cola.empty())
    {
        int nodoActual = cola.front();
        cola.pop();

        // Crear nueva misión y asignarle el ID y nombre
        Mision* nueva = new Mision();
        nueva->id = nodoActual;
        nueva->nombre = obtenerNombreMision(nodoActual);  // Obtener el nombre desde la tabla

        ordenTopologico.InsertPpio(nueva);  // Insertar la misión en la lista de orden topológico

        Arista* adyacentes = grafo->adyacentes(nodoActual);

        while (adyacentes)
        {
            int destino = adyacentes->destino.id;  // Obtener el ID del destino correctamente
            in_degree[destino]--;
            if (in_degree[destino] == 0)
            {
                cola.push(destino);
            }
            adyacentes = adyacentes->sig;
        }
    }
}

    void dijsktra(int origen, GrafoLista *g)
    {
        int *vengo = new int[cantciudades + 1]();
        int *costos = new int[cantciudades + 1]();
        bool *visitados = new bool[cantciudades + 1]();
        MinHeap h(cantciudades + 1);
        h.insertar(origen);
        while (!h.estaVacio())
        {
            int v = h.extraerMin();
            visitados[v] = true;
            Arista *w = g->adyacentes(v);
            while (w)
            {
                if (!visitados[w->destino] && costos[w->destino] > costos[v] + w->peso)
                {
                    costos[w->destino] = costos[v] + w->peso;
                    vengo[w->destino] = v;
                    h.insertar(w->destino);
                }
            }
        }
    }
    string camino1(int *vengo, int destino, int origen)
    {
        if (destino == origen)
        {
            cout << origen << " ";
        }
        else if (vengo[destino] == 0)
        {
            cout << "No hay camino" << endl;
        }
        else
        {
            camino1(vengo, vengo[destino], origen);
            cout << destino << " ";
        }
    }
    void caminoMasCorto(int *costos, int *vengo, int destino, int origen, List OrdenTopologico, GrafoLista *g, int *indegree)
    {
        OrdenTopologic(g, indegree);
        for (int i = 1; i <= cantMisiones; i++)
        {
            int misionDisp = OrdenTopologico.extraerElem();
            dijsktra(origen, g);
            string camino = "";
            camino = camino1(vengo, misionDisp, origen);
            cout << camino << endl;
            origen=misionDisp;
        }
    }
};

int main()
{
    // TODO
    return 0;
}   