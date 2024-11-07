#include <cassert>
#include <string>
#include <iostream>
#include <climits>

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
    bool comparar(const Ciudad &padre, const Ciudad &hijo, int *costos)
    {
        if (costos[padre.id] == costos[hijo.id])
        {
            return padre.id > hijo.id; // Menor id tiene prioridad si los costos son iguales
        }
        return costos[padre.id] > costos[hijo.id]; // Menor costo tiene prioridad
    }
    void intercambiar(int pos1, int pos2)
    {
        Ciudad aux = vec[pos1];
        vec[pos1] = vec[pos2];
        vec[pos2] = aux;
    }
    void flotar(int pos, int *costos)
    {
        if (pos == 1)
            return; 
        int posPadre = padre(pos);
        if (comparar(vec[posPadre], vec[pos], costos))
        {
            intercambiar(posPadre, pos);
            flotar(posPadre, costos);
        }
    }
    void hundir(int pos, int *costos)
    {
        int posIzq = hijoIzq(pos);
        int posDer = hijoDer(pos);
        int menor = pos;
        if (posIzq < primeroLibre && comparar(vec[menor], vec[posIzq], costos))
        {
            menor = posIzq;
        }
        if (posDer < primeroLibre && comparar(vec[menor], vec[posDer], costos))
        {
            menor = posDer;
        }
        if (menor != pos && comparar(vec[pos], vec[menor], costos))
        {
            intercambiar(pos, menor);
            hundir(menor, costos);
        }
    }

public:
    MinHeap(int tam)
    {
        vec = new Ciudad[tam + 1](); // Posición 0 no se usa
        primeroLibre = 1;
        capacidad = tam;
    }
    ~MinHeap()
    {
        delete[] vec;
    }
    void insertar(Ciudad &obj, int *costos)
    {
        if (!estaLleno())
        {

            vec[primeroLibre] = obj;
            flotar(primeroLibre, costos);
            primeroLibre++;
        }
    }
    Ciudad extraerMin(int *costos)
    {
        Ciudad min = vec[1];
        vec[1] = vec[primeroLibre - 1];
        primeroLibre--;
        hundir(1, costos);
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
    GrafoLista *copiarGrafo()
    {
        GrafoLista *nuevoGrafo = new GrafoLista(cantidadV, dirigido, ponderado);
        for (int i = 1; i <= cantidadV; i++)
        {
            Arista *actual = vertices[i];
            while (actual)
            {
                Ciudad origen = {actual->origen.id, actual->origen.nombre};
                Ciudad destino = {actual->destino.id, actual->destino.nombre};
                int peso = actual->peso;
                nuevoGrafo->agregarArista(origen, destino, peso);

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

        if (!dirigido)
        {
            Arista *inversa = new Arista();
            inversa->origen = destino;
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
struct Arista2
{
    Mision origen;
    Mision destino;
    Arista2 *sig;
};
class GrafoM
{

private:
    Arista2 **vertices;
    int cantidadV;
    bool dirigido;
    bool ponderado;

public:
    GrafoM(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista2 *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
    }

    ~GrafoM()
    {
        delete[] vertices;
    }

    void agregarArista(Mision origen, Mision destino)
    {
        Arista2 *nuevaArista = new Arista2();
        nuevaArista->destino = destino;
        nuevaArista->sig = vertices[origen.id];
        vertices[origen.id] = nuevaArista;

        if (!dirigido)
        {
            Arista2 *inversa = new Arista2();
            inversa->destino = origen;
            inversa->sig = vertices[destino.id];
            vertices[destino.id] = inversa;
        }
    }

    Arista2 *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    int cantidadVertices()
    {
        return cantidadV;
    }
    void eliminarVertice(int vertice)
    {
        vertices[vertice] = nullptr;
    }
};
class camino
{
private:
    int cantMisiones;
    int cantciudades;
    Mision misiones[200]; 

public:
    camino(int numCiudades)
    {
        cantciudades = numCiudades;
    }

    void LlenarVectorGrados(GrafoM *g, int *in_degree)
    {
        for (int i = 1; i <= g->cantidadVertices(); i++)
        {
            Arista2 *adyacentes = g->adyacentes(i);
            while (adyacentes)
            {
                in_degree[adyacentes->destino.id]++;
                adyacentes = adyacentes->sig;
            }
            
        }
    }

    string obtenerNombreMision(int id, Ciudad *ciudades)
    {
        if (id >= 0 && id < 100)
        {
            return ciudades[id].nombre;
        }
        return "Misión desconocida";
    }

    void OrdenTopologic(GrafoM *grafo, int *in_degree, List &ordenTopologico)
    {
        List cola;

        for (int i = 1; i <= grafo->cantidadVertices(); i++)
        {
            if (in_degree[i] == 0)
            {
                Mision *nueva = new Mision();
                nueva->id = i;
                ordenTopologico.InsertPpio(nueva);
            }
        }
    }

    void caminoMasCorto(int *costos, int *vengo, Ciudad &origen, List &OrdenTopologico, GrafoM *f, int *indegree, GrafoLista *g, Mision *misiones, Ciudad *ciudades)
    {
        bool *visitados = new bool[cantciudades + 1]();
        int costoTot = 0;             // Costo total de las misiones
        Ciudad ciudadActual = origen; // Ciudad actual donde comienza

        LlenarVectorGrados(f, indegree);
        OrdenTopologic(f, indegree, OrdenTopologico);
               cout << "Ciudad inicial: " << ciudadActual.nombre << endl;

        while (!OrdenTopologico.esVacia())
        {
            int cantOrden0 = OrdenTopologico.cantidadElementos(); 

            for (int i = 1; i <= cantciudades; i++)
            {
                costos[i] = INT_MAX;
                vengo[i] = -1;
                visitados[i] = false;
            }
            costos[ciudadActual.id] = 0;
           
           
            dijsktra(ciudadActual, g, costos, vengo, visitados);

            int misionId = 0;         
            int costoMin = INT_MAX; 

            for (int i = 1; i <= cantOrden0; i++)
            {
                int misionDisp = OrdenTopologico.extraerElem();
                int misionciudad = misiones[misionDisp].ciudad.id;
               
             
                if (costos[misionciudad] < costoMin)
                {
                    costoMin = costos[misionciudad];
                    misionId = misionDisp;
                }
            }
           
            Arista2 *ady = f->adyacentes(misionId);
            while (ady)
            {
                indegree[ady->destino.id]--; // Reducir el grado de entrada del destino
                ady = ady->sig;      
            }
            
            f->eliminarVertice(misionId);
            indegree[misionId] = -1; 
       
            
            string camino = camino1(vengo, misiones[misionId].ciudad.id, ciudadActual.id, ciudades);

          
            costoTot += costos[misiones[misionId].ciudad.id];

            ciudadActual = misiones[misionId].ciudad;
            
            cout << camino << " -> Mision: " << misiones[misionId].nombre
                 << " - " << obtenerNombreMision(misiones[misionId].ciudad.id,ciudades)
                 << " - Tiempo de viaje: " << costos[misiones[misionId].ciudad.id] << endl;
            
            OrdenTopologic(f, indegree, OrdenTopologico); 
        }
        cout << "Misiones ejecutadas con exito." <<endl;
            cout << "Tiempo total de viaje: " << costoTot << endl;
    }

    string camino1(int *vengo, int destino, int origen, Ciudad *ciudades)
    {
        string camino;
        int actual = destino;

        while (actual != origen)
        {
            camino = " -> " + ciudades[actual].nombre + camino;
            actual = vengo[actual];
        }

        return ciudades[origen].nombre + camino;
    }

    void dijsktra(Ciudad &origen, GrafoLista *g, int *costos, int *vengo, bool *visitados)
    {
        MinHeap h(cantciudades*cantciudades);
        h.insertar(origen, costos);
        while (!h.estaVacio())
        {
            Ciudad v = h.extraerMin(costos);
            visitados[v.id] = true;
            Arista *w = g->adyacentes(v.id);
            Arista *aux = w;
            while (aux)
            {
                if (costos[aux->destino.id] > costos[v.id] + aux->peso )
                   
                {
                    costos[aux->destino.id] = costos[v.id] + aux->peso;
                    vengo[aux->destino.id] = v.id;

                    h.insertar(aux->destino, costos);
                }
                aux = aux->sig;
            
            }
        }
    }
};

int main()
{
    int numMisiones;
    cin >> numMisiones;

    GrafoM grafoMisiones(numMisiones, true, false);
    Mision *misiones = new Mision[numMisiones + 1];
    for (int i = 1; i <= numMisiones; ++i)
    {
        int id, ciudadId;
        string nombre;
        cin >> id >> nombre >> ciudadId;

        misiones[id] = {nombre, id, ciudadId};

        int misionSiguiente;
        while (cin >> misionSiguiente && misionSiguiente != 0)
        {
            Mision origen = {nombre, id};
            Mision destino = {misiones[misionSiguiente].nombre, misionSiguiente};
            grafoMisiones.agregarArista(origen, destino);
        }
    }

    int numCiudades;
    int CuidadInicial;
    cin >> numCiudades >> CuidadInicial;

    Ciudad *ciudades = new Ciudad[numCiudades + 1];
    for (int i = 1; i <= numCiudades; ++i)
    {
        int id;
        string nombre;
        cin >> id >> nombre;
        ciudades[id] = {id, nombre};
    }

    GrafoLista* grafoCiudades = new GrafoLista(numCiudades, false, true);
    int numAristas;
    cin >> numAristas;
    for (int i = 0; i < numAristas; ++i)
    {
        int origenId, destinoId, peso;
        cin >> origenId >> destinoId >> peso;
        Ciudad origen = {origenId, ciudades[origenId].nombre};
        Ciudad destino = {destinoId, ciudades[destinoId].nombre};
        grafoCiudades->agregarArista(origen, destino, peso);
    }

    int *costos = new int[numCiudades + 1];
    int *vengo = new int[numCiudades + 1];
    bool *visitados = new bool[numCiudades + 1]();
    for (int i = 1; i <= numCiudades; i++)
    {
        costos[i] = INT_MAX;
        vengo[i] = -1;
    }

    Ciudad ciudadInicial = {CuidadInicial, ciudades[CuidadInicial].nombre};
    costos[ciudadInicial.id] = 0;
    camino c(numCiudades);
    List OrdenTopologico;
    int *indegree = new int[numMisiones + 1]();


    c.caminoMasCorto(costos, vengo, ciudadInicial, OrdenTopologico, &grafoMisiones, indegree, grafoCiudades, misiones, ciudades);

   
    delete[] indegree;
    delete[] misiones;
    delete[] ciudades;
    delete[] costos;
    delete[] vengo;
    delete[] visitados;

    return 0;
}