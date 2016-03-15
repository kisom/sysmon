/*
 * Copyright (c) 2015 K. Isom <coder@kyleisom.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * copy of this  software and associated documentation  files (the "Software"),
 * to deal  in the Software  without restriction, including  without limitation
 * the rights  to use,  copy, modify,  merge, publish,  distribute, sublicense,
 * and/or  sell copies  of the  Software,  and to  permit persons  to whom  the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS  PROVIDED "AS IS", WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING  BUT NOT  LIMITED TO  THE WARRANTIES  OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS  OR COPYRIGHT  HOLDERS BE  LIABLE FOR  ANY CLAIM,  DAMAGES OR  OTHER
 * LIABILITY,  WHETHER IN  AN ACTION  OF CONTRACT,  TORT OR  OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */


#ifndef __SYSMON_JOB_HH__
#define __SYSMON_JOB_HH__


#include <posix_signals.hh>


namespace smsvc {

	// State describes the current state of the program.
	enum class State {
		INVALID_STATE,

		// An error occurred during initialisation.
		INIT_ERROR,

		// The job is ready to be run.
		READY,

		// The job is currently running.
		RUNNNING,

		// The job has been stopped.
		STOPPED,

		// The job was killed with a signal.
		SIGNALLED,

		// The job exited.
		EXITED,
	};


	// state_string returns a string name for the State. The returned
	// string is suitable for use as "the job ..."
	std::string	state_string(State);


	// InitError describes an initialisation error.
	enum class InitError {
		INVALID_JOB,	// The job was invalid --- either a job file
				// failed to parse, or something passed on
				// the API failed.
		MISSING_NAME,	// The program is missing a name. Note that if
				// the name is inferred from the program,
				// this means neither a program nor name was
				// given.
		UNKNOWN,	// This is a catch all and should be
				// cause for an abort().
	};

	// A Status contains the program's running status.
	class Status {
	public:
		Status(State s, int c) : istate(s), icode(c) {};

		State	state(void) { return this.istate; }
		int	code(void)  { return this.code; }

		// Return true if the state indicates a healthy job.
		bool	good(void);

	private:
		State	istate;
		int	icode;

	};

	class Job {
	public:
		int	start(void);
		int	stop(void);
		int	kill(int);
		int	kill(std::string);
		Signal	status(void);

		bool	running(void);
		bool	good(void);
	};

} // namespace smsvc


#endif	// #ifndef __SYSMON_JOB_HH__

