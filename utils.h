/***************************************************************************
                          utils.h  -  description
                             -------------------
    begin                : Mon Dec 22 2003
    copyright            : (C) 2003 by Gene Ruebsamen
    email                : gene@erachampion.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef UTILS_H
#define UTILS_H
                   
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using std::ostringstream;

template <class T>
void string_fmt(string & val, const T & t)
{
 ostringstream oss; // create a stream
 oss << t; // insert value to stream 
 val=oss.str(); // extract string and copy 
}

template <class out_type, class in_value>
out_type cast_stream(const in_value & t)
{
 stringstream ss;
 ss << t; // first insert value to stream
 out_type result; // value will be converted to out_type
 ss >> result; // write value to result
 return result;
}

#endif
