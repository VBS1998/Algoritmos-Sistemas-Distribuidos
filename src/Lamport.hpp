#ifndef LAMPORT_H
#define LAMPORT_H

#include <cstddef>

class Lamport{

private:
    Lamport(int rank);

    static Lamport *singleton;

public:

    static Lamport *get_clock();
    static Lamport *get_clock(int rank);

    int timestamp;
    int increment;

    void update();

    void sync(int time);

};

#endif