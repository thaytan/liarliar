/***************************************************************************
                          main_window.cpp  -  description
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
#include "main_window.h"

MainWindow::MainWindow() :
m_audioviewport(100, 50),
m_control_frame(m_audioviewport)
{
	// Set window title
	set_title("Liar Liar");
	set_default_size(640,480);

	// put the main vbox into the main window
	add(m_menu_vbox);

	m_menu_vbox.pack_start(m_mainmenu, Gtk::PACK_SHRINK);
	m_menu_vbox.pack_start(m_main_vbox);

	m_main_vbox.set_border_width(10);

	// This packs the boxes into the Window (a container).
	m_main_vbox.pack_start(m_audioviewport);
	m_main_vbox.pack_start(m_status_frame, Gtk::PACK_SHRINK);
	m_main_vbox.pack_start(m_control_frame, Gtk::PACK_SHRINK);

	// Connect to the cross-thread signal in AudioViewport class
	m_control_frame.signal_draw.connect(sigc::mem_fun(m_audioviewport,&AudioViewport::draw) );
	// load the plugins
	ploader = plugin_loader::Instance(&m_status_frame);
	// instantiate a new Thread object (start thread w/ thread->Start(bool) )
  analysis_thread = new Thread(ploader,&m_control_frame);
	m_control_frame.m_play_button.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_play_button_clicked));
	m_control_frame.m_stop_button.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_stop_button_clicked));
  // connect volume slider
	m_control_frame.m_vol_slider.signal_value_changed().connect(sigc::mem_fun(*analysis_thread,&Thread::on_volume_changed));

	// fill the plugin combo box
	m_control_frame.m_plugin_combo.set_popdown_strings(ploader->get_names());
	// connect the signal from the apply button
	m_control_frame.m_apply_button.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_plugin_selected));
  // use running state from the thread object
	m_audioviewport.running = &(analysis_thread->running);

	//setup menu entries
  setup_menu();

  // The final step is to display this newly created widget...
	show_all_children();
}

MainWindow::~MainWindow()
{ }

// End Application
void MainWindow::on_quit()
{
   hide();
}

void MainWindow::on_play_button_clicked()
{
	if (!analysis_thread->running)
	{
  	// create a non-joinable thread to process audio
		analysis_thread->Start(m_plugin_text,false);
	}
}

void MainWindow::on_stop_button_clicked()
{
	// stop thread
	analysis_thread->Stop();
}

void MainWindow::on_menu_about()
{
	Glib::ustring about_text = "\
Liar Liar v0.5\n\n\
Lead Developers:\n\
----------------\n\
Gene Ruebsamen <gene@erachampion.com>\n\
Quan Nguyen <friday_morning@yahoo.com>\n\n\
FFT Library by Laurent de Soras";

	Gtk::MessageDialog dialog(*this, about_text);
  dialog.run();
}

void MainWindow::setup_menu()
{
	// File menu:
  {
  	Gtk::Menu::MenuList& menulist = m_menu_file.items();

  	menulist.push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::OPEN,
			sigc::mem_fun(*analysis_thread, &Thread::on_file_open) ) );
		menulist.push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::QUIT,
    	sigc::mem_fun(*this, &MainWindow::on_quit) ) );
	}

  //Tools menu:
  {
  	Gtk::Menu::MenuList& menulist = m_menu_tools.items();

    menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Options",
			sigc::mem_fun(*analysis_thread, &Thread::on_options) ) );
	}

  //Help menu: (exercise stock items)
  {
  	Gtk::Menu::MenuList& menulist = m_menu_help.items();

    menulist.push_back( Gtk::Menu_Helpers::MenuElem("About", sigc::mem_fun(*this, &MainWindow::on_menu_about) ) );
  }

  //Add the menus to the MenuBar:

  m_mainmenu.items().push_back( Gtk::Menu_Helpers::MenuElem("_File", m_menu_file) );
  m_mainmenu.items().push_back( Gtk::Menu_Helpers::MenuElem("_Tools", m_menu_tools) );
  m_mainmenu.items().push_back( Gtk::Menu_Helpers::StockMenuElem(Gtk::Stock::HELP, m_menu_help) );

  Gtk::MenuItem* pMenuItem = &m_mainmenu.items().back();
  pMenuItem->set_right_justified();
}

void MainWindow::on_plugin_selected()
{
	vector<string> names = ploader->get_names();

  // stop any running thread
	analysis_thread->Stop();

	// get the name of the plugin the user selected
	m_plugin_text = m_control_frame.m_plugin_combo.get_entry()->get_text();
}

