#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

// Estructura del proceso
typedef struct{
    int pid;
    int pc;     
    int ax,bx,cx;
    int quantum;
    char estado[10];
    std::vector <std::string> instruccionesq;
} Proceso;

// Funcion para obtener el valor entero del registro a evaluar
int obtenerValor(std::string& operando,const Proceso& p){
    if(operando=="AX")return p.ax;
    if(operando=="BX")return p.bx;
    if(operando=="CX")return p.cx;
    return std::stoi(operando);
}

// Funcion para asignar el valor nuevo a los registros
void asignarRegistro(std::string& registro, int valor, Proceso& p){
    if(registro == "AX") p.ax = valor;
    else if(registro == "BX") p.bx = valor;
    else if(registro == "CX") p.cx = valor;
}

// Funcion para ejecutar la instruccion en base a la operacion 
void ejecutarInstruccion(Proceso& p, std::string& instruccion){
    std::istringstream iss(instruccion);
    std::string operacion, destino, fuente;

    iss >> operacion;

    if(operacion=="NOP") return;
    else if(operacion=="INC"){
        iss >> destino;
        int valor = obtenerValor(destino, p) + 1;
        asignarRegistro(destino, valor, p);
    }
    else if(operacion=="ADD"||operacion == "SUB"){
        getline(iss,destino,',');
        getline(iss,fuente);

        destino.erase(0, destino.find_first_not_of(" "));
        fuente.erase(0,fuente.find_first_not_of(" "));

        int valDest = obtenerValor(destino,p);
        int valFuente = obtenerValor(fuente,p);

        int resultado = (operacion == "ADD") ? valDest + valFuente : valDest - valFuente;

        asignarRegistro(destino, resultado, p);
    }
    else {  
        std::cout <<"Instruccion desconocida" << instruccion << std::endl;
    }
}


int main() {
    Proceso p1;
    p1.pid = 1;
    p1.pc = 0;
    p1.ax = 2;
    p1.bx = 3;
    p1.cx = 1;
    p1.quantum = 3;
    strcpy(p1.estado, "Listo");

    p1.instrucciones = {
        "ADD AX, 5",
        "SUB BX, AX",
        "INC CX",
        "NOP"
    };

    std::cout << "Estado inicial: AX=" << p1.ax << " BX=" << p1.bx << " CX=" << p1.cx << " Quantum=" << p1.quantum << std::endl;

    for (int i = 0; i < p1.instrucciones.size(); ++i) {
        std::cout << "\nInstrucción #" << i + 1 << ": " << p1.instrucciones[i] << std::endl;
        ejecutarInstruccion(p1, p1.instrucciones[i]);
        std::cout << "AX=" << p1.ax << " BX=" << p1.bx << " CX=" << p1.cx << std::endl;
    }

    std::cout << "\nProceso terminado.\n";
    return 0;
}