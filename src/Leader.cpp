#include "Leader.hpp"

Leader::Leader()
{
    leader_rank = MPI_Service::get_singleton()->get_size() - 1;
}

int Leader::start_election()
{
    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    for (int i = 0; i < mpi->get_size(); i++)
    {
        if (i != mpi->get_rank())
        {
            //Duas vezes para nao ter perda
            mpi->send_message_nonblocking(ELECTION, i, clock->timestamp);
            mpi->send_message_nonblocking(ELECTION, i, clock->timestamp);

        }
    }

    int message, flag;
    MPI_Request request;
    MPI_Status status;

    while (1)
    {
        int timeout = mpi->receive_message_nonblocking(&message, MPI_ANY_SOURCE, &request, &status);
        if(timeout){
            leader_rank = mpi->get_rank();
            return 0;
        }

        if (message == ELECTION_ACK || message == LEADER_CHECK){
            if (mpi->get_rank() < status.MPI_SOURCE)
            {
                return 0;
            }
        }
        else if (message == ELECTION){
            mpi->send_message_nonblocking(ELECTION_ACK, status.MPI_SOURCE, clock->timestamp);
            if (mpi->get_rank() < status.MPI_SOURCE)
            {
                leader_rank = status.MPI_SOURCE;
                return 0;
            }
            
        }
    }

    return 0;
}

int Leader::check_leader()
{

    MPI_Service *mpi = MPI_Service::get_singleton();
    Lamport *clock = Lamport::get_clock();

    if (leader_rank == mpi->get_rank())
    {
        //Caso seja o rank 2, para efeitos de demonstracao, para de ser lider depois de 10 segundos
        for (int i = 0; i < 10 || mpi->get_rank() != 2; i++)
        { 
            for (int i = 0; i < mpi->get_size(); i++)
            {
                if (i != mpi->get_rank())
                {
                    mpi->send_message_nonblocking(LEADER_CHECK, i, clock->timestamp);
                }
            }
            cout << "Rank " << mpi->get_rank() << " é lider há " << i + 1 << " segundos." << endl;
            sleep(1);
        }
    }
    else
    {
        int message, flag;
        MPI_Request request;
        MPI_Status status;
        int timeout = mpi->receive_message_nonblocking(&message, MPI_ANY_SOURCE, &request, &status);

        if(timeout) start_election();
        
        if (message == ELECTION)
        {
            mpi->send_message_nonblocking(ELECTION_ACK, status.MPI_SOURCE, clock->timestamp);
            if (mpi->get_rank() > status.MPI_SOURCE)
            {
                start_election();
            }
            else
            {
                leader_rank = status.MPI_SOURCE;
            }
        }
    }
    return 0;
}