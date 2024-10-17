#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Objeto
{
    int id;
    int prioridad;
    bool lleva;
    string item;
};

struct NodoDobleDato
{
    Objeto *obj;
    int dato;
    NodoDobleDato *sig;
    NodoDobleDato() : obj(NULL), dato(0), sig(NULL) {}
};

class HASH
{
public:
    NodoDobleDato **vector;
    int buckets;
    int cantElem;

    HASH(int tamanio)
    {
        this->buckets = tamanio;
        this->cantElem = 0;
        this->vector = new NodoDobleDato *[this->buckets]();
    }

    bool agregarAux(NodoDobleDato *&h, int pos, Objeto *obj)
    {
        NodoDobleDato *act = h;
        while (act != NULL)
        {
            act = act->sig;
        }

        NodoDobleDato *nuevoNodo = new NodoDobleDato;
        nuevoNodo->dato = pos;
        nuevoNodo->obj = new Objeto(*obj); // Hacer una copia del objeto
        nuevoNodo->sig = h;
        h = nuevoNodo;
        return true;
    }

    void agregar(Objeto *obj, int posH)
    {
        int d = obj->id;
        unsigned int indx = (d < 0 ? -d : d) % buckets;
        bool inserted = agregarAux(vector[indx], posH, obj);
        if (inserted)
        {
            cantElem++;
        }
    }

    bool estaDefinida(int d)
    {
        if (esVacia())
            return false;
        unsigned int indx = (d < 0 ? -d : d) % buckets;
        NodoDobleDato *aux = vector[indx];
        while (aux)
        {
            if (aux->obj->id == d)
                return true;
            aux = aux->sig;
        }
        return false;
    }

    NodoDobleDato *recuperar(int d)
    {
        assert(estaDefinida(d));
        unsigned int indx = (d < 0 ? -d : d) % buckets;
        NodoDobleDato *aux = vector[indx];
        while (aux)
        {
            if (aux->obj->id == d)
                return aux;
            aux = aux->sig;
        }
        return NULL;
    }

    void borrarAux(NodoDobleDato *&l, int d)
    {
        if (l->obj->id == d)
        {
            NodoDobleDato *del = l;
            l = l->sig;
            delete del;
            del = NULL;
            return;
        }
        else
        {
            borrarAux(l->sig, d);
        }
    }

    void borrar(int d)
    {
        if (estaDefinida(d))
        {
            unsigned int indx = (d < 0 ? -d : d) % buckets;
            borrarAux(vector[indx], d);
            cantElem--;
        }
    }

    bool esVacia()
    {
        return cantElem == 0;
    }

    unsigned int cantidadElementos()
    {
        return cantElem;
    }
 
};

class MinHeap
{
public:
    HASH &hash;
    Objeto *vec;
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
    if (padre.prioridad == hijo.prioridad)
    {
        // Si tienen la misma prioridad, compara por 'lleva'
        if (padre.lleva == hijo.lleva)
        {
            return padre.id > hijo.id; // Ordenar por ID en caso de igualdad
        }
        return !padre.lleva && hijo.lleva; // Si 'padre' lleva, tiene mayor prioridad
    }
    return padre.prioridad > hijo.prioridad; // Prioridad por número
}


   void intercambiar(int pos1, int pos2)
{
    Objeto aux = vec[pos1];
    vec[pos1] = vec[pos2];
    vec[pos2] = aux;

    // Actualizar las posiciones en el hash
    
        hash.recuperar(vec[pos1].id)->dato = pos1;
    
    

        hash.recuperar(vec[pos2].id)->dato = pos2;
    
}

    void flotar(int pos)
    {
        if (pos == 1)
            return;
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


    MinHeap(int tam, HASH &hash) : hash(hash)
    {
        vec = new Objeto[tam + 1];
        primeroLibre = 1;
        capacidad = tam;
    }

    ~MinHeap()
    {
        delete[] vec;
    }

    void insertar(int id, int prioridad, bool lleva, string item)
    {
        if (!estaLleno())
        {
            vec[primeroLibre] = {id, prioridad, lleva, item};
            hash.agregar(&vec[primeroLibre], primeroLibre);
            flotar(primeroLibre);
            primeroLibre++;
        }
    }

    Objeto extraerMin()
    {
        if (estaVacio())
        {
            return {-1, -1, false, ""};
        }
        Objeto min = vec[1];
        intercambiar(1, primeroLibre-1);
        primeroLibre--;
        hundir(1);
        return min;
    }
};

class ColaPrioridadExt
{
private:
    MinHeap heap;
    HASH hash;

public:
    ColaPrioridadExt(int tamHeap, int tamHash) : hash(tamHash), heap(tamHeap, hash) {}

    void insertarCP(int id, int prioridad, bool lleva, string item)
    {
        heap.insertar(id, prioridad, lleva, item);
    }

    Objeto desencolar()
    {
        if (!heap.estaVacio())
        {
            Objeto min = heap.extraerMin();
            hash.borrar(min.id);
            return min;
        }
        return {-1, -1, false, ""};
    }

void eliminar(int id)
{
    if (!heap.estaVacio() && hash.estaDefinida(id))
    {
        int posenHeap = hash.recuperar(id)->dato;

        if (posenHeap == heap.primeroLibre - 1) {
            heap.primeroLibre--;
            hash.borrar(id);
            return;
        }
           
        // Intercambiar el elemento a eliminar con el último elemento del heap
        heap.intercambiar(posenHeap, heap.primeroLibre - 1);
        
        // Decrementar el tamaño del heap
        heap.primeroLibre--;
        
        // Borrar el elemento del hash
        
        // Si el elemento intercambiado no era el último, reajustar su posición
        {
            // Intentar hundir primero
            heap.hundir(posenHeap);
            
            // Si no se hundió (es decir, si sigue en la misma posición), intentar flotar
            heap.flotar(posenHeap);
        }

        hash.borrar(id);
    }
}

    void cambiarParaLlevar(int id)
    {
        if (hash.estaDefinida(id))
        {
            NodoDobleDato *nodo = hash.recuperar(id);
            heap.vec[nodo->dato].lleva = true;
            heap.flotar(nodo->dato);
        }
    }

    void mostrarPedidosPendientes()
    {
        while (!heap.estaVacio())
        {
            Objeto obj = desencolar();
            cout << " " << obj.id << " " << obj.prioridad << " " << (obj.lleva ? "true" : "false") << " " << obj.item << endl;
        }
    }
};
#include <fstream>
int main()
{
    // fstream file("");
    // if (!file.is_open()) {
    //     cerr << "Error" << endl;
    //     return -1;
    // }
    // int N = 0;
    // file >> N;
    // ColaPrioridadExt cola(N, N);

    // int O = 0;
    // file >> O;

    // for (int i = 0; i < O; i++)
    // {
    //     string f = "";
    //     file >> f;

    //     if (f == "I")
    //     {

    //         int id = 0;
    //         file >> id;
    //         int prioridad = 0;
    //         file >> prioridad;
    //         string paraLlevarStr;
    //         file >> paraLlevarStr;

    //         bool paraLlevar = false;
    //         if (paraLlevarStr == "true")
    //         {
    //             paraLlevar = true;
    //         }

    //         string item = "";
    //         file >> item;

    //         cola.insertarCP(id, prioridad, paraLlevar, item);
    //     }
    //     else if (f == "E")
    //     {
    //         int id = 0;
    //         file >> id;
    //         cola.eliminar(id);
    //     }
    //     else
    //     {
    //         int id = 0;
    //         file >> id;
    //         cola.cambiarParaLlevar(id);
    //     }
    // }

    // cola.mostrarPedidosPendientes();
    int N = 0;
    cin >> N;
    ColaPrioridadExt cola(N, N);

    int O = 0;
    cin >> O;

    for (int i = 0; i < O; i++)
    {
        string f = "";
        cin >> f;

        if (f == "I")
        {
            int id = 0;
            cin >> id;
            int prioridad = 0;
            cin >> prioridad;
            string paraLlevarStr;
            cin >> paraLlevarStr;

            bool paraLlevar = false;
            if (paraLlevarStr == "true")
            {
                paraLlevar = true;
            }

            string item = "";
            cin >> item;

            cola.insertarCP(id, prioridad, paraLlevar, item);
        }
        else if (f == "E")
        {
            int id = 0;
            cin >> id;
            cola.eliminar(id);
        }
        else
        {
            int id = 0;
            cin >> id;
            cola.cambiarParaLlevar(id);
        }
    }

    cola.mostrarPedidosPendientes();

    return 0;
}