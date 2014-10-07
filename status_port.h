/***************************************************************************
                          status_port.h  -  description
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
#ifndef STATUS_PORT_H
#define STATUS_PORT_H

#include <glibmm/ustring.h>
#include <string>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/textview.h>
#include <gtkmm/box.h>
#include <gtkmm/sizegroup.h>
#include "utils.h"

class ControlPort;		// forward declaration
class Thread;

using std::string;

class StatusPort : public Gtk::Frame
{
friend class ControlPort;
friend class Thread;
public:
  StatusPort();
  // dummy methods
  void set_analysis(Glib::ustring text);
  Glib::ustring get_analysis(void) const;
  void set_stress(float pct);
  float get_stress(void) const;
	Glib::Dispatcher signal_refresh;

protected:
  Gtk::Label m_analysis, m_confidence;
  Gtk::TextView m_analysis_text;
  Gtk::ProgressBar m_confidence_bar;
  Gtk::VBox m_vbox1;
  Gtk::HBox m_hbox1, m_hbox2;
  Glib::RefPtr<Gtk::SizeGroup> m_refSizeGroup;
	Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
	void on_refresh();		// used by analysis thread to refresh gui


private:
	Glib::ustring m_text;
	float m_pct;
	Glib::Mutex lock_;
};

#endif //STATUS_PORT_H
