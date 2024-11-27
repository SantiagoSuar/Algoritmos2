#include <iostream>
#include <cstring>
using namespace std;

struct Jugador
{
    int ritmo, tiro, pase, regate, defensa, fisico;
    int sueldo;
    bool extranjero;
    int valoracion;

    // Constructor por defecto
    Jugador() : ritmo(0), tiro(0), pase(0), regate(0), defensa(0), fisico(0), sueldo(0), extranjero(false), valoracion(0) {}

    // Constructor que recibe los valores para inicializar al jugador
    Jugador(int r, int t, int p, int re, int d, int f, string ff, int s, string e, string c)
    {
        ritmo = r;
        tiro = t;
        pase = p;
        regate = re;
        defensa = d;
        fisico = f;
        sueldo = s;
        extranjero = (e == "Si");

        // Calcular valoración base
        valoracion = (ritmo + tiro + pase + regate + defensa + fisico) / 6;

        // Ajustar valoración según forma física
        if (ff == "Lesionado")
        {
            valoracion = 0;
        }
        else if (ff == "Mala")
        {
            valoracion -= 5;
        }
        else if (ff == "Buena")
        {
            valoracion += 5;
        }

        // Ajustar valoración según confianza
        if (c == "Baja")
        {
            valoracion -= 5;
        }
        else if (c == "Alta")
        {
            valoracion += 5;
        }
    }
};

int main()
{
    int J, P, E;
    cin >> J;

    // Usamos puntero para almacenar los jugadores
    Jugador *jugadores = new Jugador[J];

    for (int i = 0; i < J; i++)
    {
        int ritmo, tiro, pase, regate, defensa, fisico, sueldo;
        string formaFisica, extranjero, confianza;
        cin >> ritmo >> tiro >> pase >> regate >> defensa >> fisico >> formaFisica >> sueldo >> extranjero >> confianza;
        jugadores[i] = Jugador(ritmo, tiro, pase, regate, defensa, fisico, formaFisica, sueldo, extranjero, confianza);
    }

    cin >> P >> E;

    // Generamos la matriz 4D
    int ****dp = new int ***[J + 1];
    for (int i = 0; i <= J; i++)
    {
        dp[i] = new int **[P + 1];
        for (int p = 0; p <= P; p++)
        {
            dp[i][p] = new int *[E + 1];
            for (int e = 0; e <= E; e++)
            {
                dp[i][p][e] = new int[12](); // 0 inicializado
            }
        }
    }

    // Rellenamos la matriz
    for (int i = 1; i <= J; i++)
    {
        int sueldo = jugadores[i - 1].sueldo;
        int valoracion = jugadores[i - 1].valoracion;
        bool esExtranjero = jugadores[i - 1].extranjero;

        for (int p = 0; p <= P; p++)
        {
            for (int e = 0; e <= E; e++)
            {
                for (int n = 0; n <= 11; n++)
                {
                    dp[i][p][e][n] = dp[i - 1][p][e][n]; // No seleccionar al jugador

                    if (p >= sueldo && n > 0 && (!esExtranjero || e > 0))
                    {
                        int valorDeUsarElJugador = valoracion + dp[i - 1][p - sueldo][e - (esExtranjero ? 1 : 0)][n - 1];
                        dp[i][p][e][n] = max(dp[i][p][e][n], valorDeUsarElJugador);
                    }
                }
            }
        }
    }

    int valoracionMaxima = dp[J][P][E][11];

    cout << (valoracionMaxima) / 11 << endl;

    // Liberación de memoria correcta
    for (int i = 0; i <= J; i++)
    {
        for (int p = 0; p <= P; p++)
        {
            for (int e = 0; e <= E; e++)
            {
                delete[] dp[i][p][e];
            }
            delete[] dp[i][p];
        }
        delete[] dp[i];
    }
    delete[] dp;
    delete[] jugadores;

    return 0;
}