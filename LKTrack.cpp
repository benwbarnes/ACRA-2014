#include <iostream>
#include "Display.hpp"
#include "Firefly.hpp"
#include "Points.hpp"
#include "Timer.hpp"
#include "Tracker.hpp"

int activePoints(Points);

int main(int argc, char *argv[])
{
	cv::Mat frame, prevFrame;
	cv::TermCriteria termCrit = cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);
	Points p, prevP;
	int numPoints;
	int frameLimit;
	bool displayOn;

	Timer mainLoop("MainLoop");
	Timer imageFetch("ImageFetch");
	Timer calcFlow("CalcFlow");
	Timer displayTimer("Display\t");

	Firefly firefly;
	Display display("App");
	Tracker lktracker(termCrit);

	if(argc != 4)
	{
		std::cout << "Usage: " << argv[0] << " num_points display[on|off] frame_limit" << std::endl;
		return -1;
	}

	numPoints = atoi(argv[1]);
	frameLimit = atoi(argv[3]);
	lktracker.setMaxPoints(numPoints);
	if(strncmp(argv[2], "on", 2)) displayOn = false;
	else displayOn = true;

	if(firefly.open())
	{
		std::cout << "Error opening camera." << std::endl;
		return -1;
	}

	prevFrame = firefly.getCVFrame();
	prevP.points = lktracker.getPoints(prevFrame); /* Gets initial point set using feature detection. */

	char keyPressed;
	uint numCycles = 0;
	double ml_secs, fetch_secs, flow_secs, display_secs;

	std::cout << "Cycle\tMain Loop\tFetch\tFlow\tPoints" << std::endl;

	while(keyPressed != 'q' && numCycles < frameLimit)
	{
		mainLoop.mark(); /* Begin loop timer. */
		imageFetch.mark(); /* Begin image fetch timer. */
		frame = firefly.getCVFrame();
		fetch_secs = imageFetch.read();
//		imageFetch.print(); /* End image fetch timer. */

		calcFlow.mark(); /*Begin flow calculation timer. */
		p = lktracker.calcFlow(prevFrame, frame, prevP);
		flow_secs = calcFlow.read();
//		calcFlow.print(); /* End flow calculation timer. */

		if(displayOn)
		{
			displayTimer.mark(); /* Begin display timer. */
			keyPressed = display.render(frame, p);
			display_secs = displayTimer.read();
//			displayTimer.print(); /* End display timer. */
		}

		if(keyPressed == 'r')
		{
			p.points = lktracker.getPoints(frame);
		}

		prevP = p;
		prevFrame = frame;
		ml_secs = mainLoop.read();
//		mainLoop.print(); /* End loop timer. */
		std::cout << numCycles << '\t' << ml_secs << '\t' << fetch_secs << '\t' << flow_secs << '\t' << activePoints(prevP) << std::endl;
		numCycles++;
	}

	firefly.close();
	return 0;
}

int activePoints(Points p)
{
	int activePoints = 0;

	for(auto point : p.status)
	{
		if(point != 0)
		{
			activePoints++;
		}
	}

	return activePoints;
}
