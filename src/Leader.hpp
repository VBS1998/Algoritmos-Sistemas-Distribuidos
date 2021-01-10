#include <unistd.h>
#include "MPI_Service.hpp"

class Leader{

public:
    int leader_rank;

    Leader();

    int start_election();
    int check_leader();
};