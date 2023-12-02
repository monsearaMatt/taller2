#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <limits>
#include <cmath>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;
const char JUGADOR = 'X';
const char MAQUINA = 'O';
const char VACIO = '-';
const string ARCHIVO_CSV = "conecta4_partidas.csv";
int contadorGanadasJugador=0;
int contadorGanadasMaquina=0;
const int INF = numeric_limits<int>::max(); // Definir INF como el valor máximo para la poda alfa-beta

vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS, VACIO));

bool esMovimientoValido(int columna) {
    return tablero[0][columna] == VACIO; // Verificar si la columna no está llena
}

void realizarMovimiento(int columna, char jugador) {
    for (int i = FILAS - 1; i >= 0; --i) {
        if (tablero[i][columna] == VACIO) {
            tablero[i][columna] = jugador;
            break;
        }
    }
}

void deshacerMovimiento(int columna) {
    for (int i = 0; i < FILAS; ++i) {
        if (tablero[i][columna] != VACIO) {
            tablero[i][columna] = VACIO;
            break;
        }
    }
}

char cambiarJugador(char jugador) {
    return (jugador == JUGADOR) ? MAQUINA : JUGADOR;
}

void imprimirTablero() {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool hacerMovimiento(int columna, char jugador) {
    if (columna < 0 || columna >= COLUMNAS) {
        cout << "Columna no válida. Elige otra columna." << endl;
        return false;
    }

    if (!esMovimientoValido(columna)) {
        cout << "Columna llena. Elige otra columna." << endl;
        return false;
    }

    realizarMovimiento(columna, jugador);
    return true;
}

bool esEmpate() {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if (tablero[i][j] == VACIO) {
                return false;
            }
        }
    }
    return true;
}

bool verificarGanador(char jugador) {
    // Verificar en las filas
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == jugador &&
                tablero[i][j + 1] == jugador &&
                tablero[i][j + 2] == jugador &&
                tablero[i][j + 3] == jugador) {
                    if (jugador == JUGADOR) {
        contadorGanadasJugador++;
    } else if (jugador == MAQUINA) {
        contadorGanadasMaquina++;
    }
                return true; // Conexión ganadora en la fila
            }
        }
    }

    // Verificar en las columnas
    for (int j = 0; j < COLUMNAS; ++j) {
        for (int i = 0; i <= FILAS - 4; ++i) {
            if (tablero[i][j] == jugador &&
                tablero[i + 1][j] == jugador &&
                tablero[i + 2][j] == jugador &&
                tablero[i + 3][j] == jugador) {
                    if (jugador == JUGADOR) {
        contadorGanadasJugador++;
    } else if (jugador == MAQUINA) {
        contadorGanadasMaquina++;
    }
                return true; // Conexión ganadora en la columna
            }
        }
    }

    // Verificar en diagonales ascendentes
    for (int i = 3; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == jugador &&
                tablero[i - 1][j + 1] == jugador &&
                tablero[i - 2][j + 2] == jugador &&
                tablero[i - 3][j + 3] == jugador) {
                    if (jugador == JUGADOR) {
        contadorGanadasJugador++;
    } else if (jugador == MAQUINA) {
        contadorGanadasMaquina++;
    }
                return true; // Conexión ganadora en la diagonal ascendente
            }
        }
    }

    // Verificar en diagonales descendentes
    for (int i = 0; i <= FILAS - 4; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == jugador &&
                tablero[i + 1][j + 1] == jugador &&
                tablero[i + 2][j + 2] == jugador &&
                tablero[i + 3][j + 3] == jugador) {
                    if (jugador == JUGADOR) {
        contadorGanadasJugador++;
    } else if (jugador == MAQUINA) {
        contadorGanadasMaquina++;
    }
                return true; // Conexión ganadora en la diagonal descendente
            }
        }
    }

    return false; // No hay conexión ganadora
}

bool esJuegoTerminado() {
    return verificarGanador(JUGADOR) || verificarGanador(MAQUINA) || esEmpate();
}

void guardarPartida(char jugador) {
    ofstream archivo(ARCHIVO_CSV, ios::app);
    if (archivo.is_open()) {
        archivo << "[";
        for (int i = 0; i < FILAS; ++i) {
            for (int j = 0; j < COLUMNAS; ++j) {
                archivo << tablero[i][j];
                if (j < COLUMNAS - 1) {
                    archivo << ",";
                }
            }
        }
        archivo << "]" << "," << jugador << endl;
        archivo.close();
    }
}

int evaluarLinea(const vector<char>& linea) {
    int valor = 0;
    int contadorJugador = 0;
    int contadorMaquina = 0;

    // Recorrer la línea para contar las fichas consecutivas
    for (char ficha : linea) {
        if (ficha == JUGADOR) {
            contadorJugador++;
        } else if (ficha == MAQUINA) {
            contadorMaquina++;
        }
    }

    // Puntuación por fichas consecutivas
    if (contadorJugador > 0) {
        valor += pow(10, contadorJugador - 1);
    }
    if (contadorMaquina > 0) {
        valor -= pow(10, contadorMaquina - 1);
    }

    return valor;
}

int evaluarTablero(const vector<vector<char>>& tablero) {
    int valor = 0;

    // Evaluar filas
    for (int i = 0; i < FILAS; ++i) {
        valor += evaluarLinea(tablero[i]);
    }

    // Evaluar columnas
    for (int j = 0; j < COLUMNAS; ++j) {
        vector<char> columna;
        for (int i = 0; i < FILAS; ++i) {
            columna.push_back(tablero[i][j]);
        }
        valor += evaluarLinea(columna);
    }

    // Evaluar diagonales ascendentes
    for (int i = 3; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            vector<char> diagonal;
            for (int k = 0; k < 4; ++k) {
                diagonal.push_back(tablero[i - k][j + k]);
            }
            valor += evaluarLinea(diagonal);
        }
    }

    // Evaluar diagonales descendentes
    for (int i = 0; i <= FILAS - 4; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            vector<char> diagonal;
            for (int k = 0; k < 4; ++k) {
                diagonal.push_back(tablero[i + k][j + k]);
            }
            valor += evaluarLinea(diagonal);
        }
    }

    return valor;
}

pair<vector<vector<char>>, char> cargarPartida() {
    ifstream archivo(ARCHIVO_CSV);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            size_t pos = linea.find(",");
            string tableroStr = linea.substr(0, pos);
            char jugador = linea[pos + 1];

            vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS));
            for (int i = 0; i < FILAS; ++i) {
                for (int j = 0; j < COLUMNAS; ++j) {
                    tablero[i][j] = tableroStr[i * COLUMNAS + j];
                }
            }

            return make_pair(tablero, jugador);
        }
        archivo.close();
    }

    return make_pair(vector<vector<char>>(FILAS, vector<char>(COLUMNAS, VACIO)), JUGADOR);
}

int alfaBetaMax(char jugador, int profundidad, int alfa, int beta);

int alfaBetaMin(char jugador, int profundidad, int alfa, int beta) {
    if (profundidad == 0 || esJuegoTerminado()) {
        return evaluarTablero(tablero);
    }

    int peorValor = INF;

    for (int columna = 0; columna < COLUMNAS; ++columna) {
        if (esMovimientoValido(columna)) {
            realizarMovimiento(columna, jugador);
            int valor = alfaBetaMax(cambiarJugador(jugador), profundidad - 1, alfa, beta);
            deshacerMovimiento(columna);

            peorValor = min(peorValor, valor);
            beta = min(beta, peorValor);

            if (beta <= alfa) {
                break; // Poda alfa-beta
            }
        }
    }

    return peorValor;
}

int alfaBetaMax(char jugador, int profundidad, int alfa, int beta) {
    if (profundidad == 0 || esJuegoTerminado()) {
        return evaluarTablero(tablero);
    }

    int mejorValor = -INF;

    for (int columna = 0; columna < COLUMNAS; ++columna) {
        if (esMovimientoValido(columna)) {
            realizarMovimiento(columna, jugador);
            int valor = alfaBetaMin(cambiarJugador(jugador), profundidad - 1, alfa, beta);
            deshacerMovimiento(columna);

            mejorValor = max(mejorValor, valor);
            alfa = max(alfa, mejorValor);

            if (beta <= alfa) {
                break; // Poda alfa-beta
            }
        }
    }

    return mejorValor;
}

int obtenerPeorMovimiento(char jugador, int profundidad) {
    int mejorValor = INF;
    int peorMovimiento = -1;

    for (int columna = 0; columna < COLUMNAS; ++columna) {
        if (esMovimientoValido(columna)) {
            realizarMovimiento(columna, jugador);
            int valor = alfaBetaMax(cambiarJugador(jugador), profundidad - 1, -INF, INF);
            deshacerMovimiento(columna);

            if (valor < mejorValor) {
                mejorValor = valor;
                peorMovimiento = columna;
            }
        }
    }

    return peorMovimiento;
}

int obtenerColumnaIA(string dificultad) {
    if (dificultad == "facil") {
        int columna = obtenerPeorMovimiento(MAQUINA, 3); // Profundidad para el nivel fácil
        return columna;
    } else if (dificultad == "medio") {
         int columna = obtenerPeorMovimiento(MAQUINA, 5); // Profundidad para el nivel normal
        return columna;
    } else if (dificultad == "dificil") {
        int columna = alfaBetaMax(MAQUINA, 5, -INF, INF); // Profundidad para el nivel difícil
        return columna;
    }

    return -1; // Valor por defecto en caso de error
}

void menu(){
    while (true) {
        // Menú de dificultad
        cout << "Elige la dificultad: " << endl;
        cout << "1. Fácil" << endl;
        cout << "2. Medio" << endl;
        cout << "3. Difícil" << endl;

        int opcion;
        cin >> opcion;

        string dificultad;
        switch (opcion) {
            case 1:
                dificultad = "facil";
                break;
            case 2:
                dificultad = "medio";
                break;
            case 3:
                dificultad = "dificil";
                break;
            case 4:
                cout<<"el numero de partidas ganadas por el jugador es: "<< contadorGanadasJugador<<"el numero de partidas ganadas por la maquina es:" <<contadorGanadasMaquina;
                break;
            default:
                cout << "Opción no válida. Seleccionando dificultad fácil por defecto." << endl;
                dificultad = "facil";
                break;
        }
        // Cargar la partida o iniciar un nuevo juego
        pair<vector<vector<char>>, char> partidaGuardada = cargarPartida();
        tablero = partidaGuardada.first;
        char jugadorActual = partidaGuardada.second;

        while (true) {
            imprimirTablero();

            if (jugadorActual == JUGADOR) {
                int columna;
                cout << "Tu turno. Elige una columna (0-6): \npara guardar la partida y dar por terminado el programa poner -1";
                cin >> columna;
                if(columna!=-1){
                if (hacerMovimiento(columna, jugadorActual)) {
                    if (verificarGanador(jugadorActual)) {
                        imprimirTablero();
                        cout << jugadorActual << " ha ganado!" << endl;
                        break;
                    } else if (esEmpate()) {
                        imprimirTablero();
                        cout << "Empate." << endl;
                        break;
                    }
                    jugadorActual = MAQUINA;
                    
                } else {
                    // Se añadió un mensaje para informar al usuario sobre la elección incorrecta de la columna
                    cout << "Columna inválida. Elige otra columna." << endl;
                }
                } else if (columna==-1){
                    guardarPartida(jugadorActual);
                    menu();
                    
                }
            } else {
                int columnaIA = obtenerColumnaIA(dificultad);

                if (columnaIA != -1 && hacerMovimiento(columnaIA, jugadorActual)) {
                    if (verificarGanador(jugadorActual)) {
                        imprimirTablero();
                        cout << jugadorActual << " ha ganado!" << endl;
                        break;
                    } else if (esEmpate()) {
                        imprimirTablero();
                        cout << "Empate." << endl;
                        break;
                    }
                    jugadorActual = JUGADOR;
                    
                } else {
                    // Se añadió un mensaje para informar al usuario sobre un error en el movimiento de la IA
                    cout << "Error en el movimiento de la IA. Terminando el juego." << endl;
                    break;
                }
            }
        }
    }
}

void continuarOtraPartida(){
    // Preguntar al usuario si desea continuar o salir
        cout << "¿Desea continuar ? (1: Sí, 0: No): ";
        int continuar;
        cin >> continuar;

        if (continuar == 0) {
            cout << "¡Gracias por jugar! Hasta luego." << endl;
            // Salir del bucle principal y terminar el programa
        }
        else if(continuar==1){
            menu();
        }
}

int main() {
    

    cout << "Bienvenido a Conecta 4" << endl;

        menu();
        return 0;
}
