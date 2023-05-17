ASCII GFX v1.2 as of 9.May.2023
===============================

This software is a library on top of the NCurses library for ease of
rendering ASCII graphics.

Installation
------------

To use this library, the NCurses development library needs to be
installed as a dependency.

On Ubuntu for example, we install the following package: libncurses5-dev

To install the above package (plus GCC and CMake), we open a terminal
window and type the following command line:

> sudo apt-get install g++ cmake libncurses5-dev

Optionally, the SDL library may be installed to provide sound playback
and joystick support via the SDL packages libsdl1.2-dev and
libsdl-mixer1.2-dev or the SDL2 packages libsdl2-dev and libsdl2-mixer-dev.

So to install SDL for sound and joystick support, we type:

> sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev joystick

On other Linux distributions the procedure is very similar - just the
package installer and the package names usually differ.

Compilation
-----------

A simple demo application is available in the source file
main.cpp. The demo application can be compiled and started on the
command line as follows:

> cmake . && make && ./main

This serves as a starting point for developing your own NCurses based
applications. It mainly demonstrates how to initialize the library,
use its built-in 5x3 grid font and cleanup things afterwards.

Demos
-----

The demos in the examples directory are intended to be just copied over
main.cpp in the parent directory.

Programming API
---------------

The software consists of the following modules:

* gfx.h/.cpp: basic graphics like sprite rendering and line drawing
* scrollarea.h/.cpp: shows a scrollable window as a cut-out of a larger area
* gridfont.h/.cpp: ASCII font made up of grid characters with 5x3 columns resp. rows
* gridarea.h/.cpp: shows a scrollable grid area made up of 5x3 grid characters
* gridmenu.h/.cpp: shows a simple overlay menu made up of grid characters
* sound.h/.cpp: sound playback for WAV files
* joystick.h/.cpp: joystick input handling
* mouse.h/.cpp: mouse input handling
* util.h/.cpp: utility functions
* main.cpp: demo application

Documentation
-------------

You can use Doxygen to generate a html documentation of the API and
its contained functions:

> doxygen

Then simply open the generated index.html file in docs/html.

Happy ASCII coding!
Stefan
