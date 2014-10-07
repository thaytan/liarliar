/***************************************************************************
                          main.cpp  -  description
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
#include <gtkmm/main.h>
#include <iostream>
#include <glibmm/thread.h>
#include <gst/gst.h>
#include "main_window.h"  

using std::cout;
using std::cerr;
using std::endl;

int main (int argc, char *argv[])
{
	// initialize the Glib::Thread subsystem
	if(!Glib::thread_supported())
		Glib::thread_init();

	gst_init(&argc,&argv);
	
	Gtk::Main kit(argc, argv);
	MainWindow vsa_gui;

	Gtk::Main::run(vsa_gui); //Shows the window and returns when it is closed.

	return 0;

}
