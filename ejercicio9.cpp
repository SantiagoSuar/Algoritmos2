#include <iostream>
#include <string>
#include <algorithm>  // Para std::max
using namespace std;

struct Jugador {
    int ritmo, tiro, pase, regate, defensa, fisico;
    int sueldo;
    bool extranjero;
    int valoracion;

    // Constructor que recibe los valores para inicializar al jugador
    Jugador(int r, int t, int p, int re, int d, int f, string ff, int s, string e, string c) {
        ritmo = r; tiro = t; pase = p; regate = re; defensa = d; fisico = f;
        sueldo = s;
        extranjero = (e == "Si");

        // Calcular valoración base
        valoracion = (ritmo + tiro + pase + regate + defensa + fisico) / 6;

        // Ajustar valoración según forma física
        if (ff == "Lesionado") {
            valoracion = 0;
        } else if (ff == "Mala") {
            valoracion -= 5;
        } else if (ff == "Buena") {
            valoracion += 5;
        }

        // Ajustar valoración según confianza
        if (c == "Baja") {
            valoracion -= 5;
        } else if (c == "Alta") {
            valoracion += 5;
        }
    }
};

int main() {
    int J, P, E;
    cin >> J;

    // Usamos puntero para almacenar los jugadores
    Jugador* jugadores = new Jugador[J];
    
    for (int i = 0; i < J; i++) {
        int ritmo, tiro, pase, regate, defensa, fisico, sueldo;
        string formaFisica, extranjero, confianza;
        cin >> ritmo >> tiro >> pase >> regate >> defensa >> fisico >> formaFisica >> sueldo >> extranjero >> confianza;
        jugadores[i] = Jugador(ritmo, tiro, pase, regate, defensa, fisico, formaFisica, sueldo, extranjero, confianza);
    }

    cin >> P >> E;

    // Generamos la matriz de tabulación ahora con 3 dimensiones usando punteros
    int*** dp = new int**[J + 1];
    for (int i = 0; i <= J; i++) {
        dp[i] = new int*[P + 1];
        for (int p = 0; p <= P; p++) {
            dp[i][p] = new int[E + 1]();
        }
    }

    // La fila 0 no la recorremos, solamente la consultamos
    for (int i = 1; i <= J; i++) {
        int sueldo = jugadores[i - 1].sueldo;
        int valoracion = jugadores[i - 1].valoracion;
        bool esExtranjero = jugadores[i - 1].extranjero;

        for (int p = 0; p <= P; p++) {
            for (int e = 0; e <= E; e++) {
                // Si el jugador actual no entra en el presupuesto o en el cupo de extranjeros
                if (sueldo > p || (esExtranjero && e == 0)) {
                    dp[i][p][e] = dp[i - 1][p][e];
                } else {
                    int valorDeUsarElJugador = valoracion + dp[i - 1][p - sueldo][e - (esExtranjero ? 1 : 0)];
                    int valorDeNoUsarElJugador = dp[i - 1][p][e];
                    dp[i][p][e] = max(valorDeUsarElJugador, valorDeNoUsarElJugador);
                }
            }
        }
    }

    // Vector dinámico para almacenar los jugadores seleccionados
    int* jugadoresSeleccionados = new int[J];
    int numJugadoresSeleccionados = 0;
    int valoracionMaxima = dp[J][P][E];

    // Reconstrucción de la selección de jugadores
    for (int i = J, p = P, e = E; i > 0; i--) {
        if (dp[i][p][e] != dp[i - 1][p][e]) {
            jugadoresSeleccionados[numJugadoresSeleccionados++] = i - 1;
            p -= jugadores[i - 1].sueldo;
            if (jugadores[i - 1].extranjero) {
                e--;
            }
        }
    }
    
    // Calcular el promedio de valoración del once inicial
    double promedioValoracion = (numJugadoresSeleccionados > 0) ? static_cast<double>(valoracionMaxima) / numJugadoresSeleccionados : 0;

    // Imprimir el promedio de valoración y los índices de los jugadores seleccionados
    cout << "Promedio de valoración del once inicial: " << promedioValoracion << endl;
    cout << "Jugadores seleccionados (índices): ";
    for (int i = 0; i < numJugadoresSeleccionados; i++) {
        cout << jugadoresSeleccionados[i] << " ";
    }

    for (int i = 0; i <= J; i++) {
        for (int p = 0; p <= P; p++) {
            delete[] dp[i][p];
        }
        delete[] dp[i];
    }
    delete[] dp;
    delete[] jugadores;
    delete[] jugadoresSeleccionados;

    return 0;
}
