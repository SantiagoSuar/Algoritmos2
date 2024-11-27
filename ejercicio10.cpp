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
bool canPlace(int row, int col, string **grid, const string &color) {
    int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for (int d = 0; d < 8; d++) {
        int nr = row + dr[d];
        int nc = col + dc[d];
        if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
            if (grid[nr][nc] == color) {
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

// Backtracking para maximizar el número de flores plantadas
void plantarFlores(int row, int col, string **grid, int floresPlantadas, int &maxFlores, int F, Flor flores[]) {
    // Debug: Print current state

    // Si llegamos al final de la cuadrícula
    if (row == N) {
        if (esMejorSolucion(floresPlantadas, maxFlores)) {
            maxFlores = floresPlantadas;
        }
        return;
    }

    // Calcular la siguiente posición
    int nextRow = col == N - 1 ? row + 1 : row;
    int nextCol = col == N - 1 ? 0 : col + 1;

    // Poda: Si el número de flores restantes no supera la mejor solución actual
    int floresRestantes = (N * N - (row * N + col));
    if (floresPlantadas + floresRestantes <= maxFlores) return;

    // Opción 1: No plantar en esta posición
    plantarFlores(nextRow, nextCol, grid, floresPlantadas, maxFlores, F, flores);

    // Opción 2: Plantar flores de cualquier color permitido
    for (int i = 0; i < F; ++i) {
        string color = flores[i].color;
       
        // Poda: Si la flor tiene restricción de fila y esta no coincide
        if (flores[i].restrictedRow != -1 && flores[i].restrictedRow != row) continue;

        // Verificar si es seguro plantar la flor
        if (canPlace(row, col, grid, color)) {
            grid[row][col] = color; // Plantar flor
            plantarFlores(nextRow, nextCol, grid, floresPlantadas + 1, maxFlores, F, flores);
            grid[row][col] = "";   // Deshacer acción
        }
    }
}

int main() {
    int F;
    cin >> N >> F;

    // Crear jardín como matriz dinámica
    string **grid = new string *[N];
    for (int i = 0; i < N; ++i) {
        grid[i] = new string[N];
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



    // Inicializar grid con strings vacíos
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            grid[i][j] = "";
        }
    }


    int maxFlores = 0;
    plantarFlores(0, 0, grid, 0, maxFlores, F, flores);

    cout << maxFlores << endl;

    // Liberar memoria
    for (int i = 0; i < N; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    delete[] flores;

    return 0;
}
