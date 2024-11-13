#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
class Camino{

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
}
    
 
int main()
{
    return 0;
}