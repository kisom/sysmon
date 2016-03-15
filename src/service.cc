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


#define _POSIX_SOURCE

#include <sys/types.h>
#include <csignal>
#include <fstream>
#include <sstream>

#include <parser.hh>
#include <service.hh>
#include <logging.hh>


namespace smsvc {


std::ostream& operator<<(std::ostream& os, const Service& svc)
{
	os << "service: " << svc.name;
	if (!svc.desc.empty()) {
		os << " (" << svc.desc << ")";
	}
	return os;
}


Service::~Service()
{
	delete this->istatus;
}

int
load_service(std::string path, Service &svc)
{
	smparser::Parser	p(path);

	auto console = logger::get_logger("service.cc");
	if (svc.loaded) {
		console->error("[load service] path={}, "
		    "service already loaded");
		return -1;
	}

	svc.istatus = new Status(State::INIT_ERROR, -1);

	if (-1 == p.parse()) {
		return 1;
	}

	// TODO: sanity checks on config file values.
	std::string	name = p.getv("name");
	std::string	comm = p.getv("program");

	// Without a name or command line, we can't do anything.
	if (name.empty() && comm.empty()) {
		console->error("[load_service] path={}, name=\"\", "
		    "program=\"\"", path);
		return -1;
	}
	else if (name.empty()) {
		// TODO: run basename on this.
		std::istringstream	iss(comm);

		iss >> name;
	}
	else {
		comm = name;
	}

	svc.name = name;

	std::istringstream	iss(comm);
	svc.comm.clear();
	while (iss) {
		std::string	arg;
		iss >> arg;
		svc.comm.push_back(arg);
	}

	svc.desc = p.getv("desc");
	svc.pidfile = p.getv("pidfile");

	// TODO: don't hardcode pidfile path like this. This needs some
	// thinking about how to do configuration.
	if (svc.pidfile.empty()) {
		svc.pidfile = "/var/run/" + svc.name + ".pid";
	}

	svc.loaded = true;
	delete svc.istatus;
	svc.istatus = new Status(State::READY, 0);

	return 0;
}


int
Service::start()
{
	return -1;
}


int
Service::stop()
{
	return -1;
}


int
Service::kill(int signal)
{
	return ::kill(this->pid, signal);
}


int	
Service::kill(std::string signame)
{
	const Signal	*sig = get_signal(signame);

	return ::kill(this->pid, sig->value);
}


Status
Service::status()
{
	return *(this->istatus);
}


bool
Service::running()
{
	return false;
}


bool
Service::good()
{
	return false;
}


} // namespace smsvc
