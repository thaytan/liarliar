/***************************************************************************
                          audio_viewport.h  -  description
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

#ifndef AUDIO_VIEWPORT_H
#define AUDIO_VIEWPORT_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <glibmm/thread.h>
#include <vector>
#include <iostream>
#include <fstream>

class ControlPort;             // forward declaration
class ProcessThread;

using std::vector;
using std::ifstream;

//Custom drawing area with modified expose_event.
class AudioViewport : public Gtk::DrawingArea
{
friend class MainWindow;
friend class ControlPort;
friend class Thread;
public:
  AudioViewport(int x_size = 0, int y_size = 0);
 
protected: 
   // signal handlers
  bool on_expose_event(GdkEventExpose* event);
  bool on_configure_event(GdkEventConfigure* event);
	virtual void draw(void); 		// thread drawing function
   
  void scribble_draw_brush(int x, int y);

  Glib::RefPtr<Gdk::Pixmap> m_refPixmap_Scribble;

private:
  // temporary variables
  vector<double> *drawing_vals;
	Glib::Mutex *draw_mutex;   // mutex for synchronizing cross thread drawing buffer
	bool *running;
};

#endif //AUDIO_VIEWPORT_H
