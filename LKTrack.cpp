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
	bool displayOn;

	Timer mainLoop("MainLoop");
	Timer imageFetch("ImageFetch");
	Timer calcFlow("CalcFlow");
	Timer displayTimer("Display\t");

	Firefly firefly;
	Display display("App");
	Tracker lktracker(termCrit);

	if(argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " num_points display[on|off]" << std::endl;
		return -1;
	}

	numPoints = atoi(argv[1]);
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

	std::cout << std::endl; /* Break between initialisation text and loop text. */
	char keyPressed;
	uint numCycles = 0;
	while(keyPressed != 'q')
	{
		mainLoop.mark(); /* Begin loop timer. */
		std::cout << "Cycle number:\t\t" << numCycles << std::endl;
		numCycles++;
		imageFetch.mark(); /* Begin image fetch timer. */
		frame = firefly.getCVFrame();
		imageFetch.print(); /* End image fetch timer. */

		calcFlow.mark(); /*Begin flow calculation timer. */
		p = lktracker.calcFlow(prevFrame, frame, prevP);
		calcFlow.print(); /* End flow calculation timer. */

		if(displayOn)
		{
			displayTimer.mark(); /* Begin display timer. */
			keyPressed = display.render(frame, p);
			displayTimer.print(); /* End display timer. */
		}

		if(keyPressed == 'r')
		{
			p.points = lktracker.getPoints(frame);
		}

		prevP = p;
		prevFrame = frame;
		mainLoop.print(); /* End loop timer. */
		std::cout << "Main loop frequency:\t" << 1.0/mainLoop.read() << " Hz" <<  std::endl;
		std::cout << "Active points:\t\t" << activePoints(prevP) << std::endl;

		std::cout << std::endl; /* Separates timer brackets. */
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
