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


#ifndef __SYSMON_POSIX_SIGNALS_HH__
#define __SYSMON_POSIX_SIGNALS_HH__


#include <map>


namespace smsvc {

	enum class SignalAction {
		ACTION_INVALID,

		ACTION_CONTINUE,
		ACTION_CORE,
		ACTION_IGNORE,
		ACTION_STOP,
		ACTION_TERM,
	};


	class Signal {
	public:
		Signal(std::string n, int v, SignalAction a, std::string d) :
			name(n), value(v), action(a), description(d) {};

		std::string	name;
		int		value;
		SignalAction	action;
		std::string	description;
	};


	void		 signal_init(void);
	bool		 signal_fatal(Signal *);
	bool		 signal_fatal(int);
	Signal		*get_signal(int);
	Signal		*get_signal(std::string);
	std::string	 signal_name(std::string);

} // namespace smsvc


#endif // #ifndef __SYSMON_POSIX_SIGNALS_HH__
