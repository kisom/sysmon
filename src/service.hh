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


#ifndef __SYSMON_SERVICE_HH__
#define __SYSMON_SERVICE_HH__


#include <sys/types.h> // needed for pid_t

#include <ostream>

#include <job.hh>

namespace smsvc {

	class Service : Job {
	public:
		// A Service is constructed from a configuration file.
		Service()
		{
			this->istatus = new Status(State::INIT_ERROR, -1);
		}
		~Service();

		int	 start(void);
		int	 stop(void);
		int	 kill(int);
		int	 kill(std::string);
		Status	 status(void);

		bool	 running(void);
		bool	 good(void);

		// Based on the Common Lisp version; every
		// job has a name, the command line, and a
		// (possibly empty) description.
		std::string			name;
		std::vector<std::string>	comm;
		std::string			desc;

		// A service has additional fields: the PID
		// must be stored in its own file, as well.
		std::string			pidfile;

		friend std::ostream& operator<<(std::ostream&, const Service&);

		// load_service reads a service configuration from disk
		// into the Service.
		friend int	load_service(std::string, Service&);

	private:
		Status	*istatus;
		bool	 loaded;
		pid_t	 pid;

		int	 write_pidfile(pid_t);
	};

	std::ostream&	operator<<(std::ostream&, const Service&);
	int		load_service(std::string, Service&);
} // namespace smsvc


#endif	// #ifndef __SYSMON_SERVICE_HH__
