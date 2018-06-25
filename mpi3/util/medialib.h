#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <string>
#include <vector>


std::string generatePID();


class Mpi3Element
{

private:
    std::string pid = NULL;
    std::string name = NULL;
    std::string added = NULL;

};


class Folder : public Mpi3Element
{

private:
    std::string folder;
    std::vector<std::string> playlists;

};


class Playlist : public Mpi3Element
{

}


#endif // MEDIALIB_H
