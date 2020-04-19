//=============================================================================
// shm_variables.cpp
// 
// A collection of variables in SHared Memory (SHM).  These variables
// can be used across processes running on this system. This acts as a 
// central messaging hub for all running processes associated with the 
// climbing wall.   
//=============================================================================
#include "Shm_vars.h"
using std::string;

namespace SHM
{

  Shm<uint64_t>*     timestamp;  
  Shm<uint64_t>*     session_start_time;  
  Shm<float>*        session_duration;

  Shm<uint64_t>*     set_start_time;
  Shm<float>*        set_duration;
  Shm<int>*          set_count;

  Shm<bool>*         curr_halt;    
  Shm<int>*          curr_speed;   
  Shm<int>*          curr_incline; 

  Shm<bool>*         req_halt;
  Shm<int>*          req_speed;
  Shm<int>*          req_incline;

  Shm<int>*          hr;
  Shm<int>*          hr_avg;
  Shm<int>*          hr_avg_dur;
  Shm<bool>*         hr_driven;
  Shm<int>*          hr_target;

  Shm<double>*       elevation;
  Shm<bool>*         log_active;

  void allocate_new_shm (void)
  {
    timestamp = new (Shm<uint64_t>::alloc_new("timestamp")) Shm<uint64_t>();
    session_start_time = new (Shm<uint64_t>::alloc_new("session_start_time")) Shm<uint64_t>();
    session_duration = new (Shm<float>::alloc_new("session_duration")) Shm<float>();

    set_start_time = new (Shm<uint64_t>::alloc_new("set_start_time")) Shm<uint64_t>();
    set_duration = new (Shm<float>::alloc_new("set_duration")) Shm<float>();
    set_count = new (Shm<int>::alloc_new("set_count")) Shm<int>();

    curr_halt = new (Shm<bool>::alloc_new("curr_halt")) Shm<bool>();
    curr_speed= new (Shm<int>::alloc_new("curr_speed")) Shm<int>();
    curr_incline= new (Shm<int>::alloc_new("curr_incline")) Shm<int>();

    req_halt = new (Shm<bool>::alloc_new("req_halt")) Shm<bool>();
    req_speed= new (Shm<int>::alloc_new("req_speed")) Shm<int>();
    req_incline= new (Shm<int>::alloc_new("req_incline")) Shm<int>();

    hr= new (Shm<int>::alloc_new("heartrate")) Shm<int>();
    hr_avg= new (Shm<int>::alloc_new("heartrate_avg")) Shm<int>();
    hr_avg_dur= new (Shm<int>::alloc_new("heartrate_avg_duration")) Shm<int>();
    hr_driven= new (Shm<bool>::alloc_new("heartrate_driven")) Shm<bool>();
    hr_target= new (Shm<int>::alloc_new("heartrate_target")) Shm<int>();

    elevation= new (Shm<double>::alloc_new("elevation")) Shm<double>();
    log_active = new (Shm<bool>::alloc_new("log_active")) Shm<bool>();

    timestamp->init_data(0);
    session_start_time->init_data(0);
    session_duration->init_data(0.0);

    set_start_time->init_data(0);
    set_duration->init_data(0.0);
    set_count->init_data(0);

    curr_halt->init_data(true);
    curr_speed->init_data(0);
    curr_incline->init_data(0);

    req_halt->init_data(true); 
    req_speed->init_data(0);
    req_incline->init_data(0);

    hr->init_data(0);
    hr_avg->init_data(0);
    hr_avg_dur->init_data(0);
    hr_driven->init_data(false);
    hr_target->init_data(0);

    elevation->init_data(0);
    log_active->init_data(false);


//=============================================================================

  }

  void connect_existing_shm (void)
  {
    timestamp  = new (Shm<uint64_t>::alloc_ex("timestamp"))Shm<uint64_t>();
    session_start_time = new (Shm<uint64_t>::alloc_ex("session_start_time"))Shm<uint64_t>();
    session_duration = new (Shm<float>::alloc_ex("session_duration"))Shm<float>();

    set_start_time = new (Shm<uint64_t>::alloc_ex("set_start_time"))Shm<uint64_t>();
    set_duration = new (Shm<float>::alloc_ex("set_duration"))Shm<float>();
    set_count = new (Shm<int>::alloc_ex("set_count")) Shm<int>();

    curr_halt  = new (Shm<bool>::alloc_ex("curr_halt")) Shm<bool>();
    curr_speed = new (Shm<int>::alloc_ex("curr_speed")) Shm<int>();
    curr_incline = new (Shm<int>::alloc_ex("curr_incline")) Shm<int>();

    req_halt = new (Shm<bool>::alloc_ex("req_halt")) Shm<bool>();
    req_speed = new (Shm<int>::alloc_ex("req_speed")) Shm<int>(); 
    req_incline = new (Shm<int>::alloc_ex("req_incline")) Shm<int>(); 

    hr = new (Shm<int>::alloc_ex("heartrate")) Shm<int>(); 
    hr_avg = new (Shm<int>::alloc_ex("heartrate_avg")) Shm<int>(); 
    hr_avg_dur = new (Shm<int>::alloc_ex("heartrate_avg_duration")) Shm<int>(); 
    hr_driven = new (Shm<bool>::alloc_ex("heartrate_driven")) Shm<bool>(); 
    hr_target = new (Shm<int>::alloc_ex("heartrate_target")) Shm<int>(); 

    elevation = new (Shm<double>::alloc_ex("elevation")) Shm<double>(); 
    log_active = new (Shm<bool>::alloc_ex("log_active")) Shm<bool>(); 
  }
};
