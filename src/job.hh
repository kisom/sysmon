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


#include <vector>

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
		RUNNING,

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

		State	state(void) { return this->istate; }
		int	code(void)  { return this->icode; }

		// Return true if the state indicates a healthy job.
		bool	good(void);

	private:
		State	istate;
		int	icode;

	};

	// A Job is an abstract type that provides a common interface for
	// jobs. A job can be scheduled (à la cron) or a service (such
	// as managed by OpenRC or systemd).
	class Job {
	public:
		virtual int	start(void) = 0;
		virtual int	stop(void) = 0;
		virtual int	kill(int) = 0;
		virtual int	kill(std::string) = 0;
		virtual Status	status(void) = 0;

		virtual bool	running(void) = 0;
		virtual bool	good(void) = 0;

	private:
		// Note that private data fields in a purely
		// virtual class have no effect; they're
		// provided here for consistency in inherited classes.

		// Based on the Common Lisp version; every
		// job has a name, the command line, and a
		// (possibly empty) description.
		std::string			name;
		std::vector<std::string>	comm;
		std::string			desc;
	};

} // namespace smsvc


#endif	// #ifndef __SYSMON_JOB_HH__

