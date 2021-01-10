#include "Mutex.hpp"

//----------------Coord---------------------------------
Mutex_Coord::Mutex_Coord(){
    mutex = FREE;
    receiving = false;
    Q = new queue<int>();
}

Mutex_Coord::~Mutex_Coord(){
    delete(Q);
}

void Mutex_Coord::receive_resquests(){
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    int message;
    MPI_Status status;

    while(receiving){
        mpi->receive_message(&message, MPI_ANY_SOURCE, &status);


        if(message == MUTEX_REQUEST){
            if(mutex == FREE){
                mutex = BUSY;
                mpi->send_message(MUTEX_REQUEST, status.MPI_SOURCE, clock->timestamp);
            }else{
                Q->push(status.MPI_SOURCE);
            }
        }
        else if(message == MUTEX_FREE){
            if(Q->empty()){
                mutex = FREE;
            }else{
                mpi->send_message(MUTEX_REQUEST, Q->front(), clock->timestamp);
                Q->pop();
            }
        }
        else if(message == STOP_RECEIVING){
            if(Q->empty() && mutex == FREE)
                receiving = false;
        }
    }
}

void Mutex_Coord::start_receiving(){
    receiving = true;
    receive_resquests();
}

void Mutex_Coord::stop_receiving(){
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();
    mpi->send_message(STOP_RECEIVING, COORD_RANK, clock->timestamp);
}


            


//------------------------Slaves-------------------------------

int Mutex_Slave::request_mutex(){
    
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    mpi->send_message(MUTEX_REQUEST, COORD_RANK, clock->timestamp);

    int message;
    mpi->receive_message(&message, COORD_RANK);

    return message;
}

int Mutex_Slave::free_mutex(){
    
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    return mpi->send_message(MUTEX_FREE, COORD_RANK, clock->timestamp);
}