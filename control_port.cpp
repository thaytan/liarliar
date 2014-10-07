/***************************************************************************
                          control_port.cpp  -  description
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
#include "control_port.h"

ControlPort::ControlPort(AudioViewport &audio) :
m_volume("volume"),
m_plugin("plugin"),
m_apply_button(Gtk::Stock::APPLY),
m_stop_button(Gtk::Stock::STOP),
m_vol_slider(0,100,0), // min, max, step
running(false)
{
  set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  set_label("Control");
	m_vol_slider.set_value(100);

  // packing
  add(m_hbox1);
  m_hbox1.set_border_width(5);
  m_hbox1.pack_start(m_play_button, Gtk::PACK_SHRINK, 3);   // play button
  m_hbox1.pack_start(m_stop_button, Gtk::PACK_SHRINK, 3);   // pause button
  m_hbox1.pack_start(m_vbox1);   // volume
  m_hbox1.pack_start(m_vseparator, Gtk::PACK_SHRINK, 3);   // vert line
  m_hbox1.pack_start(m_vbox2, Gtk::PACK_SHRINK, 3);   // plugins
  m_hbox1.pack_start(m_apply_button, Gtk::PACK_SHRINK, 3);   // apply button

  // buttons
  m_play_button.add_pixlabel("play.xpm","Play");
   
  // vbox1
  m_vbox1.pack_start(m_volume,Gtk::PACK_SHRINK);
  m_vbox1.pack_start(m_vol_slider);
   
  // vbox2
  m_vbox2.pack_start(m_plugin,Gtk::PACK_SHRINK);
  m_vbox2.pack_start(m_plugin_combo, Gtk::PACK_SHRINK);
   
  // plugin combo box setup (temp)
	// TODO:  actually do something with the plugins
  //std::list<Glib::ustring> listStrings;
  //listStrings.push_back("Simple");
  //listStrings.push_back("Micro Tremor");
  //listStrings.push_back("Pitch Change");
  //listStrings.push_back("Freq Analysis");
	

  //m_plugin_combo.set_popdown_strings(listStrings);

	audio_ptr = &audio;
	
  show_all_children();
}

ControlPort::~ControlPort()
{
}

