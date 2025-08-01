#include <iostream>
#include <fstream>      // para leer archivos
#include <sstream>      // para procesar líneas
#include <string> 
#include <vector>
#include <cstring>
using namespace std;

typedef struct {
    int pid;
    int pc;             // contador de programa (inicialízalo en 0)
    int ax, bx, cx;     // registros simulados
    int quantum;
    char estado[10];    // "Listo", "Ejecutando", "Terminado"
} Proceso;

int main() {
    ifstream procesostxt("procesos.txt");

    if (!procesostxt.is_open()) {
        cerr << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    vector<string> lineas;
    string linea;

    while (getline(procesostxt, linea)) {
        lineas.push_back(linea);
    }

    procesostxt.close(); 

    for (size_t i = 0; i < lineas.size(); i++) {
        cout << lineas[i] << endl;
    }

    vector<Proceso> procesos;

    for (const string& linea : lineas) {
        Proceso pro;
        pro.pc = 0;
        strcpy(pro.estado, "Listo");

        istringstream iss(linea);
        string token;

        while (iss >> token) {
        if (token.find("PID=") != string::npos)
            pro.pid = stoi(token.substr(4));
        else if (token.find("AX=") != string::npos)
            pro.ax = stoi(token.substr(3));
        else if (token.find("BX=") != string::npos)
            pro.bx = stoi(token.substr(3));
        else if (token.find("CX=") != string::npos)
            pro.cx = stoi(token.substr(3));
        else if (token.find("Quantum=") != string::npos)
            pro.quantum = stoi(token.substr(8));
        }

        procesos.push_back(pro);

    }

    for (const Proceso& p : procesos) {
        cout << "PID=" << p.pid << " AX=" << p.ax << " BX=" << p.bx
        << " CX=" << p.cx << " Quantum=" << p.quantum 
        << " Estado=" << p.estado << " PC=" << p.pc << endl;
    }
    return 0;
}