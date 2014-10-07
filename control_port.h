/***************************************************************************
                          control_port.h  -  description
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
#ifndef CONTROL_PORT_H
#define CONTROL_PORT_H

#include <gtkmm/button.h>
#include <gtkmm/combo.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/scale.h>
#include <gtkmm/stock.h>
#include <glibmm/thread.h>
#include <sigc++/class_slot.h>
#include "utils.h"
#include "audio_viewport.h"
#include "status_port.h"

class MainWindow;
class Thread;

class ControlPort : public Gtk::Frame
{
friend class Thread;
friend class MainWindow;
public:
  ControlPort(AudioViewport &);
	~ControlPort();

protected:
	Gtk::Button m_play_button, m_stop_button, m_apply_button;
  Gtk::Combo m_plugin_combo;
  Gtk::Label m_volume, m_plugin;
  Gtk::VBox m_vbox1, m_vbox2, m_vbox3;
  Gtk::HBox m_hbox1;
  Gtk::HScale m_vol_slider;
  Gtk::VSeparator m_vseparator;
	Glib::Dispatcher signal_draw;
	AudioViewport *audio_ptr;

private:
	bool running;
};

#endif //CONTROL_PORT_H
