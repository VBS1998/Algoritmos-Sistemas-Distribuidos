#include "Lamport.hpp"

Lamport::Lamport(int rank){
    timestamp = 0;
    increment = 1 << rank;
};

Lamport* Lamport::singleton = NULL;

Lamport* Lamport::get_clock(){
    if(singleton == NULL) singleton = new Lamport(0);
    return singleton;
}

Lamport* Lamport::get_clock(int rank){
    if(singleton == NULL) singleton = new Lamport(rank);
    return singleton;
}

void Lamport::update(){
    timestamp += increment;
};

void Lamport::sync(int time){
    timestamp = timestamp > time ? timestamp : time;
    timestamp++;
};