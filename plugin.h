/***************************************************************************
                          plugin.h  -  Plugin API for shared libraries
                             -------------------
    begin                : Fri Dec 19 2003
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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <sigc++/sigc++.h>
#include <glibmm/ustring.h>
#include <string>
#include <vector>
#include <map>
#include "status_port.h"
#include "FFTReal.h"		// Laurent de Soras DSP library

class plugin_loader;	// forward declaration
class Thread;

using std::string;
using std::vector;
using std::less;
using std::map;

// virtual base plugin class for the plugin architecture
class plugin : public sigc::trackable
{
friend class plugin_loader;
friend class Thread;
public:
	plugin();
	virtual ~plugin();
	void set_stress(float pct);
	float get_stress() const;
	void set_analysis_text(Glib::ustring text);
	Glib::ustring get_analysis_text() const;
	Glib::ustring get_plugin_description() const;
	Glib::ustring get_plugin_name() const;
protected:
	Glib::ustring description;
	Glib::ustring name;
	virtual void on_activate(const vector<double> *, unsigned int rate) = 0;      // signal handler
private:
	StatusPort *status_ptr;    // warning: statusport object in another thread
};

// the types of the class factories
typedef plugin *maker_t();   // typedef to make it easier to setup our factory
extern map<string,maker_t *,less<string> > factory;	// the global factory

#endif // PLUGIN_H
