#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
struct Arista
{
    int origen;
    int destino;
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
    int dato;
    int tam;
    NodoLista *sig;
    NodoLista() : dato(0), sig(NULL) {}
    NodoLista(int d) : dato(d), sig(NULL) {}
};
class List
{
private:
    NodoLista *ppio;

public:
    void InsertPpio(int d)
    {
        NodoLista *nuevo = new NodoLista(d);
        nuevo->sig = ppio;
        ppio = nuevo;
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

    void agregarArista(int origen, int destino, int peso = 1)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->destino = destino;
        nuevaArista->peso = peso;
        nuevaArista->sig = vertices[origen];
        vertices[origen] = nuevaArista;
        if (!dirigido)
        {
            Arista *inversa = new Arista();
            inversa->destino = origen;
            inversa->peso = peso;
            inversa->sig = vertices[destino];
            vertices[destino] = inversa;
        }
    }

    void imprimir()
    {
        cout << endl
             << "Lista de adyacencia:" << endl;
        for (int i = 1; i <= cantidadV; i++)
        {
            cout << i << ": ";
            Arista *actual = adyacentes(i);
            while (actual->sig != NULL)
            {
                cout << actual->destino << " ";
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
class MinHeap
{
public:
    Objeto *vec; // Array de objetos
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

    bool comparar(const Objeto &padre, const Objeto &hijo)
    {
        // Prioridad por precio, si los precios son iguales, por id mayor
        if (padre.precio == hijo.precio)
        {
            return padre.id < hijo.id; // El id más grande tiene prioridad si los precios son iguales
        }
        return padre.precio > hijo.precio; // Menor precio tiene prioridad
    }

    void intercambiar(int pos1, int pos2)
    {
        Objeto aux = vec[pos1];
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
        vec = new Objeto[tam + 1]; // Posición 0 no se usa
        primeroLibre = 1;
        capacidad = tam;
    }

    ~MinHeap()
    {
        delete[] vec;
    }

    void insertar(Objeto obj)
    {
        if (!estaLleno())
        {
            vec[primeroLibre] = obj;
            flotar(primeroLibre);
            primeroLibre++;
        }
    }

    Objeto extraerMin()
    {
        if (estaVacio())
        {
            return {-1, -1}; // Valor especial si está vacío
        }
        Objeto min = vec[1];
        vec[1] = vec[primeroLibre - 1];
        primeroLibre--;
        hundir(1);
        return min;
    }

    void actualizar(int id, int nuevoPrecio)
    {
        // Buscar el objeto con el mismo id y actualizar su precio si es menor
        for (int i = 1; i < primeroLibre; i++)
        {
            if (vec[i].id == id)
            {
                if (nuevoPrecio < vec[i].precio)
                {
                    vec[i].precio = nuevoPrecio;
                    hundir(i); // Reajustar la posición
                    flotar(i); // Reajustar la posición
                }
                break;
            }
        }
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
                in_degree[adyacentes->destino]++;
                adyacentes = adyacentes->sig;
            }
        }
    }
    void OrdenTopologic(GrafoLista *&grafo, int *in_degree)
    {
        Cola cola;
        List ordenTopologico;
        for (int i = 0; i < grafo->cantidadVertices(); i++)
        {
            if (in_degree[i] == 0)
            {
                cola.push(i);
            }
        }
        int i = 0;
        while (!cola.empty())
        {
            int nodoActual = cola.front();
            cola.pop();
            ordenTopologico.InsertPpio(nodoActual);
            Arista *adyacentes = grafo->adyacentes(nodoActual);
            while (adyacentes)
            {
                int destino = adyacentes->destino;
                in_degree[destino]--;
                if (in_degree[destino] == 0)
                {
                    cola.push(destino);
                }
                adyacentes = adyacentes->sig;
            }
        }
        if (ordenTopologico.esVacia() != grafo->cantidadVertices())
        {
            cout << "El grafo tiene un ciclo" << endl;
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