#include <iostream>
#include "MPI_Service.hpp"
#include "Lamport.hpp"
#include "Mutex.hpp"
#include "Leader.hpp" 

using namespace std;

void join(){
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    if(mpi->get_rank() == 0){
        char aux;
        cin >> aux;
        mpi->send_message(JOIN, 1, clock->timestamp);
        mpi->send_message(JOIN, 2, clock->timestamp);
    }else{
        int message;
        do{
            mpi->receive_message(&message, 0);
        }while(message != JOIN);
    }
}

int main(int argc, char** argv){

    MPI_Service *mpi = MPI_Service::get_singleton();

    int size = mpi->get_size();
    int rank = mpi->get_rank();

    Lamport *clock = Lamport::get_clock(rank);

    //Showing Lamport's Algorithm --------------------------------------------------------

    clock->update();
    cout << "Rank " << rank << " timestamp before messages: " << clock->timestamp << endl;

    for(int i = 0; i < 2; i++){
        if(rank == 0){
            mpi->send_message(SYNC_CLOCK, 1, clock->timestamp);

            int message;
            mpi->receive_message(&message, 2);
            //The receive_message method calls clock->sync already.
        }
        else if(rank == 1){
            int message;
            mpi->receive_message(&message, 0);

            mpi->send_message(SYNC_CLOCK, 2, clock->timestamp);
        }
        else if(rank == 2){
            int message;
            mpi->receive_message(&message, 1);

            mpi->send_message(SYNC_CLOCK, 0, clock->timestamp);
        }
    }
    cout << "Rank " << rank << " timestamp after messages: " << clock->timestamp << endl;
    //-------------------------------------------------------------------------------------

    join();

    //-------------------MUTEX-------------------------------------------------------------
    clock->update();
    
    if(rank == COORD_RANK){
        Mutex_Coord *coord = new Mutex_Coord();
        coord->start_receiving();
        delete(coord);
    }
    else{

        Mutex_Slave *slave = new Mutex_Slave();
        slave->request_mutex();

        for(int i = 0; i < 10; i++)
            cout << "Rank " << rank << " tem o mutex." << endl;

        slave->free_mutex();
        Mutex_Coord::stop_receiving();

        delete(slave);
    }

    //--------------------------------------------------------------------------------------

    join();

    //---------------LIDER------------------------------------------------------------------
    
    Leader *leader = new Leader();

    while(1){
        leader->check_leader();
        if(mpi->get_rank() == 2){
            cout << "Terminando rank 2." << endl;
            break;
        }
    }

    delete(leader);
    delete(clock);
    MPI_Service::destroy();

    join();

    return 0;
}
