#include "medialib.h"

#include <random>
#include <time.h>


std::string generatePID()
{

    srand (time(NULL));

    std::string clist [36] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R",
                              "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::string pid;
    for (int i = 0; i < 16; i++)
    {
        pid += clist[rand() % 36];
    }

    return pid;
}

