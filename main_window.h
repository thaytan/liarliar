/***************************************************************************
                          main_window.h  -  description
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
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/stock.h>
#include <gtkmm/entry.h>
#include <gtkmm/messagedialog.h>
#include "audio_viewport.h"
#include "control_port.h"
#include "status_port.h"
#include "plugin_loader.h"
#include "thread.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow();
  virtual ~MainWindow();

protected:
  // signal handlers
  virtual void on_quit();
	virtual void on_play_button_clicked();
  virtual void on_stop_button_clicked();
	virtual void on_menu_about();
	virtual void on_plugin_selected();
   
  //Member widgets:
  AudioViewport m_audioviewport;
	Thread *analysis_thread;
	plugin_loader *ploader;
  ControlPort m_control_frame;
  StatusPort m_status_frame;
	Gtk::MenuBar m_mainmenu;
  Gtk::VBox m_main_vbox, m_menu_vbox;
  Gtk::Menu m_menu_file, m_menu_tools, m_menu_help;

private:
	void setup_menu();	// helper function for setting up the menus
	string m_plugin_text;
};

#endif // MAIN_WINDOW_H
