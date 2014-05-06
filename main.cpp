#include <iostream>
#include <libconfig.h++>
#include <memory>
#include "Display.hpp"
#include "Firefly.hpp"
#include "Tracker.hpp"
#include "TrackingAlgorithm.hpp"

int parseConfig(libconfig::Config&);

int main()
{
	libconfig::Config cfg;
	if(parseConfig(cfg) != 0) return -1;
	libconfig::Setting &setting = cfg.getRoot();

	KLT klt;
	Tracker tracker(setting, klt);

	Display display("App");
	char keyPressed = '\0';

	while(keyPressed != 'q') {
		tracker.update();
		keyPressed = display.render(tracker.getFrame());
	}

	return 0;
}

int parseConfig(libconfig::Config &cfg)
{
	try {
		cfg.readFile("configuration.cfg");
	}
	catch(const libconfig::ParseException &pex) {
		std::cerr << "Error:\t" << pex.getError() << std::endl;
		std::cerr << "File:\t" << pex.getFile() << std::endl;
		std::cout << "Line:\t" << pex.getLine() << std::endl;
		return -1;
	}
	catch(const libconfig::FileIOException) {
		std::cerr << "Could not find configuration file \"configuration.cfg\"" << std::endl;
		return -1;
	}
	return 0;
}