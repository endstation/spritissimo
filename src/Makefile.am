AM_CPPFLAGS = -DPKG_DATA_DIR=\"$(pkgdatadir)\" \
	      @SFML_CFLAGS@ \
	      @TINYXML2_CFLAGS@ \
	      @SIGCPP_CFLAGS@ \
	      @LIBPNG_CFLAGS@ \
	      -std=c++11 \
	      -Wall \
	      -g 
spritissimo64_LDADD = @SFML_LIBS@ @TINYXML2_LIBS@ @SIGCPP_LIBS@ @LIBPNG_LIBS@ -lboost_system -lboost_filesystem \
		      -lboost_program_options

bin_PROGRAMS = spritissimo64
spritissimo64_SOURCES = \
		      main.cpp \
		      Sprite.h \
		      Sprite.cpp \
		      Editing_panel.h \
		      Editing_panel.cpp \
		      Application.h \
		      Application.cpp \
		      Resolution.h \
		      Resolution.cpp \
		      Palette_panel.h \
		      Palette_panel.cpp \
		      Project.h \
		      Project.cpp \
		      Preview_panel.h \
		      Preview_panel.cpp \
		      Editing_status_panel.h \
		      Editing_status_panel.cpp \
		      Globals.h \
		      Heartbeat.h \
		      Heartbeat.cpp \
		      Undo_stack.h \
		      Undo_stack.cpp \
		      Clipboard.h \
		      Clipboard.cpp \
		      File_IO.h \
		      File_IO.cpp \
		      Status_bar.h \
		      Status_bar.cpp \
		      Config.h \
		      Config.cpp \
		      Animation_panel.h \
		      Animation_panel.cpp \
		      Animation_status_panel.h \
		      Animation_status_panel.cpp \
		      Editing_event.h \
		      Editing_event.cpp \
		      Help_screen.h \
		      Help_screen.cpp \
		      Color_selection_panel.h \
		      Color_selection_panel.cpp \
		      Palette.h \
		      Palette.cpp \
		      Panel_dims.h \
		      Zoomable.h \
		      Zoomable.cpp \
		      Sprite_dims.h \
		      Sprite_dims.cpp



