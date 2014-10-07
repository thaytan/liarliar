/***************************************************************************
                          plugin.cpp  -  description
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

#include "plugin.h"

plugin::plugin()
{ }

plugin::~plugin()
{ }

float plugin::get_stress() const
{
	return status_ptr->get_stress();
}

void plugin::set_stress(float pct) 
{
	status_ptr->set_stress(pct);
	status_ptr->signal_refresh();
}

void plugin::set_analysis_text(Glib::ustring text)
{
	status_ptr->set_analysis(text);
	status_ptr->signal_refresh();
}

Glib::ustring plugin::get_analysis_text() const
{
	return status_ptr->get_analysis();
}
   
Glib::ustring plugin::get_plugin_description() const
{
	return description;
}

Glib::ustring plugin::get_plugin_name() const
{
	return name;
}
