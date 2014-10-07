/***************************************************************************
                          status_port.cpp  -  description
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
#include "status_port.h"

StatusPort::StatusPort() :
m_analysis("Analysis:"),
m_confidence("Stress:")
{
   set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
   set_label("Status");

   // create size group for labels
   m_refSizeGroup = Gtk::SizeGroup::create(Gtk::SIZE_GROUP_HORIZONTAL);
   m_refSizeGroup->add_widget(m_analysis);
   m_refSizeGroup->add_widget(m_confidence);

   m_confidence_bar.set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);

	//Create the first TextBuffer:
  m_refTextBuffer1 = Gtk::TextBuffer::create();

   // textview attributes
   m_analysis_text.set_cursor_visible(false);
   m_analysis_text.set_editable(false);
  m_refTextBuffer1->set_text("no status information");
  m_analysis_text.set_buffer(m_refTextBuffer1);

	// Connect to the cross-thread signal for refreshing the text & confidence
	signal_refresh.connect(sigc::mem_fun(*this,&StatusPort::on_refresh) );

   // packing
   add(m_vbox1);
   m_vbox1.set_border_width(5);
   m_vbox1.pack_start(m_hbox1,Gtk::PACK_EXPAND_WIDGET,3);
   m_vbox1.pack_start(m_hbox2,Gtk::PACK_EXPAND_WIDGET,3);

   m_hbox1.pack_start(m_analysis, Gtk::PACK_SHRINK);
   m_hbox1.pack_start(m_analysis_text);
   m_hbox2.pack_start(m_confidence, Gtk::PACK_SHRINK);
   m_hbox2.pack_start(m_confidence_bar);

   m_analysis.set_justify(Gtk::JUSTIFY_LEFT);
   m_confidence.set_justify(Gtk::JUSTIFY_LEFT);

   set_stress(0.0);

	// initialize the text buffer
  m_refTextBuffer1 = Gtk::TextBuffer::create();

   show_all_children();
}

float StatusPort::get_stress(void) const
{
   return m_confidence_bar.get_fraction()*100.0;
}

void StatusPort::set_stress(float pct)
{
 	lock_.lock();
  if (pct >= 0.0 && pct <= 100.0)
  {
		m_pct = pct;
  }
	lock_.unlock();
}

Glib::ustring StatusPort::get_analysis(void) const
{
   return m_refTextBuffer1->get_text(false);
}

void StatusPort::set_analysis(Glib::ustring text)
{
	lock_.lock();
	m_text = text;
	lock_.unlock();
}

void StatusPort::on_refresh()
{
	//Set the new value for confidence bar
  string value;
  string_fmt(value,m_pct);
  value = value + "%";
  m_confidence_bar.set_fraction(m_pct/100.0);
  m_confidence_bar.set_text(value);

	// set new value for text output
  m_refTextBuffer1->set_text(m_text);
  m_analysis_text.set_buffer(m_refTextBuffer1);
}
