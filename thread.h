/***************************************************************************
                          thread.h  -  Thread C++ wrapper
                             -------------------
    begin                : Wed Dec 24 2003
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

// Wrapper class around the Glib::Thread

#ifndef THREAD_H
#define THREAD_H

#include <glibmm/thread.h>
#include <glibmm/dispatcher.h>
#include <gtkmm/object.h>
#include <gtkmm/fileselection.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include <gst/gst.h>

#include "control_port.h"
#include "plugin_loader.h"
#include "plugin.h"

class MainWindow;

struct listener_struct
{
  unsigned int    bit_depth;    // audio bit depth (ie 8 or 16 bit)
  unsigned int		channels;			// # of audio channels
	unsigned int 		rate;					// sampling rate
  bool    data_signed;
  bool    not_listened_yet;
};

class Thread : public sigc::trackable
{
friend class MainWindow;
public:
  Thread(plugin_loader *, Gtk::Object *o=NULL);
	virtual ~Thread();
	Glib::Thread *Start(string name, bool joinable);

	void Stop();
protected:
  void EntryPoint();		// thread entry point
  virtual void Setup();   // thread init code
  virtual void Execute(); // main thread execution code
	virtual void Destroy();	// thread cleanup
	static void cb_manipulate(GstElement *element, GstBuffer *data, Thread* thread_obj_ptr);
	virtual void on_file_open();
	virtual void on_options();
	virtual void on_volume_changed();
private:
  inline void lock_thread();
  inline void unlock_thread();
  inline void update_drawing_vals(double data_val);
  inline void refresh_drawing(int audio_buf_size);

	sigc::connection plugin_connection;
	typedef sigc::signal1<void,vector<double> *> type_audio_signal;
	type_audio_signal m_audio_signal;
	vector<double> *m_audio_buffer_vec;		// local audio buffer to be sent to plugin
																				// for processing
	plugin *current_plugin_ptr;
	Gtk::Object *object;
	plugin_loader *ploader;
	bool running;  // controls the termination of the thread
	bool mic_input;				// microphone or file input
	string m_plugin_name;
  listener_struct audio_props;
	vector<double>::iterator draw_iter_start;
	vector<double>::iterator draw_iter_end;
	GstElement *pipeline;
	std::string filename;
	std::string audio_sink;
};

#endif
