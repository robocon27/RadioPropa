/***************************************************************************
 *   ProgressBar.h implements a ProgressBar for calculations not           *
 *                 producing additional output                             * 
 *   Copyright (C) 2010 by Tobias Winchen                                  *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>
#include <ctime>

class ProgressBar {
private:
	unsigned long _steps;
	unsigned long _currentCount;
	unsigned long _maxbarLength;
	unsigned long _nextStep;
	unsigned long _updateSteps;
	time_t _startTime;
	std::string stringTmpl;
	std::string arrow;

public:
	/// Initialize a ProgressBar with [steps] number of steps, updated at [updateSteps] intervalls
	ProgressBar(unsigned long steps = 0, unsigned long updateSteps = 100) :
			_steps(steps), _currentCount(0), _maxbarLength(10), _updateSteps(
					updateSteps), _nextStep(1), _startTime(0) {
		if (_updateSteps > _steps)
			_updateSteps = _steps;
		arrow.append(">");
	}

	void start(const std::string &title) {
		_startTime = time(NULL);
		std::string s = ctime(&_startTime);
		s.erase(s.end() - 1, s.end());
		stringTmpl = "  Started ";
		stringTmpl.append(s);
		stringTmpl.append(" : [%-10s] %3i%%    %s: %02i:%02is %s\r");
		std::cout << title << std::endl;

	}
	/// update the progressbar
	/// should be called steps times in a loop 
	void update() {
		_currentCount++;
		if (_currentCount == _nextStep || _currentCount == _steps) {
			_nextStep += long(_steps / float(_updateSteps));

			int percentage = int(100 * _currentCount / float(_steps));
			time_t currentTime = time(NULL);
			if (_currentCount < _steps) {
				int j = 0;
				if (arrow.size()
						<= (_maxbarLength) * (_currentCount) / (_steps))
					arrow.insert(0, "=");
				float tElapsed = currentTime - _startTime;
				float tToGo = (_steps - _currentCount) * tElapsed
						/ _currentCount;
				printf(stringTmpl.c_str(), arrow.c_str(), percentage,
						"Finish in", int(tToGo / 60), int(tToGo) % 60, "");
				fflush(stdout);
			} else {
				float tElapsed = currentTime - _startTime;
				std::string s = " - Finished at ";
				s.append(ctime(&currentTime));
				char fs[255];
				sprintf(fs, "%c[%d;%dm Finished %c[%dm", 27, 1, 32, 27, 0);
				printf(stringTmpl.c_str(), fs, percentage, "Needed",
						int(tElapsed / 60), int(tElapsed) % 60, s.c_str());
			}
		}
	}

	/// Mark the progressbar with an error
	void setError() {
		time_t currentTime = time(NULL);
		_currentCount++;
		float tElapsed = currentTime - _startTime;
		std::string s = " - Finished at ";
		s.append(ctime(&currentTime));
		char fs[255];
		sprintf(fs, "%c[%d;%dm  ERROR   %c[%dm", 27, 1, 31, 27, 0);
		printf(stringTmpl.c_str(), fs, _currentCount, "Needed",
				int(tElapsed / 60), int(tElapsed) % 60, s.c_str());
	}

};
