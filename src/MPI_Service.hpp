#ifndef MPI_SERVICE_H
#define MPI_SERVICE_H

#include <mpi.h>
#include <iostream>
#include "Lamport.hpp"

#define SYNC_CLOCK 0

#define MUTEX_REQUEST 1
#define MUTEX_FREE 2
#define STOP_RECEIVING 3

#define LEADER_CHECK 4
#define ELECTION 5
#define ELECTION_ACK 6

#define JOIN 7

using namespace std;

class MPI_Service{
    
private:
    MPI_Service();
    ~MPI_Service();

    static MPI_Service *service;

public:

    static MPI_Service *get_singleton();

    static void destroy();

    int get_rank();
    int get_size();

    int send_message(int message, int dest, int timestamp);
    int send_message_nonblocking(int message, int dest, int timestamp);
    int receive_message(int *ret, int src);
    int receive_message(int *ret, int src, MPI_Status *status);
    int receive_message_nonblocking(int *ret, int src, MPI_Request *request, MPI_Status *status);
};

#endif