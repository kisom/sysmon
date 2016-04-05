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


#include <iostream>

#include <klogger/console.hh>
#include <service.hh>

using namespace std;


static void
usage(bool err, const char *argv0)
{
	std::basic_ostream<char>	*disp = &cout;
	if (err) {
		disp = &cerr;
	}

	*disp << "Usage: " << argv0 << " job-file\n";
}

int
main(int argc, const char *argv[])
{
	klog::ConsoleLogger	console;

	if (2 != argc) {
		usage(true, argv[0]);
	}

	smsvc::Service	svc;
	if (-1 == smsvc::load_service(argv[1], svc)) {
		// logger::console->error("[monitor] failed to load service");
		exit(1);
	}

	cout << "Loaded service " << svc << endl;
}
