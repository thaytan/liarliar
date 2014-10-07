/***************************************************************************
                          audio_viewport.cpp  -  description
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
#include "audio_viewport.h"

AudioViewport::AudioViewport(int x_size, int y_size) :
DrawingArea(),
running(false)
{
  // instantiate mutex
	draw_mutex = new Glib::Mutex();

	// shared vector<double>
	draw_mutex->lock();
	drawing_vals = new vector<double>;
	drawing_vals->push_back(0.0);	// init
  draw_mutex->unlock();

  set_size_request(x_size, y_size);

	#ifndef NDEBUG
  std::cout << x_size << ", " << y_size << std::endl;
	#endif
  signal_expose_event().connect(
      sigc::mem_fun(*this, &AudioViewport::on_expose_event));
  signal_configure_event().connect(
      sigc::mem_fun(*this, &AudioViewport::on_configure_event));

  //TODO: Why do we store m_width and m_height? murrayc

  // This is where we connect the slot to the Glib::signal_timeout()
 // SigC::Connection conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &AudioViewport::on_timeout), timeout_value);
}

//Expose_event method.
bool AudioViewport::on_expose_event(GdkEventExpose* event)
{
	if (*running) {
		return true;   // gene - TODO: should disable expose event completely when in running state
	}

  Glib::RefPtr<Gdk::Window> win = get_window();
  Glib::RefPtr<Gdk::GC> gc = get_style()->get_black_gc();

	#ifndef NDEBUG
  std::cout << "start expose_event" << std::endl;
  #endif

  get_window()->draw_drawable(
      //get_style()->get_fg_gc(get_state()),
      get_style()->get_black_gc(),
      m_refPixmap_Scribble,
      // Only copy the area that was exposed:
      event->area.x, event->area.y,
      event->area.x, event->area.y,
      event->area.width, event->area.height);

  scribble_draw_brush(get_allocation().get_width(),get_allocation().get_height());

	#ifndef NDEBUG
  std::cout << "end expose_event" << std::endl;
	#endif
  // return true to stop any further event handlers being called
  // to draw this area
  return false;
}

/* Draw a rectangle on the screen.
 */
void AudioViewport::scribble_draw_brush(int x, int y)
{
	draw_mutex->lock();
	Glib::RefPtr<Gdk::Window> win = get_window();
  Glib::RefPtr<Gdk::GC> gc = get_style()->get_black_gc();

  // Paint to the pixmap, where we store our state.
  int movey = y/2;

  // Draw a straight line through the middle of the audio viewport
  m_refPixmap_Scribble->draw_line(gc, 0, y/2, x, y/2);

  for (unsigned int i=0; i<drawing_vals->size()-1; ++i)
  {
		// drawing lines makes for smoother gfx, but somehow I got the idea it takes more CPU
		// than drawing points, anyone care to test this?  - Gene
		m_refPixmap_Scribble->draw_point(gc, (i*x)/drawing_vals->size(), int(movey-((y/2)*(*drawing_vals)[i])));
     //m_refPixmap_Scribble->draw_line(gc, (i*x)/drawing_vals->size(), int(movey-((y/2)*(*drawing_vals)[i])),
     //   ((i+1)*x)/drawing_vals->size(), int(movey-((y/2)*(*drawing_vals)[i+1])));
  }
	draw_mutex->unlock();
   // Now invalidate the affected region of the drawing area.
   //get_window()->invalidate_rect(update_rect, false);
}

bool AudioViewport::on_configure_event(GdkEventConfigure*)
{
	#ifndef NDEBUG
  std::cout << "Start on_configure_event" << std::endl;
	#endif

  m_refPixmap_Scribble = Gdk::Pixmap::create(
      get_window(),
      get_allocation().get_width(),
      get_allocation().get_height(),
      -1);

//initialize area to white

  m_refPixmap_Scribble->draw_rectangle(
      get_style()->get_white_gc(),
      true,
      0, 0,
      get_allocation().get_width(),
      get_allocation().get_height());

  scribble_draw_brush(get_allocation().get_width(),get_allocation().get_height());

	#ifndef NDEBUG
  std::cout << "end on_configure_event" << std::endl;
	#endif

  /* We've handled the configure event, no need for further processing. */
  return true;
}

void AudioViewport::draw(void)
{
   // clear buffer
   m_refPixmap_Scribble->draw_rectangle(
      get_style()->get_white_gc(),
      true,
      0, 0,
      get_allocation().get_width(),
      get_allocation().get_height());

   // draw to buffer
   scribble_draw_brush(get_allocation().get_width(),get_allocation().get_height());

   // paint to window
   get_window()->draw_drawable(
      //get_style()->get_fg_gc(get_state()),
      get_style()->get_black_gc(),
      m_refPixmap_Scribble,
      // repaint entire buffer
      0, 0,
      0, 0,
      -1, -1);
}

