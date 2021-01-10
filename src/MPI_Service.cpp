#include <unistd.h>

#include "MPI_Service.hpp"

MPI_Service::MPI_Service(){
    MPI_Init(NULL, NULL);
}

MPI_Service::~MPI_Service(){
    MPI_Finalize();
};

MPI_Service* MPI_Service::service = NULL;

/// Método que retorna instância única do MPI_Service
MPI_Service* MPI_Service::get_singleton(){
    if(service != NULL) return service;
    
    service = new MPI_Service();
    return service;
};

void MPI_Service::destroy(){
    delete(service);
}

/// Método para receber rank do processo
int MPI_Service::get_rank(){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
};

/// Método para receber quantidade de processos
int MPI_Service::get_size()
{
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;
};

/// Método para enviar mensagem bloqueante
int MPI_Service::send_message(int message, int dest, int timestamp)
{   
    if(dest < get_size()){
        int arr[2] = {message, timestamp};
        return MPI_Send(arr, 2, MPI_INT, dest, 1, MPI_COMM_WORLD);
    }
    return -1;
};

/// Método para enviar mensagem não bloqueante
int MPI_Service::send_message_nonblocking(int message, int dest, int timestamp)
{   
    if(dest < get_size()){
        int arr[2] = {message, timestamp};
        MPI_Request request;
        return MPI_Isend(arr, 2, MPI_INT, dest, 1, MPI_COMM_WORLD, &request);
    }
    return -1;
};

/// Método para receber mensagem bloqueante
int MPI_Service::receive_message(int *ret, int src)
{
    int message[2];
    int fail = MPI_Recv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (!fail)
    {
        *ret = message[0];
        int timestamp = message[1];

        //Sincroniza relógio lógico de Lamport
        Lamport::get_clock()->sync(timestamp);
    }
    return fail;
};

/// Método para receber mensagem bloqueante com status
int MPI_Service::receive_message(int *ret, int src, MPI_Status *status)
{
    int message[2];
    int fail = MPI_Recv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, status);
    if (!fail)
    {
        *ret = message[0];
        int timestamp = message[1];

        //Sincroniza relógio lógico de Lamport
        Lamport::get_clock()->sync(timestamp);
    }
    return fail;
};

/// Método para receber mensagem não bloqueante com timeout de 5 segundos
int MPI_Service::receive_message_nonblocking(int *ret, int src, MPI_Request *request, MPI_Status *status)
{
    
    int message[2];
    
    int fail = MPI_Irecv(message, 2, MPI_INT, src, 1, MPI_COMM_WORLD, request);

    int flag;
    bool timeout = false;

    //Timeout de 5 segundos
    int i = 0;
    do
    {
        MPI_Test(request, &flag, status);
        sleep(1);
        if (++i == 5)
        {
            timeout = true;
            break;
        }
    } while (!flag);

    if(timeout){
        return -1;
    }

    //Caso não de timeout
    if (!fail)
    {
        *ret = message[0];
        int timestamp = message[1];

        //Sincroniza relógio lógico de Lamport
        Lamport::get_clock()->sync(timestamp);
    }
    return fail;
};