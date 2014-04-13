# Quadcopter Linux Development

This software provides functionality relating to optical flow on an ODROID platform. This includes\:
 * Point Grey Firefly camera interface code
 	* Includes conversion of images to OpenCV datatypes
 * Display code to visualise points
 * A simple timer class allowing for rudimentary function profiling

This code is currently *very* rough. Whilst some effort has been made to keep the classes neat, the main LKTrack file has been mangled a bit to hack up functionality. Apologies also for the minimal commenting; familiarity with the APIs (Point Grey and OpenCV) makes it pretty self-documenting.

## Brief guide to the source code

 * LKTrack
 	* Entry point/main application loop. Implements all functionality including command line argument parsing, timers, and output.
 * Display
 * Firefly
 * Makefile
 * Points
 * Timer
 * Tracker
 * Makefile
 	* For this code to compile you must have the OpenCV and Point Grey Flycapture libraries installed in the listed locations.
