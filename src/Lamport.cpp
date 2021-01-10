#include "Lamport.hpp"

Lamport::Lamport(int rank){
    timestamp = 0;
    increment = 1 << rank;
};

Lamport* Lamport::singleton = NULL;

/// Método que retorna instância única do Lamport
Lamport* Lamport::get_clock(){
    if(singleton == NULL) singleton = new Lamport(0);
    return singleton;
}

/// Método que retorna instância única do Lamport e cria com incremento baseado no rank caso já não exista
Lamport* Lamport::get_clock(int rank){
    if(singleton == NULL) singleton = new Lamport(rank);
    return singleton;
}

// Método que atualiza o Relógio
void Lamport::update(){
    timestamp += increment;
};

// Método que sincroniza o Relógio
void Lamport::sync(int time){
    timestamp = timestamp > time ? timestamp : time;
    timestamp++;
};