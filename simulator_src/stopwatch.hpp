#ifndef __SIMULATOR__STOPWATCH__H__
#define __SIMULATOR__STOPWATCH__H__

#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class StopWatch {

	typedef std::chrono::high_resolution_clock Clock;

	Clock clock;

	std::chrono::time_point<Clock> begin;

	std::vector<std::chrono::time_point<Clock> > duration;

	std::vector<std::string> name;

	int counter;

	std::string thisName;

	typedef std::chrono::milliseconds ms;
	typedef std::chrono::seconds sec;

public:

	explicit StopWatch(const std::string &__s = "Stop Watch")
			: begin(clock.now()), counter(0) {
		start();
		thisName = __s;
	}

	void start()  {
		begin = clock.now();
		counter = 0;
	}

	long take(std::string str = ""){
		duration.push_back(clock.now());
		if (str.empty())
			str = "T" + std::to_string(counter + 1);
		name.push_back(str);

		counter++;

		return std::chrono::duration_cast<ms> (
				duration.back() - begin
		).count();
	}

	void report(bool seconds = false) {
		std::cout << "Report of " << thisName << "\n";
		std::cout << std::setw(2) << " ";
		std::cout << std::setw(12) << " ";
		std::cout << std::setw(10) << "Int";
		std::cout << std::setw(10) << "Acc\n";

		for (int i = 0; i < counter; ++i) {
			long dt;
			std::cout << std::setw(2) << (i + 1) << ".";
			std::cout << std::setw(15) << name[i];

			if (seconds)
				dt = std::chrono::duration_cast<sec>(
						duration[i] - ((i > 0) ? duration[i - 1] : begin)
				).count();
			else
				dt = std::chrono::duration_cast<ms>(
						duration[i] - ((i > 0) ? duration[i - 1] : begin)
				).count();
			std::cout << std::setw(8) << dt;
			if (seconds)
				std::cout << "s ";
			else
				std::cout << "ms";

			if (seconds)
				dt = std::chrono::duration_cast<sec>(duration[i] - begin).count();
			else
				dt = std::chrono::duration_cast<ms>(duration[i] - begin).count();
			std::cout << std::setw(8) << dt;
			if (seconds)
				std::cout << "s ";
			else
				std::cout << "ms";

			std::cout << std::endl;
		}
		std::cout.clear();
	}
};

#endif //VE490_STOPWATCH_H
