// ============================================================================
// Command.h
//
// This class is a model for a single serial command as sent to or recieved
// from the climbing wall.
// ============================================================================
#pragma once
#include <vector>
#include <string>

class Command
{
  friend class kiosk_sim;      // the kiosk_sim can access our command_data

  public:
  // constructors
  Command (void);
  Command (std::string command, std::string description);

  void set_to_speed(int feet_per_minute);
  void set_to_incline(int degrees); 

  void set_to_hex_string(std::string command, std::string description);

  void set_to_halt(void){ set_to_hex_string("8989", "halt command");}
  void set_to_request_status(void){ set_to_hex_string("9595","request status");}
    //we often use 9595 to request status, but sometimes 5B5B is used too...
    //this implementation sticks with 9595 for now.

  void clear();
  bool is_valid (void) {return _valid;}

  std::string get_printable (void);
  std::string get_description(void) {return _description;}

  private: 

  bool _valid; // only set to true when we can confirm we've been
               // set to some potentially valid command.  Specifically, we are
               // not empty/cleared.

  std::vector<unsigned char> _data;
  std::string _description; //meant to be human readable to non-technical
                            //users.  for example: "speed 20 feet per minute"
                            //                     "incline 5 degrees overhang"

  // both speed and incline are referenced to values at the begining of their
  // valid ranges.  

  static const std::string reference_speed;        // speed: zero feet per min.
  static const std::string reference_incline;      // incline: 15 degrees slab.

  static const unsigned char speed_one_fpm;      // add value to high & low data
  static const unsigned char incline_one_degree; // bytes to calculate new vlaue
  void unit_test ();
  
};
