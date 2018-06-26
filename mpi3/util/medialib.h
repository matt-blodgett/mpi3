#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <string>
#include <vector>


std::string generatePID();


class Mpi3Element
{

//public:
//    std::string pid() const;
//    std::string name() const;
//    std::string added() const;

public:
    std::string pid = NULL;
    std::string name = NULL;
    std::string added = NULL;

};


class Mpi3Song : public Mpi3Element
{

private:
    std::string artist;
    std::string album;
    std::string time;
    std::string path;
    std::string kind;

    int size;
    int sampleRate;
    int bitRate;

};


class Mpi3Playlist : public Mpi3Element
{

private:
    std::string folder;
    std::vector<std::string> songs;

};


class Mpi3Folder : public Mpi3Element
{

private:
    std::string folder;
    std::vector<std::string> playlists;

};


class Mpi3Library : public Mpi3Element
{

private:
    std::string filepath;

};



#endif // MEDIALIB_H
