//=============================================================================
// shm_variables.h 
// 
// A collection of variables in SHared Memory (SHM).  These variables
// can be used across processes running on this system. This acts as a 
// central messaging hub for all running processes associated with the 
// climbing wall.   
//=============================================================================
#ifndef Shmvars_h__
#define Shmvars_h__
#include "Shm.h"
#include <stdint.h>
#include <string>
#include <iostream>

using std::string;

namespace SHM
{
  // note: timestamps are always given in seconds, from epoc.  1970/01/01 
  // the current timestamp.  i.e: NOW. 
  extern SHM::Shm<uint64_t>*    timestamp;            
  extern SHM::Shm<uint64_t>*    session_start_time;   // our session start time.
  extern SHM::Shm<float>*       session_duration;     // total session duration
                                                      // in minutes.

  extern SHM::Shm<uint64_t>*    set_start_time;       // our set start time.  
  extern SHM::Shm<float>*       set_duration;         // total set duration 
  extern SHM::Shm<int>*         set_count;            // what set is this?
                                         


  // cur values represents the one most recently sent to the climbing wall.
  // Conceptually, only the process which manages serial communication to the 
  // climbing wall should be writing this value.  everyone else reads it.  
  // but this is not 
 
  extern SHM::Shm<bool>*     curr_halt;    // true if halt has been sent.
  extern SHM::Shm<int>*      curr_speed;   // speed in feet per minute.
  extern SHM::Shm<int>*      curr_incline; // incline in degrees.   

  // requested values represents the one most recently sent to the climbing 
  // wall.  Any  process may write this value.  however, this value may be 
  // overwriten before it is sent to the climbing wall.  
  extern SHM::Shm<bool>*     req_halt;    // true if we've sent halt to the wall
  extern SHM::Shm<int>*      req_speed;   // speed in feet per minute.
  extern SHM::Shm<int>*      req_incline; // incline in degrees. 

  // heartrate is the current heartrate of the climber.
  extern SHM::Shm<int>*      hr;          // in beats per minute.
  extern SHM::Shm<int>*      hr_avg;      // average across some measure of time
  extern SHM::Shm<int>*      hr_avg_dur;  // duration of the average.  ie 20 sec

  // for logging: 
  // the following several variables are interesting to log, therefor we 
  // share them.  

  //True when some aspect
  // of the climbing wall is being modified in response to heartrate data.  
  extern SHM::Shm<bool>*     hr_driven;   // is the hr_driven feature active?
  extern SHM::Shm<int>*      hr_target;   // if so, what's the target hr. 
  
  // it's fun, but not esspecially accurate, to track the total elevation 
  // climbed in this session (not accurate because the speed in fpm is not 
  // especially accurate)
  extern SHM::Shm<double>*      elevation;   // session elevation.

  extern SHM::Shm<bool>*     log_active;  // true if we should be logging.
  
// helper functions
  void allocate_new_shm (void);
  void connect_existing_shm (void);
};
#endif
