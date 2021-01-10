#ifndef MUTEX_H
#define MUTEX_H

#include <queue>
#include "MPI_Service.hpp"
#include "Lamport.hpp"

#define COORD_RANK 1

#define FREE 0
#define BUSY 1

class Mutex_Coord{

private:
    int mutex;
    bool receiving;

    queue<int> *Q;

public:

    Mutex_Coord();
    ~Mutex_Coord();

    void receive_resquests();

    void start_receiving();
    static void stop_receiving();

};

class Mutex_Slave{

public:
    int request_mutex();
    int free_mutex();
};

#endif