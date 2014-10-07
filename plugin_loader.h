/***************************************************************************
                          plugin_loader.h  -  description
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

#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <cassert>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include "status_port.h"
#include "plugin.h"

using std::string;
using std::vector;
using std::less;
using std::list;
using std::map;
using std::cerr;
using std::cout;
using std::endl;

class plugin_loader
{
public:
	static plugin_loader *Instance(StatusPort *);		// allow only one instance of this class
	plugin* execute(string name);
	vector<string> get_names();
protected:
	plugin_loader();
	~plugin_loader();
private:
	list<void *> dl_list;		// list to hold handles for dynamic libs
	list<void *>::iterator dl_list_itr;
	vector<string> plugin_names; // vector of plugin names used for menus
	list<plugin *> plugin_list;		// list of plugin objects we create
	list<plugin *>::iterator plugin_list_itr;
	map<string,maker_t *,less<string> >::iterator fitr;
	static plugin_loader *_instance;
	StatusPort *status_ptr;
};

// the types of the class factories
typedef plugin *maker_t();   // typedef to make it easier to setup our factory
//typedef plugin maker_t;   // typedef to make it easier to setup our factory
// size of buffer for reading in directory entries
const unsigned int BUF_SIZE = 1024;

#endif
