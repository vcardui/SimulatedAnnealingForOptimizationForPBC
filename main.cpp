/*
25 de septiembre de 2025
Vanessa Reteguín - 375533

Algoritmo de recosido simulado para placa de PBC

Universidad Aútonoma de Aguascalientes
Centro de ciencias básicas
Ciencias de la computación
Ingeniería en Computación Inteligente (ICI)
Semestre V

Grupo 5 - A
Optimización Inteligente
Luis Fernando Gutiérrez Marfileño

Instrucciones:

1. Ubicar 8 componentes electrónicos en una PCB de 50mm x 50mm buscando
minimizar la longitud total del cableado. Los componentes son:
    1. Un capacitor (C)
    2. Un diodo (D)
    3. Un sensor (S)
    4. Un procesador (P)
    5. Un resistor (R)
    6. Una bobina (B)
    7. Un transistor (T)
    8. Un led (L)

2. La matriz de adyacencia de las conexiones es la siguiente:
    P C S T D L R B
    P 0 0 1 1 0 1 0 0
    C 0 0 0 0 0 0 1 1
    S 1 0 0 1 0 0 0 0
    T 1 0 1 0 0 1 0 0
    D 0 0 0 0 0 1 1 0
    L 1 0 0 1 1 0 0 0
    R 0 1 0 0 1 0 0 0
    B 0 1 0 0 0 0 0 0

3. Función objetivo

    $$
    c = min \sum_{i,j}^{n}{\sqrt{(x_{i}-x_{j})^2+(y_{i}-y_{j})^2}}
    $$

4. Restricciones:
    1. Sin superposición. (es decir dos o más elementos no pueden compartir la
       misma coordenada de referencia
    2. Dentro de los límites físicos (dar un espacio de -15 a +15 a cada
       componente).

5. El mecanismo de decisión o Regla de aceptación está dado por:
    Sí S(x) > S(x’) entonces x= x’ si no, la probabilidad de que x’ se acepte
    es:

    $$
    P(\Delta Z) = exp(\frac{\Delta Z}{CT})
    $$

    Donde:

    - ∆Z = (Z(x′) − Z(x))
    - C = 1,38054 × 10−23 (Cte. de Boltzman) pero en este caso C = 1
    - T es la temperatura
      Se escoge un numero aleatorio n uniformemente distribuido en el intervalo
      (0,1), ese número es comparado con P(∆Z)
    - Si n < P(∆Z), x’ reemplaza a x como solución actual
    - Si n >= P(∆Z), x se usa de nuevo como paso inicial de una próxima
      iteración Al principio T es un valor alto (fundición), y luego va
      disminuyendo (enfriamiento), es decir, cada vez es menor la probabilidad
      de que la nueva alternativa reemplace a la anterior

6. El mecanismo de direccionamiento o Plan de enfriamiento se define por:
   Una sucesión finita de valores del parámetro de control inicia con los
   siguientes valores del rango de temperatura, pero si las distancias obtenidas
   son mayores que las dimensiones de la placa, busca reducir el rango de
   temperatura hasta lograr distancias totales menores:
        - Un valor inicial del parámetro, $T_{0}$ (1000)
        - Una función para decrementar el parámetro de control $\alpha$
        - Un valor final del parámetro de control $T_{f}$(20)
        - Numero máximo de iteraciones (2000)
        - Parámetro de control $\alpha$ (0.8)
        - Función de enfriamiento:

        $$
        T_{k} = \alpha[\frac{T_{0}}{1+k}]
        $$


    Para la creación del programa deberán realizarse los siguientes pasos:

7. En las primeras líneas elaborar comentarios con la siguiente información:
    1. Nombre de la institución
    2. Nombre del centro al que pertenece la carrera
    3. Nombre del departamento al que pertenece la carrera
    4. Nombre de la materia
    5. Nombre(s) de quien(es) realiza(n) la práctica
    6. Nombre del profesor
    7. Una descripción breve de lo que realiza el programa

8. Incluir las librerías necesarias.

9. Desplegar una explicación breve del problema.

10. Desplegar la última solución y su evaluación (la ubicación de cada
    componente mediante su coordenada de referencia y la distancia total
    calculada).

11. Una vez realizada esta operación, se debe regresar al menú principal.

12. Al salir se debe detener el programa y luego regresar el control al sistema
    inicial.
*/

/* ------------ Resources / Documentation involved ------------- */
// 2D Vector in C++
// https://www.geeksforgeeks.org/cpp/2d-vector-in-cpp-with-user-defined-size/

// Rectange area for desmos: 0<y<80\left\{0<x<100\right\}

/* ------------------------- Libraries ------------------------- */
#include <iostream> /* cin/cout */
#include <string>
#include <utility> /* std:pair */
#include <vector>
using namespace std;

/* ------------------------ Constraints ------------------------ */

class Component {
   public:
    string fullname;
    char shortname;
    double x;
    double y;
};

class Solution {
   public:
    int floorWidth;
    int floorHeight;

    int maxIterations;
    float initialTemperature;
    float currentTemperature;
    float finalTemperature;
    float alpha;

    int componentsQuantity;

    vector<vector<int>> conectionns = {};

    double totalDistance(Component components[]) {
        double cost = 0.0;

        for (int i = 0; i < conectionns.size(); i++) {
            for (int j = i + 1; j < conectionns.size(); j++) {
                if (conectionns[i][j]) {
                    cost += distance(components[i], components[j]);
                }
            }
        }
        return cost;
    }

    void disturb(Component components[]) {
        int i;
        i = rand() % componentsQuantity;

        components[i].x +=
            ((rand() % 21) - 10);  // movement between -10 and +10
        components[i].y += ((rand() % 21) - 10);

        // PCB limits

        if (components[i].x < 0) {
            components[i].x = 0;
        }

        if (components[i].x > floorWidth) {
            components[i].x = floorWidth;
        }

        if (components[i].y < 0) {
            components[i].y = 0;
        }

        if (components[i].y > floorHeight) {
            components[i].y = floorHeight;
        }
    }

    void copy(Component destination[], Component source[]) {
        for (int i = 0; i < componentsQuantity; i++) {
            destination[i] = source[i];
        }
    }

    static double distance(Component a, Component b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
};

/* ------------------------- Functions ------------------------- */
void displayTitle() {
    cout << endl
         << "\n_______        __________           _____             _________ "
            "            "
            "\n__  __ \\_________  /___(_)______ ______(_)___________ ______  "
            "/_____________"
            "\n_  / / /__  __ \\  __/_  /__  __ `__ \\_  /___  /_  __ `/  __  "
            "/_  __ \\_  ___/"
            "\n/ /_/ /__  /_/ / /_ _  / _  / / / / /  / __  /_/ /_/ // /_/ / / "
            "/_/ /  /    "
            "\n\\____/ _  .___/\\__/ /_/  /_/ /_/ /_//_/  _____/\\__,_/ "
            "\\__,_/  \\____//_/     "
            "\n       /_/                                                      "
            "            "
            "\n_________          _________________________"
            "\n______  /____      ___  __ \\__  __ )_  ____/"
            "\n_  __  /_  _ \\     __  /_/ /_  __  |  /     "
            "\n/ /_/ / /  __/     _  ____/_  /_/ // /___   "
            "\n\\__,_/  \\___/      /_/     /_____/ \\____/\n";
}

void diplayMenu() {
    cout << endl
         << "\n.--------------------."
            "\n||    -{ MENU }-    ||"
            "\n.--------------------."
            "\n| [1] Optimizar PBC  |"
            "\n|         [2] Salir  |"
            "\n.--------------------.\n";
}

void endTitle() {
    cout << "\n  ^~^  , * ------------- *"
            "\n ('Y') ) |  Hasta luego! | "
            "\n /   \\/  * ------------- *"
            "\n(\\|||/)        FIN      \n";
}

void printAdjacencyMatrix() {
    cout << endl
         << "\n------------------------ { Matriz de adyacencia } "
            "-------------------------"
            "\n           Procesador Capacitor Sensor Transistor Diodo Led "
            "Resistor Bobina"
            "\nProcesador     0         0        1        1        0    1     "
            "0       0"
            "\nCapacitor      0         0        0        0        0    0     "
            "1       1"
            "\nSensor         1         0        0        1        0    0     "
            "0       0"
            "\nTransistor     1         0        1        0        0    1     "
            "0       0"
            "\nDiodo          0         0        0        0        0    1     "
            "1       0"
            "\nLed            1         0        0        1        1    0     "
            "0       0"
            "\nResistor       0         1        0        0        1    0     "
            "0       0"
            "\nBobina         0         1        0        0        0    0     "
            "0       0\n";
}

int main() {
    /* ------------------------- Variables ------------------------- */
    /* - Menu - */
    int userChoice;
    bool run = true;

    /* - Algorithm - */
    int i;
    double currentDistance, bestDistance, newDistance;

    /* --------------------------- Code ---------------------------- */
    displayTitle();
    while (run == true) {
        diplayMenu();
        while (!((cin >> userChoice) && (userChoice >= 1 && userChoice <= 2))) {
            cin.clear();
            cin.ignore();
        }

        switch (userChoice) {
            case 1: {
                srand(time(NULL));

                Solution solution;
                solution.componentsQuantity = 8;
                solution.floorWidth = 100;
                solution.floorHeight = 80;
                solution.maxIterations = 10000;
                solution.initialTemperature = 1080.0;
                solution.finalTemperature = 25.0;
                solution.alpha = 0.99;

                solution.conectionns.push_back({0, 1, 0, 0, 1, 0});
                solution.conectionns.push_back({1, 0, 1, 0, 0, 0});
                solution.conectionns.push_back({0, 1, 0, 1, 0, 1});
                solution.conectionns.push_back({0, 0, 1, 0, 0, 1});
                solution.conectionns.push_back({1, 0, 0, 0, 0, 1});
                solution.conectionns.push_back({0, 0, 1, 1, 1, 0});

                // Components names
                vector<pair<string, char>> componentNames = {
                    {"Procesador", 'P'}, {"Capacitor", 'C'}, {"Sensor", 'S'},
                    {"Transistor", 'T'}, {"Diodo", 'D'},     {"Led", 'L'},
                    {"Resistor", 'R'},   {"Bobina", 'B'}};

                Component currentSolution[solution.componentsQuantity];
                Component bestSolution[solution.componentsQuantity];
                Component newSolution[solution.componentsQuantity];

                // Random initialization values
                for (i = 0; i < solution.componentsQuantity; i++) {
                    currentSolution[i].x = rand() % solution.floorWidth;
                    currentSolution[i].y = rand() % solution.floorHeight;
                }

                solution.copy(bestSolution, currentSolution);

                // Hasta aqui todo bien

                currentDistance = solution.totalDistance(currentSolution);
                bestDistance = currentDistance;

                // Start warm-up process
                for (i = 0;
                     i < solution.maxIterations && solution.currentTemperature;
                     i++) {
                    solution.copy(newSolution, currentSolution);
                    solution.disturb(newSolution);

                    // Evaluate solution's fitness
                    newDistance = solution.totalDistance(newSolution);

                    if ((newDistance < currentDistance) ||
                        (exp((currentDistance - newDistance) /
                             solution.currentTemperature) > ((double)rand()))) {
                        solution.copy(currentSolution, newSolution);
                        currentDistance = newDistance;

                        if (newDistance < newDistance) {
                            solution.copy(bestSolution, newSolution);
                            bestDistance = newDistance;
                        }
                    }

                    // Determinar ritmo de enfriamiento
                    solution.currentTemperature *= solution.alpha;
                }

                cout << endl << "Ubicacion óptima de 6 componentes: \n" << endl;
                for (i = 0; i < solution.componentsQuantity; i++) {
                    cout

                        << i + 1 << ": (" << bestSolution[i].x << ", "
                        << bestSolution[i].y << ")\t- "
                        << componentNames[i].first << " ("
                        << componentNames[i].second << ")" << endl;
                }

                for (i = 0; i < solution.componentsQuantity; i++) {
                    cout

                        << "(" << bestSolution[i].x << ", " << bestSolution[i].y
                        << ")" << endl;
                }

                cout << endl << "Costo total minimo: " << bestDistance;

                printAdjacencyMatrix();
            } break;

            case 2:
                endTitle();
                run = false;
                break;
        }
    }

    return 0;
}