#include <iostream>
#include <string>
using namespace std;

int N; // Tamaño de la cuadrícula
struct  Flor
{
    string color;
    int restrictedRow;
};

// Verifica si se puede plantar una flor en una posición específica
bool esPosibleColocar(int row, int col, string **mat, const string &color) {
    int movimientosFila[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int movimientosCol[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for (int d = 0; d < 8; d++) {
        int nuevafila = row + movimientosFila[d];
        int nuevacola = col + movimientosCol[d];
        if (nuevafila >= 0 && nuevafila < N && nuevacola >= 0 && nuevacola < N) {
            if (mat[nuevafila][nuevacola] == color) {
                return false;
            }
        }
    }
    return true;
}

// Verifica si la solución actual es mejor que la máxima encontrada
bool esMejorSolucion(int floresPlantadas, int &maxFlores) {
    return floresPlantadas > maxFlores;
}
bool esSolucion() {
    return true;
}
// Backtracking para maximizar el número de flores plantadas
void plantarFlores(int row, int col, string **mat, int floresPlantadas, int &maxFlores, int F, Flor flores[]) {

    // Si llegamos al final de la cuadrícula
    if (row == N) {
        if (esSolucion() && esMejorSolucion(floresPlantadas, maxFlores)) {
            maxFlores = floresPlantadas;
        }
        return;
    }

    // Calcular la siguiente posición
    int nextRow = (col == N - 1 )? row + 1 : row;
    int nextCol = (col == N - 1 )? 0 : col + 1;

    // Poda: Si el número de flores restantes no supera la mejor solución actual
    int floresRestantes = (N * N - (row * N + col));
    if (floresPlantadas + floresRestantes <= maxFlores) return;

    // Opción 1: No plantar en esta posición
    plantarFlores(nextRow, nextCol, mat, floresPlantadas, maxFlores, F, flores);

    // Opción 2: Plantar flores de cualquier color permitido
    for (int i = 0; i < F; ++i) {
        string color = flores[i].color;
       
        // puedo poner objetos en cualquier fila o en la fila restringida
        if (flores[i].restrictedRow == -1 || flores[i].restrictedRow == row){

        // Verificar si es seguro plantar la flor
        if (esPosibleColocar(row, col, mat, color)) {
            mat[row][col] = color; // Plantar flor
            plantarFlores(nextRow, nextCol, mat, floresPlantadas + 1, maxFlores, F, flores);
            mat[row][col] = "";   // Deshacer acción
        }
        }
    }
}

int main() {
    int F;
    cin >> N >> F;

    // Crear jardín como matriz dinámica
    string **mat = new string *[N];
    for (int i = 0; i < N; ++i) {
        mat[i] = new string[N];
    }

    // Crear array de flores
    Flor *flores = new Flor[F];

    // leer restricciones y flores
    for(int i = 0; i < F; i++){
        string color;
        int restrictedRow, restrictedCol;
        cin >> color >> restrictedRow;
        flores[i].color = color;
        flores[i].restrictedRow = restrictedRow;
    }



    // Inicializar mat con strings vacíos
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat[i][j] = "";
        }
    }


    int maxFlores = 0;
    plantarFlores(0, 0, mat, 0, maxFlores, F, flores);

    cout << maxFlores << endl;

    // Liberar memoria
    for (int i = 0; i < N; ++i) {
        delete[] mat[i];
    }
    delete[] mat;

    delete[] flores;

    return 0;
}
