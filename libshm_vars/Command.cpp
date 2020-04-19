// ============================================================================
// Command.cpp
// 
// This class is a model for a single serial command as used by the climbing
// wall. 
// ============================================================================
#include "Command.h"
#include <sstream>
#include <cmath>      //provides abs();

// for unit testing with main 
#include <iostream>

// utilities predeclairation
unsigned int hex_string_to_integer(std::string input);
std::string integer_to_hex_string(unsigned int input);

//Initialize static constants.
const std::string Command::reference_speed = "8e008e"; // speed: zero feet per min.
const std::string Command::reference_incline ="920092"; // incline: 15 degrees slab

// for speed or incline, add this value to both high and low data bytes to 
// calculate a new relative value.  Please note, there is no carry or borrow 
// between high and low databytes if you overflow/underflow the value.
const unsigned char Command::speed_one_fpm      = 0x03; 
const unsigned char Command::incline_one_degree = 0x02; 
// this implys the following maximum resolution:
// speed: 1, 1.3, 1.6, 2
// incline: 1, 1.5, 2
// 1/3s for speed is exciting in case I get into any algorithmic tuning.


Command::Command (void)
{
  clear();
}


Command::Command (std::string command, std::string description)
{
  set_to_hex_string (command, description);
}


void Command::clear()
{
  _valid = false;
  _data.clear(); 
  _description = "invalid (cleared)";
}


void Command::set_to_incline(int degrees)
{
  if ( ( degrees >= 15 ) || ( degrees <= -90 ) )
  { // function arguement is out of range... warn stdout and clear this Command.
    std::cout <<"WARNNING: Command::set_to_incline called with out of range input."
              << std::endl;
    clear();
    return;
  }

  // set up the description string
  std::stringstream stream;
  std::string incline_string; 
  stream << std::abs(degrees);
  stream >> incline_string;

  std::string description = "incline: " ; 
  // only a single diget wide, so add a space so things line up nicely.
  if (std::abs(degrees) < 10)  
  {  description += " ";
  }
  description += incline_string;
  if (degrees == 0)
  { description += " degrees vertical " ; 
  }
  else if (degrees > 0)
  { description += " degrees slab    " ; 
  } 
  else
  { description += " degrees overhang" ; 
  } 

  //set data to the value: 15 degrees slab, set the final description.
  set_to_hex_string((reference_incline), description);
  
  
  //perform arithmatic on the data bytes until incline is set properly.
  for (int counter = 15; counter != degrees; --counter)
  {
    _data[1] += incline_one_degree;
    _data[2] += incline_one_degree;
  }

  std::cout << "data is: " << get_printable() << std::endl;

  
}


void Command::set_to_speed(int feet_per_minute)
// set the speed and description of the Command to given user input.
{
  if ( ( feet_per_minute > 50 ) || ( feet_per_minute < 0 ) )
  { // function arguement is out of range... warn stdout and clear this Command.
    std::cout << "WARNNING: Command::set_to_speed called with out of range input."
              << std::endl;
    clear();
    return;
  }

  // set up the description string
  std::string fpm_string; 
  std::stringstream stream;
  stream << feet_per_minute;
  stream >> fpm_string;

  std::string description = "speed:   " ; 
  // only a single diget wide, so add a space so things line up nicely.
  if (std::abs(feet_per_minute) < 10)  
  {  description += " ";
  }
  description += fpm_string; 
  description += " feet per minute " ; 
  
 
  if (feet_per_minute == 0 )
  {
    // set the command to halt and use that as speed zero.
    set_to_hex_string("8989", description);
  }
  else 
  { 
    //set data to the value: zero feet per minute, set the final description.
    set_to_hex_string((reference_speed), description);

    //perform arithmatic on the data bytes until speed is set properly.
    for (int counter = 0; counter < feet_per_minute; ++counter)
    {
      _data[1] += speed_one_fpm;
      _data[2] += speed_one_fpm;
    }
  }
}


void Command::set_to_hex_string(std::string command, std::string description)
{
  clear();

  for (unsigned int counter = 0; counter < (command.size()); counter +=2)
  {
    std::string tmp = command.substr (counter, 2);
    _data.push_back( hex_string_to_integer (tmp));
  }
  
  _description = description;

  if (_data.size() >= 1) 
  {
    _valid = true;
  } 
}


std::string Command::get_printable (void)
{
  std::string result = "";
  result = _description;
  result += " ";
  if (_valid)
  {
    result += "+";
  }
  else 
  {
    result += "-";
  }
  result += " ";
  for (int counter = 0; counter < _data.size(); ++counter)
  {
    std::stringstream stream;
  
    result += "(";
    result += integer_to_hex_string(_data[counter]);
    result += ") ";
  } 
  
  return result;
}


unsigned int hex_string_to_integer(std::string input)
{ 
  std::stringstream stream;
  int out;
  stream << std::hex << input;
  stream >> out;
  return out;
}


std::string integer_to_hex_string(unsigned int input)
{ 
  std::stringstream stream;
  std::string result; 
  stream << std::hex << input;
  stream >> std::hex >> result;
  return result;
}


