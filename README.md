# Quadcopter Linux Development

This software provides optical flow calculation from a PointGrey Firefly video source. Included is:

 * Point Grey Firefly camera interface code
 > Includes conversion of images to OpenCV datatypes
 > Options for external triggering or freerunning video capture
 > Asynchronous callback for image capture
 * Tracker class which manages image acquisition and optical flow calculation
 * TrackingAlgorithm class (interface) which allows for polymorphic usage of specific tracking algorithms
 > KLT Algorithm
 * Display code to visualise points

## Brief guide to the source code

* main.cpp
> Parses configuration file, instantiates objects and maintains application loop.
* Firefly
> Wraps the PointGrey Flycapture2 API, providing an OpenCV-compatible interface.
> Stores a thread-safe FIFO queue of images which may be sequentially retrieved with nextFrame().
* Tracker
> Manages tracker state (stores a queue of images to be processed), and provides an image output for display.
* TrackingAlgorithm
> Provides a pure abstract interface allowing different tracking algorithms to be used interchangeably.
* KLT : TrackingAlgorithm
> Provides an implementation of the KLT algorithm (cv::calculateOpticalFlowPyrLK).
* FlowFrame
> Data structure combining an image and associated points data.
* Display
> Maintains display window state.
> Allows display of plain cv::Mats or FlowFrames with points included.

---

## Changelog

* Completely overhauled source code to better follow event-driven and multithreaded design principles.