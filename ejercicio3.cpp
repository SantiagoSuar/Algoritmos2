#include <iostream>
using namespace std;

struct Objeto {
    int id;
    int precio;
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

int main() {
    int N, K;
    cin >> N;

    // Crear un heap con capacidad N (máximo número de objetos)
    MinHeap heap(N);

    // Array para verificar si un id ya fue utilizado
    bool idUsado[100001] = {false}; // Dado que 1 <= id <= N, hasta 100000

    for (int i = 0; i < N; ++i) {
        int id, precio;
        cin >> id >> precio;

        // Si el id no ha sido usado, lo insertamos en el heap
        if (!idUsado[id]) {
            idUsado[id] = true;
            heap.insertar({id, precio});
        } else {
            // Si el id ya ha sido utilizado, actualizar solo si el nuevo precio es menor
            heap.actualizar(id, precio);
        }
    }

    // Leer K (cantidad de objetos a seleccionar)
    cin >> K;

    // Extraer los K objetos más baratos del heap
    for (int i = 0; i < K; ++i) {
        Objeto obj = heap.extraerMin();
        cout << obj.id << endl;
    }

    return 0;
}
