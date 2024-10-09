#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Ciudad{
    string nombre;
    int id;
};

struct Mision{
    string nombre;
    int id;
};

class MinHeap {
private:
    Objeto* vec; // Array de objetos
    int capacidad;
    int primeroLibre;

    bool estaLleno() {
        return primeroLibre > capacidad;
    }

    bool estaVacio() {
        return primeroLibre == 1;
    }

    int padre(int pos) {
        return pos / 2;
    }

    int hijoIzq(int pos) {
        return 2 * pos;
    }

    int hijoDer(int pos) {
        return 2 * pos + 1;
    }

    bool comparar(const Objeto& padre, const Objeto& hijo) {
        // Prioridad por precio, si los precios son iguales, por id mayor
        if (padre.precio == hijo.precio) {
            return padre.id < hijo.id;  // El id más grande tiene prioridad si los precios son iguales
        }
        return padre.precio > hijo.precio;  // Menor precio tiene prioridad
    }

    void intercambiar(int pos1, int pos2) {
        Objeto aux = vec[pos1];
        vec[pos1] = vec[pos2];
        vec[pos2] = aux;
    }

    void flotar(int pos) {
        if (pos == 1) return;  // Ya estamos en la raíz
        int posPadre = padre(pos);
        if (comparar(vec[posPadre], vec[pos])) {
            intercambiar(posPadre, pos);
            flotar(posPadre);
        }
    }

    void hundir(int pos) {
        int posIzq = hijoIzq(pos);
        int posDer = hijoDer(pos);
        int menor = pos;

        if (posIzq < primeroLibre && comparar(vec[menor], vec[posIzq])) {
            menor = posIzq;
        }
        if (posDer < primeroLibre && comparar(vec[menor], vec[posDer])) {
            menor = posDer;
        }
        if (menor != pos) {
            intercambiar(pos, menor);
            hundir(menor);
        }
    }

public:
    MinHeap(int tam) {
        vec = new Objeto[tam + 1];  // Posición 0 no se usa
        primeroLibre = 1;
        capacidad = tam;
    }

    ~MinHeap() {
        delete[] vec;
    }

    void insertar(Objeto obj) {
        if (!estaLleno()) {
            vec[primeroLibre] = obj;
            flotar(primeroLibre);
            primeroLibre++;
        }
    }

    Objeto extraerMin() {
        if (estaVacio()) {
            return { -1, -1 };  // Valor especial si está vacío
        }
        Objeto min = vec[1];
        vec[1] = vec[primeroLibre - 1];
        primeroLibre--;
        hundir(1);
        return min;
    }

    void actualizar(int id, int nuevoPrecio) {
        // Buscar el objeto con el mismo id y actualizar su precio si es menor
        for (int i = 1; i < primeroLibre; i++) {
            if (vec[i].id == id) {
                if (nuevoPrecio < vec[i].precio) {
                    vec[i].precio = nuevoPrecio;
                    hundir(i);  // Reajustar la posición
                    flotar(i);  // Reajustar la posición
                }
                break;
            }
        }
    }
};

class camino{
    private:
        int cantMisiones;
        int cantciudades;
        int *posiciones = new int[51]();

    public:
        
        void dijsktra(int origen){
            int* vengo = new int[cantciudades+1]();
            int* costos = new int [cantciudades+1]();
            bool* visitados = new bool [cantciudades+1]();
            MinHeap h(cantciudades+1);
            h.insertar(origen,0);
            while(!h.estaVacio()){
                int v = h.extraerMin();
                visitados[v] = true;
                NodoLista* w = grafoLA[v];
                while(w){
                    if(!visitados[w->dato] && costos[w] > costos [v] + dist(v,w)){
                        costos [w->dato] = costos[v] + dist(v,w);
                        vengo[w] = v;
                        h.insertar(w,costos[w])
                    }
                }
            }



        }

};


int main()
{
    // TODO
    return 0;
}