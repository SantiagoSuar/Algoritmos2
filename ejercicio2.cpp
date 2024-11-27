#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct Libro
{
    int id;
    string titulo;
    bool disp;
    bool borrado;

    Libro(int id, string titulo) : id(id), titulo(titulo), disp(true), borrado(false) {}
};

class HASH
{
private:
    Libro **vector;
    int buckets;
    int cantElem;

    int habilitados;
    int deshabilitados;

    bool esPrimo(int num)
    {
        if (num <= 1)
            return false;
        if (num <= 3)
            return true;
        if (num % 2 == 0 || num % 3 == 0)
            return false;
        for (int i = 5; i * i <= num; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
        }
        return true;
    }
    // Función para encontrar el siguiente primo mayor o igual a un número dado
    int sigPrimo(int num)
    {
        while (!esPrimo(num))
        {
            num++;
        }
        return num;
    }

    int fnHash2(int key)
    {
        int h = 0;
        while (key != 0)
        {
            h = (h * 31 + key % 10);
            key /= 10;
        }
        return h;
    }
    int fnHash1(int key)
    {
        int h = 0;
        int i = 0;
        while (key != 0 && i != 3)
        {
            h = (h * 31 + key % 10);
            key /= 10;
            i++;
        }
        return h;
    }
    void insertarAux(int id, string titulo, Libro **vector)
    {

        int i = 0;
        bool insertado = false;
        while (!insertado)
        {
            int pos = abs(fnHash1(id) + i * fnHash2(id)) % buckets;

            if (!vector[pos] || vector[pos]->borrado)
            {
                vector[pos] = new Libro(id, titulo);
                cantElem++;
                habilitados++;
                insertado = true;
            }
            else
            {
                if (vector[pos]->id == id)
                {
                    vector[pos]->titulo = titulo;
                    if (!vector[pos]->disp){
                        habilitados++, deshabilitados--, vector[pos]->disp = true;
                    }
                        insertado = true;
                }
                        

            }
            i++;
        }
    }
void rehash()
{
    int nuevoBuckets = sigPrimo(buckets * 2);
    Libro **nuevoVector = new Libro *[nuevoBuckets];
    for (int i = 0; i < nuevoBuckets; i++)
    {
        nuevoVector[i] = NULL;
    }
    int cantNue= this->cantElem;
    this->habilitados=0;
    this->deshabilitados=0;

    int viejosBuckets = buckets;
    buckets = nuevoBuckets; // Actualiza el tamaño de buckets antes de reinsertar

    for (int i = 0; i < viejosBuckets; i++)
    {
        if (vector[i] != NULL && !vector[i]->borrado)
        {
            int id = vector[i]->id;
            string tit = vector[i]->titulo;
            insertarAux(id, tit, nuevoVector);
            if(!vector[i]->disp){
                toggleDisponibilidad(id,nuevoVector);
            }

        }
    }

    delete[] vector;
    vector = nuevoVector;
    cantElem=cantNue;

}


    int obtenerPos(int id,Libro **vector)
    {
        int i = 0;
        bool insertado = false;
        while (!insertado)
        {
                       int pos = abs(fnHash1(id) + i * fnHash2(id)) % buckets;


            if (!vector[pos])
            {
                return -1;
            }
            else
            {
                if (vector[pos]->id == id)
                {
                    if (!vector[pos]->borrado)
                    {
                        return pos;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }

            i++;
        }
    }

    void toggleDisponibilidad(int id,Libro **vector)
    {
        int pos = obtenerPos(id,vector);
        if (pos != -1)
        {
            vector[pos]->disp = !vector[pos]->disp;
            if (!vector[pos]->disp)
            {
                habilitados--;
                deshabilitados++;
            }
            else
            {
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
    HASH(int tamanio)
    {
        this->buckets = tamanio;
        this->cantElem = 0;
        this->deshabilitados=0;
        this->habilitados=0;
        this->vector = new Libro *[this->buckets]();
    }

    void ADD(int id, string titulo)
    {
        if (((float)(cantElem + 1) / buckets) > 0.7)
        {
            
            this->rehash();
        }

        insertarAux(id, titulo, this->vector);
        
    }
    void FIND(int id)
    {
        int pos = obtenerPos(id,vector);
        if (pos == -1)
        {
            cout << "libro_no_encontrado" << endl;
        }
        else
        {
            cout << vector[pos]->titulo << " " << (vector[pos]->disp ? "H" : "D") << endl;
        }
    }
    void TOGGLE(int id)
    {

        toggleDisponibilidad(id,vector);
    }
    void COUNT()
    {
        cout << cantElem << " " << habilitados << " " << deshabilitados << endl;
    }
    void ejecutarOperacion(const string &operacion, const int &param1, const string &param2 = "")
    {
        if (operacion == "ADD")
        {
            int id = param1;
            string titulo = param2;
            ADD(id, titulo);
        }
        else if (operacion == "FIND" || operacion == "TOGGLE")
        {
            int id = param1;
            ejecutarOperacionAux(operacion, id);
        }
        else if (operacion == "COUNT")
        {
            COUNT();
        }
    }

    void ejecutarOperacionAux(const string &operacion, int id)
    {
        if (operacion == "FIND")
        {
            FIND(id);
        }
        else if (operacion == "TOGGLE")
        {
            TOGGLE(id);
        }
    }
};



int main() {
    int numOperaciones;
    cin >> numOperaciones;
    HASH *h = new HASH(7);
    for (int i = 0; i < numOperaciones; i++)
    {
        string operacion, param2;
        int  param1;
        cin >> operacion;
        if (operacion == "ADD")
        {
            cin >> param1 >> param2;
            h->ejecutarOperacion(operacion, param1, param2);
        }
        else if (operacion == "FIND" || operacion == "TOGGLE")
        {
            cin >> param1;
            h->ejecutarOperacion(operacion, param1);
        }
        else if (operacion == "COUNT")
        {
            h->ejecutarOperacion(operacion,0,"");
        }
    }

    return 0;
}


