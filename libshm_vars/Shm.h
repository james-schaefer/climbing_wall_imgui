// Shm.h
// This class implements a template class that shares a single variable across 
// mutiple processes and protects access to that variable via semaphores. Both 
// are stored in shared memory.  Each process that accesses the variable 
// does so through an interface which enforces thread-safe access. 
//
// The locking model is a classic reader writer lock.  Concurrent reads are
// allowed, when a write happens, readers are starved until the write 
// completes. 

#ifndef Shm_h__
#define Shm_h__

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>          // provies sleep
#include <iostream>          // provides cout, endl
#include <stdlib.h>          // provides exit.
#include <errno.h>

#include <semaphore.h>

namespace SHM
{

  template<typename T>
  class Shm
  {
    public:
      // name must be unique across the entire system. 
      static void* alloc_new(std::string unique_name);
      static void* alloc_ex(std::string unique_name);
  
    private:
      //The value we're working so hard to encapsulate across processes.
      volatile T value; 
  
      int read_count;
      int write_count;
  
      sem_t read_sem;
      sem_t write_sem;
      sem_t try_read_sem;
      sem_t resource_sem;
  
      int error_code;
  
  
    public:
      Shm();
      void init_data(T initial_value);
  
      //bool destroy_existing_shm();
  
      T get();
      void set(T new_value);
  };
  
  template<typename T>
  Shm<T>::Shm()
  {
  }
  
  template<typename T>
  void Shm<T>::init_data(T initial_value)
  {
    value = initial_value;
    read_count = 0;
    write_count = 0;
  
    sem_init(&read_sem, 0, 1);
    sem_init(&write_sem, 0, 1);
    sem_init(&try_read_sem, 0, 1);
    sem_init(&resource_sem, 0, 1);
    error_code = 0;
  }
  
  template<typename T>
  void* Shm<T>::alloc_new(std::string unique_name)
  { // create the shared memory associated with this objct by creating a new
    // /dev/shm entry.
    int fd = shm_open(unique_name.c_str(), O_CREAT|O_RDWR, 0);
    if (fd < 0)
    {
      std::cout << "shm_open has failed.  abort." << std::endl;
      exit(-1);
    }
  
    // resize shared memory to fit this object.
    ftruncate(fd, sizeof(Shm<T>));
  
    // return the memory mapped shared memory.
    return static_cast<void*>( mmap(NULL, sizeof(struct Shm<T>),
                               PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
  }
  
  template<typename T>
  void* Shm<T>::alloc_ex(std::string unique_name)
  { // create the shared memory associated with this object from exisiting 
    // /dev/shm entry.
    int fd = shm_open(unique_name.c_str(), O_RDWR, 0);
    if (fd < 0)
    {
      std::cout << "shm_open has failed.  abort." << std::endl;
      exit(-1);
    }
  
    // return the memory mapped shared memory.
    return static_cast<void*>( mmap(NULL, sizeof(struct Shm<T>),
                               PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
  }
  
  // template<typename T>
  // bool Shm<T>::destroy_existing_shm()
  // {
  //     shm_unlink(path().c_str());
  // }
  
  template<typename T>
  T Shm<T>::get()
  {
    T ret_val = 0;
    sem_wait(&try_read_sem);
    {
      sem_wait(&read_sem);
      {
        ++read_count;
        if(read_count == 1) // we are the *first* reader in...
        {
          sem_wait(&resource_sem);
        }
      sem_post(&read_sem);
      }
    sem_post(&try_read_sem);
    }
  
    // resource semaphore is held...
    {
      ret_val = value;
    }
  
    sem_wait(&read_sem);
    {
      --read_count;
      if(read_count == 0) // all readers are done...
      {
        sem_post(&resource_sem);
      }
    sem_post(&read_sem);
    }
  
    return ret_val;
  }
  
  template<typename T>
  void Shm<T>::set(T new_value)
  {
    sem_wait(&write_sem);
    {
      ++write_count;
      if(write_count == 1) // we are the first writer
      {
        sem_wait(&try_read_sem);
      }
    sem_post(&write_sem);
    }
  
    // try_read_sem is held - to lock out new readers.
    sem_wait(&resource_sem);
      value = new_value;
    sem_post(&resource_sem);
  
    sem_wait(&write_sem);
    {
      --write_count;
      if(write_count == 0) // I am the last writer.
      {
        sem_post(&try_read_sem);
      }
    sem_post(&write_sem);
    }
  }
}
#endif
