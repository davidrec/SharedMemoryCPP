// SharedMemory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace boost::interprocess;

void CreateSharedMemory(std::string* val)
{
    std::cout << "Will create Shared memory\n";
    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    //Create a shared memory object.
    shared_memory_object shm(create_only, "MySharedMemory", read_write);

    //Set size
    shm.truncate(50);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);

   
    std::memcpy(region.get_address(), val->c_str() , region.get_size());

    int stop;
    std::cin >> stop;
}

void ReadMemory()
{
    std::cout << "Will read the Shared memory\n\n";
    //Open already created shared memory object.
    shared_memory_object shm(open_only, "MySharedMemory", read_only);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_only);

    //Check that memory was initialized to 1
    char* mem = static_cast<char*>(region.get_address());
    for (std::size_t i = 0; i < region.get_size(); ++i)
        std::cout << mem;
}

void EditSharedMemory(std::string* val)
{
    std::cout << "Will edit the Shared memory\n\n";
    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    //Create a shared memory object.
    shared_memory_object shm(boost::interprocess::open_or_create, "MySharedMemory", read_write);

    //Set size
    shm.truncate(50);

    //Map the whole shared memory in this process
    mapped_region region(shm, read_write);


    std::memcpy(region.get_address(), val->c_str(), region.get_size());

    int stop;
    std::cin >> stop;
}

void SelectStage(int option,std::string val)
{
    switch (option)
    {
    case 1:
        CreateSharedMemory(&val);
        break;
    case 2:
        ReadMemory();
        break;
    case 3:
        EditSharedMemory(&val);
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    //
    // First argument - Run option:
    //  (1) Createing sheared memory
    //  (2) Read shared memory and dump
    //  (3) Editting shared memory
    // Secound argument:
    //      text value.
    // 
    std::string val = "This is sample";
    if (argc == 2)
    {
        SelectStage(atoi(argv[1]),val);
    }
    else if (argc == 3)
    {
        SelectStage(atoi(argv[1]), argv[2]);
    }
    return 0;
}