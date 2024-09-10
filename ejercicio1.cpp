#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;


class AVL
{
public:
    struct NodoAVL
    {
        int id;
        string titulo;
        bool disp;
        NodoAVL *izq, *der;
        int altura;
        NodoAVL(int d, string titulo) : id(d), titulo(titulo), disp(true), izq(NULL), der(NULL), altura(1) {}
    };

    NodoAVL *raiz;

    int total;
    int habilitados;
    int deshabilitados;
    int max(int a, int b)
    {
        return a > b ? a : b;
    }

    // Funcion para acceder a la altura del nodo, si el nodo es nulo, la altura es 0
    int altura(NodoAVL *n)
    {
        if (!n)
            return 0;
        return n->altura;
    }

    // Funcion para calcular el balance del nodo
    int balance(NodoAVL *n)
    {
        if (!n)
            return 0;
        return altura(n->izq) - altura(n->der);
    }

    NodoAVL *rotacionHoraria(NodoAVL *A)
    {
        NodoAVL *B = A->izq;
        NodoAVL *T2 = B->der;
        B->der = A;
        A->izq = T2;
        A->altura = 1 + max(altura(A->izq), altura(A->der));
        B->altura = 1 + max(altura(B->izq), altura(B->der));
        return B;
    }

    NodoAVL *rotacionAntiHoraria(NodoAVL *B)
    {
        NodoAVL *A = B->der;
        NodoAVL *T2 = A->izq;
        A->izq = B;
        B->der = T2;
        B->altura = 1 + max(altura(B->izq), altura(B->der));
        A->altura = 1 + max(altura(A->izq), altura(A->der));
        return A;
    }

    NodoAVL *insertarAux(NodoAVL *nodo, int dato, string tit)
    {
        if (!nodo)
        {
            total++;
            habilitados++;
            return new NodoAVL(dato, tit);
        }

        if (dato < nodo->id)
            nodo->izq = insertarAux(nodo->izq, dato, tit);
        else if (dato > nodo->id)
            nodo->der = insertarAux(nodo->der, dato, tit);
        else if (dato == nodo->id)
        {
            nodo->titulo = tit;
            return nodo;
        }

        // A la vuelta de la recursión, actualizo la altura del nodo en el que estoy
        nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));

        // Verifico si el nodo está desbalanceado
        int balanceo = balance(nodo);

        bool desbalanceoIzq = balanceo > 1;
        bool desbalanceoDer = balanceo < -1;

        // Caso izquierda izquierda
        if (desbalanceoIzq && dato < nodo->izq->id)
        {
            return rotacionHoraria(nodo);
        }

        // Caso derecha derecha
        if (desbalanceoDer && dato > nodo->der->id)
        {
            return rotacionAntiHoraria(nodo);
        }

        // Caso izquierda derecha
        if (desbalanceoIzq && dato > nodo->izq->id)
        {
            nodo->izq = rotacionAntiHoraria(nodo->izq);
            return rotacionHoraria(nodo);
        }

        // Caso derecha izquierda
        if (desbalanceoDer && dato < nodo->der->id)
        {
            nodo->der = rotacionHoraria(nodo->der);
            return rotacionAntiHoraria(nodo);
        }

        return nodo;
    }
    NodoAVL * buscarLibroAux(NodoAVL *n, int id)
    {
        if (n == NULL)
            return NULL;
        if (id < n->id)
           return buscarLibroAux(n->izq, id);
        else if (id > n->id)
            return buscarLibroAux(n->der, id);
        else if (id == n->id)
        {
            return n; // Encontrado
        }
    }
    string FIND(NodoAVL *n, int e)
    {
        if (buscarLibroAux(n, e) && n->disp == 1)
        {
            return n->titulo;
        }
        else
        {
            return "libro_no_encontrado";
        }
    }

    void toggleDisponibilidad(NodoAVL *n, int id)
    {
        if (buscarLibroAux(n, id))
        {
            if (n->disp == 1)
            {
                n->disp = 0;
                habilitados--;
                deshabilitados++;
            }
            else
            {
                n->disp = 1;
                habilitados++;
                deshabilitados--;
            }
        }
        else
        {
            cout << "libro_no_encontrado" << endl;
        }
    }
  
public:
    AVL() : raiz(NULL), total(0), habilitados(0),deshabilitados(0) {}

    void insertar(int dato, string titulo)
    {
        raiz = insertarAux(raiz, dato, titulo);
    }

    void FIND(int id)
    {
        NodoAVL *n = buscarLibroAux(raiz, id);
        if (n == NULL)
        {
            cout << "libro_no_encontrado" << endl;
        }
        else
        {
            cout << n->titulo << " " << (n->disp ? "H" : "D") << endl;
        }
    }

    void TOGGLE(int id)
    {
        NodoAVL *n = buscarLibroAux(raiz, id);
        if (n == NULL)
        {
            cout << "libro_no_encontrado" << endl;
        }
        else
        {
            toggleDisponibilidad(n,id);
        }
    }

    void COUNT()
    {
        cout << total << " " << habilitados << " " << deshabilitados << endl;
    }
};
int main()
{
    AVL arbol;
    arbol.insertar(15, "Don_Quijote");
    arbol.insertar(23, "La_Casa_de_los_Espiritus");
    arbol.FIND(15);  // Output: Don_Quijote H
    arbol.FIND(88);  // Output: libro_no_encontrado
    arbol.TOGGLE(15);
    arbol.FIND(15);  // Output: Don_Quijote D
    arbol.insertar(25, "Cien_Años_de_Soledad");
    arbol.COUNT();   // Output: 3 2 1
    arbol.TOGGLE(99);  // Output: libro_no_encontrado
    arbol.TOGGLE(23);
    arbol.COUNT();   // Output: 3 1 2
    return 0;
}
