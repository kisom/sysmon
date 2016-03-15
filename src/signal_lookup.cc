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


#include <cstdlib>
#include <iostream>
#include <regex>

#include <posix_signals.hh>

using namespace std;


static bool
is_number(const char *s)
{
	static auto	nre = regex("^\\d{1,2}$");

	return regex_match(s, nre);
}


static void
display_signal(smsvc::Signal *sig)
{
	cout << sig->name << " (" << sig->value << ")\n";
	cout << sig->description << endl;
}


int
main(int argc, const char *argv[])
{
	if (2 != argc) {
		cerr << "Usage: " << argv[0] << " signal\n";
		exit(1);
	}

	if (is_number(argv[1])) {
		int	n = atoi(argv[1]);

		smsvc::Signal *sig = smsvc::get_signal(n);
		display_signal(sig);
	}
	else {
		string	name = smsvc::signal_name(string(argv[1]));

		smsvc::Signal *sig = smsvc::get_signal(name);
		display_signal(sig);
	}

	return 0;
}
