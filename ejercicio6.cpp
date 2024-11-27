#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Ciudad {
    int id;
    string nombre;
};

struct Arista {
    Ciudad origen;
    Ciudad destino;
    int peso;
    Arista *sig;
};

class MinHeap {
private:
    Ciudad *vec;
    int *costos;
    int capacidad;
    int primeroLibre;

    int padre(int pos) { return pos / 2; }
    int hijoIzq(int pos) { return 2 * pos; }
    int hijoDer(int pos) { return 2 * pos + 1; }

        
    bool comparar(const Ciudad &padre, const Ciudad &hijo, int *costos)
    {
        if (costos[padre.id] == costos[hijo.id])
        {
            return padre.id > hijo.id; // Menor id tiene prioridad si los costos son iguales
        }
        return costos[padre.id] > costos[hijo.id]; // Menor costo tiene prioridad
    }

    void intercambiar(int pos1, int pos2) {
        Ciudad aux = vec[pos1];
        vec[pos1] = vec[pos2];
        vec[pos2] = aux;
    }

    void flotar(int pos) {
        if (pos == 1) return;
        int posPadre = padre(pos);
        if (comparar(vec[posPadre], vec[pos], costos)) {
            intercambiar(posPadre, pos);
            flotar(posPadre);
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
    MinHeap(int tam) {
        vec = new Ciudad[tam + 1]();
        capacidad = tam;
        primeroLibre = 1;
    }

    void setCostos(int *costos) { this->costos = costos; }

    ~MinHeap() { delete[] vec; }

    bool estaVacio() { return primeroLibre == 1; }

    void insertar(Ciudad obj) {
        if (primeroLibre > capacidad) return;
        vec[primeroLibre] = obj;
        flotar(primeroLibre++);
    }

    Ciudad extraerMin() {
        Ciudad min = vec[1];
        vec[1] = vec[--primeroLibre];
        hundir(1, costos);
        return min;
    }
};

class GrafoLista {
private:
    Arista **vertices;
    int cantidadV;
    bool dirigido;

public:
    GrafoLista(int cantV, bool esDirigido) {
        vertices = new Arista *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
    }

    ~GrafoLista() {
        for (int i = 0; i <= cantidadV; ++i) {
            Arista *actual = vertices[i];
            while (actual) {
                Arista *temp = actual;
                actual = actual->sig;
                delete temp;
            }
        }
        delete[] vertices;
    }
 GrafoLista *copiarGrafo()
    {
        GrafoLista *nuevoGrafo = new GrafoLista(cantidadV, dirigido);
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
    void agregarArista(Ciudad origen, Ciudad destino, int peso) {
        Arista *nuevaArista = new Arista{origen, destino, peso, vertices[origen.id]};
        vertices[origen.id] = nuevaArista;

        if (!dirigido) {
            Arista *inversa = new Arista{destino, origen, peso, vertices[destino.id]};
            vertices[destino.id] = inversa;
        }
    }

    Arista *adyacentes(int vertice) { return vertices[vertice]; }

    void multiplicoArista(int origen, int destino) {
        Arista *aux = vertices[origen];
        while (aux) {
            if (aux->destino.id == destino) {
                aux->peso *= 2;
                break;
            }
            aux = aux->sig;
        }
        aux = vertices[destino];
        while (aux) {
            if (aux->destino.id == origen) {
                aux->peso *= 2;
                break;
            }
            aux = aux->sig;
        }
    }
};

// Inicializar arrays para Dijkstra
void initializeArrays(int cantCiudades, int *costos, int *vengo, bool *visitados) {
    for (int i = 0; i <= cantCiudades; ++i) {
        costos[i] = INT32_MAX;
        vengo[i] = -1;
        visitados[i] = false;
    }
}

// Dijkstra
void dijkstra(Ciudad &origen, GrafoLista *grafo, int cantCiudades, int *costos, int *vengo, bool *visitados) {
    MinHeap heap(cantCiudades);
    heap.setCostos(costos);
    costos[origen.id] = 0;
    heap.insertar(origen);

    while (!heap.estaVacio()) {
        Ciudad u = heap.extraerMin();
        if (visitados[u.id]) continue;
        visitados[u.id] = true;

        for (Arista *adj = grafo->adyacentes(u.id); adj; adj = adj->sig) {
            int v = adj->destino.id;
            if (costos[v] > costos[u.id] + adj->peso) {
                costos[v] = costos[u.id] + adj->peso;
                vengo[v] = u.id;
                heap.insertar(adj->destino);
            }
        }
    }
}

int obtenerCamino(int origen, int destino, int *vengo, int *camino)
{
    int indice = 0;
    int actual = destino;
    while (actual != -1)
    {
        camino[indice++] = actual;
        if (actual == origen)
            break;
        actual = vengo[actual];
    }
    // Invertir el camino
    for (int i = 0; i < indice / 2; ++i)
    {
        int temp = camino[i];
        camino[i] = camino[indice - i - 1];
        camino[indice - i - 1] = temp;
    }
    return indice; // Retorna la longitud del camino
}
 

// Multiplicar pesos en el camino
void duplicarPesosCamino(GrafoLista *grafo, int *camino, int longitud) {
    for (int i = 0; i < longitud - 1; ++i) {
        grafo->multiplicoArista(camino[i], camino[i + 1]);
    }
}

int main() {
    int cantCiudades;
    cin >> cantCiudades;

    string ciudades[301];
    for (int i = 0; i < cantCiudades; ++i) {
        int id;
        string nombre;
        cin >> id >> nombre;
        ciudades[id] = nombre;
    }

    int morfeo, entidad, equipo, extraccion;
    cin >> morfeo >> entidad >> equipo >> extraccion;

    Ciudad ciudadMorfeo = {morfeo, ciudades[morfeo]};
    Ciudad ciudadEntidad = {entidad, ciudades[entidad]};
    Ciudad ciudadEquipo = {equipo, ciudades[equipo]};
    Ciudad ciudadExtraccion = {extraccion, ciudades[extraccion]};

    int numAristas;
    cin >> numAristas;

    GrafoLista grafo(cantCiudades, false);
    for (int i = 0; i < numAristas; ++i) {
        int origenId, destinoId, peso;
        cin >> origenId >> destinoId >> peso;
        Ciudad origen = {origenId, ciudades[origenId]};
        Ciudad destino = {destinoId, ciudades[destinoId]};
        grafo.agregarArista(origen, destino, peso);
    }

    // Variables para almacenar costos y caminos
    int costos[301], vengo[301], camino[301];
    bool visitados[301];
    int totalCostOption1 = 0, totalCostOption2 = 0;

    string path1_step1, path1_step2, path1_step3;
    string path2_step1, path2_step2, path2_step3;

    // Opción 1: Desactivar la Entidad, Buscar Equipo, Ir a Punto de Extracción
    GrafoLista *grafo1 = grafo.copiarGrafo();

    // Paso 1: Morfeo a Entidad
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadMorfeo, grafo1, cantCiudades, costos, vengo, visitados);
    totalCostOption1 += costos[ciudadEntidad.id];
    int longitud = obtenerCamino(ciudadMorfeo.id, ciudadEntidad.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path1_step1 += " -> ";
        path1_step1 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo1, camino, longitud);

    // Paso 2: Entidad a Equipo
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadEntidad, grafo1, cantCiudades, costos, vengo, visitados);
    totalCostOption1 += costos[ciudadEquipo.id];
    longitud = obtenerCamino(ciudadEntidad.id, ciudadEquipo.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path1_step2 += " -> ";
        path1_step2 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo1, camino, longitud);

    // Paso 3: Equipo a Punto de Extracción
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadEquipo, grafo1, cantCiudades, costos, vengo, visitados);
    totalCostOption1 += costos[ciudadExtraccion.id];
    longitud = obtenerCamino(ciudadEquipo.id, ciudadExtraccion.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path1_step3 += " -> ";
        path1_step3 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo1, camino, longitud);

    // Opción 2: Buscar Equipo, Desactivar la Entidad, Ir a Punto de Extracción
    GrafoLista *grafo2 = grafo.copiarGrafo();

    // Paso 1: Morfeo a Equipo
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadMorfeo, grafo2, cantCiudades, costos, vengo, visitados);
    totalCostOption2 += costos[ciudadEquipo.id];
    longitud = obtenerCamino(ciudadMorfeo.id, ciudadEquipo.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path2_step1 += " -> ";
        path2_step1 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo2, camino, longitud);

    // Paso 2: Equipo a Entidad
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadEquipo, grafo2, cantCiudades, costos, vengo, visitados);
    totalCostOption2 += costos[ciudadEntidad.id];
    longitud = obtenerCamino(ciudadEquipo.id, ciudadEntidad.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path2_step2 += " -> ";
        path2_step2 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo2, camino, longitud);

    // Paso 3: Entidad a Punto de Extracción
    initializeArrays(cantCiudades, costos, vengo, visitados);
    dijkstra(ciudadEntidad, grafo2, cantCiudades, costos, vengo, visitados);
    totalCostOption2 += costos[ciudadExtraccion.id];
    longitud = obtenerCamino(ciudadEntidad.id, ciudadExtraccion.id, vengo, camino);
    for (int i = 0; i < longitud; ++i) {
        if (i > 0) path2_step3 += " -> ";
        path2_step3 += ciudades[camino[i]];
    }
    duplicarPesosCamino(grafo2, camino, longitud);

    // Determinar la mejor opción
    if (totalCostOption1 <= totalCostOption2) {
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << totalCostOption1 << endl;
        cout << "La otra opcion tiene un costo de " << totalCostOption2 << endl;
        cout << "Paso 1: " << path1_step1 << " -> Desactivar la Entidad" << endl;
        cout << "Paso 2: " << path1_step2 << " -> Buscar equipo" << endl;
        cout << "Paso 3: " << path1_step3 << " -> Ir a Punto de extraccion" << endl;
    } else {
        cout << "BE11, la mejor ruta es Buscar equipo, Desactivar la Entidad y punto de extraccion con un costo de " << totalCostOption2 << endl;
        cout << "La otra opcion tiene un costo de " << totalCostOption1 << endl;
        cout << "Paso 1: " << path2_step1 << " -> Buscar equipo" << endl;
        cout << "Paso 2: " << path2_step2 << " -> Desactivar la Entidad" << endl;
        cout << "Paso 3: " << path2_step3 << " -> Ir a Punto de extraccion" << endl;
    }

    // Liberar memoria
    delete grafo1;
    delete grafo2;

    return 0;
}
