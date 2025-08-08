#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <fstream>

typedef struct{
    int pid = 0;
    int pc = 0;     
    int ax = 0,bx = 0,cx = 0;
    int quantum = 0;
    char estado[10]; // "Listo", "Ejecutando", "Terminado"
    std::vector <std::string> instrucciones;

    std::string printRegisters(){
        std::stringstream ss;
        ss << "AX: " << this->ax << ", BX: " << this->bx << ", CX: " << this->cx << " , PC: " << this->pc;
        return ss.str();
    }

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

// Cambiar estado de los procesos
void cambiarEstado(Proceso& p, const std::string& nuevoEstado){
    strcpy(p.estado, nuevoEstado.c_str());
}

// Funcion para ejecutar la instruccion en base a la operacion 
// ADD, INC, NOP, SUB, JMP
int ejecutarInstruccion(Proceso& p, std::string instruccion){
    std::istringstream iss(instruccion);
    std::string operacion, destino, fuente;

    iss >> operacion;

    if(operacion=="JMP"){
        std::string jump;
        iss >> jump;
        return std::stoi(jump);
    }
    else if(operacion=="NOP");
    else if(operacion=="INC"){
        iss >> destino;
        int valor = obtenerValor(destino, p) + 1;
        asignarRegistro(destino, valor, p);
    }
    else if(operacion=="ADD"||operacion=="SUB"||operacion=="MUL"){
        iss >> destino; destino = destino.substr(0,2);
        iss >> fuente; fuente = fuente.substr(0,2);

        int valDest = obtenerValor(destino,p);
        int valFuente = obtenerValor(fuente,p);

        int resultado;
        if (operacion == "ADD") resultado = valDest + valFuente;
        else if (operacion == "SUB") resultado = valDest - valFuente;
        else resultado = valDest * valFuente;

        asignarRegistro(destino, resultado, p);
    }
    else {  
        std::cout <<"Instrucción desconocida" << instruccion << std::endl;
    }
    return -1;
}

void roundRobin(std::vector<Proceso>& procesos){
    int cycles = 0;
    int wProceso = 0; // índice del proceso actual
    int i = 0;

    // Contar todos los ciclos a ejecutar (número total de instrucciones)
    for (int k = 0; k < procesos.size(); k++) {
        cycles += procesos[k].instrucciones.size();
        cambiarEstado(procesos[k], "Listo"); // Inicializamos todos como "Listo"
    }

    while (i < cycles) {
        Proceso &proceso = procesos[wProceso++];

        // 🔒 Saltar proceso si ya terminó
        if (strcmp(proceso.estado, "Terminado") == 0) {
            if (wProceso == procesos.size()) wProceso = 0;
            continue;
        }

        for (int j = proceso.quantum; j > 0; j--) {
            // ✅ Si ya no quedan instrucciones, marcar como terminado
            if (proceso.pc >= proceso.instrucciones.size()) {
                cambiarEstado(proceso, "Terminado");
                break;
            }

            cambiarEstado(proceso, "Ejecutando");
            std::cout << "\n----------------------- CICLO " << ++i << " -----------------------" << std::endl;
            std::cout << "Proceso activo: " << proceso.pid << std::endl;
            std::cout << "Instruccion: " << proceso.instrucciones[proceso.pc] << std::endl;
            std::cout << "Estado: " << proceso.estado << std::endl;
            std::cout << "Registros antes: " << proceso.printRegisters();
            std::cout << ", Quantum: " << j << std::endl;

            int jump = ejecutarInstruccion(proceso, proceso.instrucciones[proceso.pc]);

            if (jump == -1) {
                proceso.pc++;  // solo avanzar si no hubo salto
            } else {
                // 🧠 Ajustar ciclos para evitar que terminemos antes de tiempo
                cycles += std::abs(proceso.pc - jump);
                proceso.pc = jump;
            }

            std::cout << "Registros despues: " << proceso.printRegisters();
            std::cout << ", Quantum: " << j-1 << std::endl;

            // ✅ Actualizar estado según si terminó
            if (proceso.pc >= proceso.instrucciones.size()) {
                cambiarEstado(proceso, "Terminado");
            } else {
                cambiarEstado(proceso, "Listo");
            }

            std::cout << "Estado: " << proceso.estado << std::endl;
        }

        std::cout << "\n[Cambio de contexto]" << std::endl << std::endl;

        if (wProceso == procesos.size()) wProceso = 0;
    }
}

int main(){
    freopen("procesos.txt", "r", stdin);
    std::vector<Proceso> procesos;
    std::string line;
    std::string token; // part of each linee separete by space

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        Proceso procesoToInclude;
        iss >> token; procesoToInclude.pid = std::stoi(token.substr(4));
        iss >> token; procesoToInclude.ax = std::stoi(token.substr(3));
        iss >> token; procesoToInclude.bx = std::stoi(token.substr(3));
        iss >> token; procesoToInclude.cx = std::stoi(token.substr(3));
        iss >> token; procesoToInclude.quantum = std::stoi(token.substr(8));
        procesos.push_back(procesoToInclude);
    }

    for (int i = 0; i < procesos.size(); ++i) {
        std::string archivoNombre = std::to_string(i + 1) + ".txt";
        std::ifstream archivoInstrucciones(archivoNombre);

        if (!archivoInstrucciones) {
            std::cerr << "Error: No se encontro al archivo " << archivoNombre << std::endl;
            continue;
        }

        std::string instruccion;
        while (std::getline(archivoInstrucciones, instruccion)) {
            if (!instruccion.empty()) {
                procesos[i].instrucciones.push_back(instruccion);
            }
        }

        archivoInstrucciones.close();
    }

    roundRobin(procesos);
    return 0;
}
