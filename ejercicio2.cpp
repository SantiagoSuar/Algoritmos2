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
        Libro** vector;
        int buckets;
        int cantElem;

        void rehash(){
            buckets = sigPrimo(buckets);


        }
        
        int fnHash1(){
            
        }

        void insertarAux(int id, string titulo){
            
        }
        

    public:

        HASH(int tamanio){
            this->buckets = tamanio;
            this->cantElem = 0;
            this->vector = new Libro* [this->buckets] ();
        }

        void insertar (int id, string titulo){
            if(((float)(cantElem + 1)/buckets) > 0.7){
                this->rehash();
            } else this->insertarAux(id,titulo);
        }

};

int main()
{
    // TODO
    return 0;
}