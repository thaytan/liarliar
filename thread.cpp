/***************************************************************************
                          thread.cpp  -  description
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

#include "thread.h"

// static member callback function for gstreamer
void Thread::cb_manipulate (GstElement *element, GstBuffer *buffer, Thread* thread_obj_ptr)
{
//  // =============================================================================
//
//  if (thread_obj_ptr->audio_props.not_listened_yet)
//  {
//    // ======= Get the capabilities of the element =======
//    // find the source pad
//    GstPad *srcpad;
//    srcpad = gst_element_get_pad (element, "src");
//
//    GstCaps *caps;
//    caps = gst_pad_get_caps (srcpad);
//
//		#ifndef NDEBUG
//    g_print ("inside listener:  pad name %s\n", gst_pad_get_name (srcpad));
//		#endif
//
//    while (caps) {
//    //print_caps(caps, "         ");
//      GList *props;
//      GstPropsEntry *prop;
//
//      props = caps->properties->properties;
//      // iterate through the list of properties to find the depth
//      while (props) {
//        prop = (GstPropsEntry*)(props->data);
//        props = g_list_next(props);
//
//        // If property is depth, then find out what the value is
//        if (strcmp(gst_props_entry_get_name (prop), "depth") == 0)
//        {
//          gst_props_entry_get_int (prop, (gint *)&thread_obj_ptr->audio_props.bit_depth);
//					#ifndef NDEBUG
//          g_print ("============== Audio bit depth is: %i\n", thread_obj_ptr->audio_props.bit_depth);
//					#endif
//        }
//        // Find out the number of channels
//        if (strcmp(gst_props_entry_get_name (prop), "channels") == 0)
//        {
//          gst_props_entry_get_int (prop, (gint *)&thread_obj_ptr->audio_props.channels);
//					#ifndef NDEBUG
//          g_print ("============== Number of channels is: %i\n", thread_obj_ptr->audio_props.channels);
//					#endif
//        }
//        // Obtain the sampling rate
//        if (strcmp(gst_props_entry_get_name (prop), "rate") == 0)
//        {
//          gst_props_entry_get_int (prop, (gint *)&thread_obj_ptr->audio_props.rate);
//					#ifndef NDEBUG
//          g_print ("============== Sampling Rate: %i\n", thread_obj_ptr->audio_props.rate);
//					#endif
//        }
//        // Find out if the data is signed or unsigned
//        if (strcmp(gst_props_entry_get_name (prop), "signed") == 0)
//        {
//          gst_props_entry_get_boolean (prop, (gint *)&thread_obj_ptr->audio_props.data_signed);
//					#ifndef NDEBUG
//          if (thread_obj_ptr->audio_props.data_signed)
//            g_print ("============== Data is signed \n");
//          else
//            g_print ("============== Data is unsigned \n");
//					#endif
//        }
//      }
//      caps = caps->next; GstCaps->
//    }
//    thread_obj_ptr->audio_props.not_listened_yet = false;
//  } // end if not_listened_yet
//    // ======== End of capabilities code ===============


// =============================================================================


	// calculate how many bytes are in each audio data element
	unsigned int num_bytes =  (thread_obj_ptr->audio_props.bit_depth/8)*thread_obj_ptr->audio_props.channels;
  // get the number of elements in buffer
	gsize buf_size = gst_buffer_get_size (buffer);
	unsigned int buf_element_size = buf_size / num_bytes;
	// determine the size of the local buffer (data to be sent to the plugin)
	unsigned int local_buf_size = ( ((thread_obj_ptr->audio_props.rate/2) / buf_element_size) * buf_element_size ) + ( ((thread_obj_ptr->audio_props.rate/2) % buf_element_size) ? buf_element_size : 0);
  // reserve the proper amount of space for the local audio buffer
	(*(thread_obj_ptr->m_audio_buffer_vec)).reserve( local_buf_size );
	GstMapInfo map;

  if (!gst_buffer_map (buffer, &map, GST_MAP_READ))
    return;

  //for (unsigned int i=0; i<buf_element_size; ++i)
	for (unsigned int i=0;i<buf_size;i+=num_bytes)
  {
    // Convert from two bytes of little endian to a decimal then to a double
    double data_val;

    if (thread_obj_ptr->audio_props.bit_depth == 16)   // 16 bit signed audio data
    {
      // Convert two bytes into one 16bit number & scale the range to be between -1 and 1
			// for the GUI drawing routines
		  gint16 *int_ptr = (gint16 *)(map.data+i);
		  data_val = (static_cast<double>(*int_ptr)) / 32768.0;    // scale to [-1,1]
    }
    else if (thread_obj_ptr->audio_props.bit_depth == 8)  // 8 bit unsigned audio data
		{
			// leave data in 8 bit format & convert to signed + scale range to be [-1,1]
      data_val = (static_cast<double>(*(map.data+i)) - 128.0) / 128.0;
		}
		else
		{
			g_print ("Unable to handle audio bit depth of: %d", thread_obj_ptr->audio_props.bit_depth);
			exit(1);
		}

    // write to shared memory (for GUI updates)
    thread_obj_ptr->update_drawing_vals(data_val);

		// write to local buffer so that raw audio data can pass to plugin
		(*(thread_obj_ptr->m_audio_buffer_vec)).push_back(data_val);
  }

  gst_buffer_unmap (buffer, &map);

  // collect enough raw data for 2hz resolution using FFT before calling plugin
	if (  (*(thread_obj_ptr->m_audio_buffer_vec)).size() >=  thread_obj_ptr->audio_props.rate/2 ) {
		// call plugin for processing & pass the local audio data
		thread_obj_ptr->current_plugin_ptr->on_activate(  thread_obj_ptr->m_audio_buffer_vec, thread_obj_ptr->audio_props.rate );
		// clear local buffer so we can gather new audio data on the next pass for the plugin
  	(*(thread_obj_ptr->m_audio_buffer_vec)).clear();
    Glib::Thread::yield();
	}

  // update the GUI (audio signal portion only)
	thread_obj_ptr->refresh_drawing( (buf_size)/((thread_obj_ptr->audio_props.bit_depth/8)*thread_obj_ptr->audio_props.channels));

	#ifndef NDEBUG
	if (!thread_obj_ptr->running)  {
		cout << "exiting thread.." << endl;
	}
	#endif
}

Thread::Thread(plugin_loader *p, Gtk::Object *obj) :
ploader(p),
object(obj),
mic_input(false),
running(false),
audio_sink("autoaudiosink")  // default audio is esd
{
	m_audio_buffer_vec = new vector<double>;
}

Thread::~Thread()
{
	delete m_audio_buffer_vec;
}

Glib::Thread *Thread::Start(string p_name, bool joinable)
{
	m_plugin_name = p_name;
	running = true;
	return (Glib::Thread::create(sigc::mem_fun(*this, &Thread::EntryPoint), joinable));
}

// stops the gstreamer audio pipeline
void Thread::Stop()
{
	running = false;

	// reset the local buffer
  m_audio_buffer_vec->clear();
}

void Thread::EntryPoint()
{
	Setup();
	Execute();
	Destroy();
}

void Thread::Setup()
{
  // Do any setup here
	// load the plugin
	current_plugin_ptr = ploader->execute(m_plugin_name); // execute & save current plugin
}

void Thread::Execute()
{
	// downcast object to ControlPort
	ControlPort *cobject = dynamic_cast<ControlPort*>(object);
	// to get the vector in the audio object
  //	*(cobject->audio_ptr->drawing_vals)[index]
	// to lock the mutex before writing/reading from vector
	//  (*(cobject->audio_ptr->draw_mutex)).lock();
	// to unlock the mutex after writing/reading from vector
	//  (*(cobject->audio_ptr->draw_mutex)).unlock();

  audio_props.not_listened_yet = true;

  // Open an MP3 and create a Gstreamer pipeline
  GstElement *filesrc, *decoder, *audiosink;
  GError *error = NULL;

	#ifndef NDEBUG
  std::cout << "Initializing Gstreamer\n";
	#endif

	string element_str;

	switch (mic_input)
	{
		case true:
			#ifndef NDEBUG
			cout << "INPUT: Microphone" << endl;
			#endif
			element_str = "osssrc ! identity name=listener ! " +  audio_sink;
			pipeline = (GstElement*)gst_parse_launch (element_str.c_str(), &error);
  		if (!pipeline) {
    		g_print ("Parse error: %s\n", error->message);
    		exit (1);
  		}
		break;
		case false:
			#ifndef NDEBUG
			cout << "FILE" << endl;
			#endif
			element_str = "filesrc name=my_filesrc ! mad ! identity name=listener ! " + audio_sink;
			pipeline = (GstElement*)gst_parse_launch (element_str.c_str(), &error);
  		if (!pipeline) {
    		g_print ("Parse error: %s\n", error->message);
    		exit (1);
  		}
			filesrc = gst_bin_get_by_name (GST_BIN (pipeline), "my_filesrc");
			g_object_set (G_OBJECT (filesrc), "location", filename.c_str(), NULL);
		break;
	}

  // Listener Code
  GstElement *listener;
  //listener = gst_element_factory_make ("identity",
	//			       "listener");
  listener = gst_bin_get_by_name (GST_BIN (pipeline), "listener");

  g_signal_connect (G_OBJECT (listener), "handoff", G_CALLBACK (&Thread::cb_manipulate), this);
  // End Listener

  // start playing
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

	#ifndef NDEBUG
  g_print ("Playback started\n");
	#endif

 /// while (gst_bin_iterate (GST_BIN (pipeline)) && running == true);

  // stop the pipeline
  gst_element_set_state (pipeline, GST_STATE_NULL);

	// we don't need a reference to these objects anymore
  // unreffing the pipeline unrefs the contained elements as well
 	gst_object_unref (GST_OBJECT (pipeline));

	Stop();		// terminate thread
}

void Thread::Destroy()
{
	#ifndef NDEBUG
	cout << "cleaning up thread.." << endl;
	#endif
}

void Thread::lock_thread()
{
  // Lock and clear
  ControlPort *cobject = dynamic_cast<ControlPort *>(object);

	(*(cobject->audio_ptr->draw_mutex)).lock();
}

void Thread::unlock_thread()
{
  // unlock
  ControlPort *cobject = dynamic_cast<ControlPort *>(object);

	(*(cobject->audio_ptr->draw_mutex)).unlock();
}

void Thread::update_drawing_vals(double data_val)
{
  // push the data_val onto the vector
  ControlPort *cobject;
  cobject = dynamic_cast<ControlPort *>(object);

	(*(cobject->audio_ptr->draw_mutex)).lock();	// lock mutex
  (*(cobject->audio_ptr->drawing_vals)).push_back(data_val);
	(*(cobject->audio_ptr->draw_mutex)).unlock(); // unlock mutex
}

void Thread::refresh_drawing(int audio_buf_size)
{
  // Assuming that the audio source creates a signal every 1/10th of a second,
  // we display 1 second of audio at a time.  We scroll over audio_buf_size pieces of data
  //vector::iterator draw_iter = (*(cobject->audio_ptr->drawing_vals)).begin();
  ControlPort *cobject = dynamic_cast<ControlPort *>(object);

	#ifndef NDEBUG
  g_print("Vector size = %d, Audio Buffer Size = %d\n", (*(cobject->audio_ptr->drawing_vals)).size(),
      audio_buf_size);
	#endif

  if ( (*(cobject->audio_ptr->drawing_vals)).size() > audio_props.rate)
  {
 		(*(cobject->audio_ptr->draw_mutex)).lock();  // lock mutex before write
    {
      draw_iter_start = (*(cobject->audio_ptr->drawing_vals)).begin();
      draw_iter_end = (*(cobject->audio_ptr->drawing_vals)).begin() += audio_buf_size;
      (*(cobject->audio_ptr->drawing_vals)).erase(draw_iter_start,draw_iter_end);  // erase shared mem
    }
 		(*(cobject->audio_ptr->draw_mutex)).unlock();  // unlock mutex after write

  	// Refresh the drawing
    cobject->signal_draw();
  }


}

void Thread::on_file_open()
{
  Gtk::FileSelection dialog("Please select a file");

  int result = dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
			#ifndef NDEBUG
      std::cout << "File selected: " << dialog.get_filename() << std::endl;
			#endif
			filename = dialog.get_filename();
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      break;
    }
    default:
    {
			#ifndef NDEBUG
      std::cout << "Unexpected button clicked." << std::endl;
			#endif
      break;
    }
  }
}

// Audio Prefs: allow user to select Audio Input Source & Output Source - <gene>
void Thread::on_options()
{
	Gtk::Dialog option_dialog("Set Audio I/O Options");
	Gtk::RadioButton::Group input, output;
	Gtk::RadioButton *rb_file = manage( new Gtk::RadioButton(input,"File Input"));
	Gtk::RadioButton *rb_mic = manage( new Gtk::RadioButton(input,"Microphone Input"));
	Gtk::RadioButton *rb_esd = manage( new Gtk::RadioButton(output,"ESounD"));
	Gtk::RadioButton *rb_oss = manage( new Gtk::RadioButton(output,"Open Sound System"));
	Gtk::RadioButton *rb_artsd = manage( new Gtk::RadioButton(output,"ArtsD"));


	// grab the vbox for the Dialog
	Gtk::VBox *vbox_area = option_dialog.get_vbox();
	option_dialog.set_has_separator(true);

	vbox_area->pack_start( *manage( new Gtk::Label("Audio Input:") ), Gtk::PACK_SHRINK );
	// pack the radio buttons
	vbox_area->pack_start(*rb_file,Gtk::PACK_SHRINK);
	vbox_area->pack_start(*rb_mic,Gtk::PACK_SHRINK);
	vbox_area->pack_start( *manage( new Gtk::HSeparator() ), Gtk::PACK_SHRINK );
	vbox_area->pack_start( *manage( new Gtk::Label("Audio Output:") ), Gtk::PACK_SHRINK );
	vbox_area->pack_start(*rb_esd,Gtk::PACK_SHRINK);
	vbox_area->pack_start(*rb_oss,Gtk::PACK_SHRINK);
	vbox_area->pack_start(*rb_artsd,Gtk::PACK_SHRINK);

  // read the inputs
	if (mic_input)
		rb_mic->set_active();
	else
		rb_file->set_active();
	// read the outputs
	if (audio_sink == "esdsink")
		rb_esd->set_active();
	else if (audio_sink == "osssink")
		rb_oss->set_active();
	else if (audio_sink == "artsdsink")
		rb_artsd->set_active();

	option_dialog.add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
	option_dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);

	option_dialog.show_all_children();
	int result = option_dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
			mic_input = rb_mic->get_active();
			if ( rb_esd->get_active() )
				audio_sink = "esdsink";
			else if ( rb_oss->get_active() )
				audio_sink = "osssink";
			else if ( rb_artsd->get_active() )
				audio_sink = "artsdsink";
			#ifndef NDEBUG
      std::cout << "OK clicked. Selected: " << audio_sink << std::endl;
			#endif
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
			#ifndef NDEBUG
      std::cout << "Cancel clicked." << std::endl;
			#endif
			option_dialog.hide();
      break;
    }
	}
}

void Thread::on_volume_changed()
{
  ControlPort *cobject = dynamic_cast<ControlPort *>(object);

	// get current volume setting
	// double volume = cobject->m_vol_slider.get_value();

	// TODO:  implement volume control
}
