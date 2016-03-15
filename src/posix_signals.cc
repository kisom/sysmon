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


#include <cassert>
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <stdlib.h>

#include <logging.hh>
#include <posix_signals.hh>


namespace smsvc {


static std::map<int, Signal*>		signals = {};
static std::map<std::string, Signal*>	signalstr = {};


static void
add_signal(std::string name, int value, SignalAction act, std::string desc)
{
	auto sig = new Signal(name, value, act, desc);

	signals[value] = sig;
}


void
signal_init()
{
	if (!signals.empty()) {
		return;
	}

	add_signal("SIGHUP", 1, SignalAction::ACTION_TERM,
	    "Hangup detected on controlling terminal or death of controlling process.");
	add_signal("SIGINT", 2, SignalAction::ACTION_TERM,
	    "Interrupt from keyboard.");
	add_signal("SIGQUIT", 3, SignalAction::ACTION_CORE,
	    "Quit from keyboard.");
	add_signal("SIGILL", 4, SignalAction::ACTION_CORE,
	    "Illegal instruction.");
	add_signal("SIGABRT", 6, SignalAction::ACTION_CORE,
	    "Abort signal from abort(3).");
	add_signal("SIGFPE", 8, SignalAction::ACTION_CORE,
	    "Floating point exception.");
	add_signal("SIGKILL", 9, SignalAction::ACTION_TERM,
	    "Kill signal.");
	add_signal("SIGSEGV", 11, SignalAction::ACTION_CORE,
	    "Invalid memory reference.");
	add_signal("SIGPIPE", 13, SignalAction::ACTION_TERM,
	    "Broken pipe: write to pipe with no readers.");
	add_signal("SIGALRM", 14, SignalAction::ACTION_TERM,
	    "Timer signal from alarm(2).");
	add_signal("SIGTERM", 15, SignalAction::ACTION_TERM,
	    "Termination signal.");
	add_signal("SIGUSR1", 10, SignalAction::ACTION_TERM,
	    "User-defined signal 1.");
	add_signal("SIGUSR2", 12, SignalAction::ACTION_TERM,
	    "User-defined signal 2.");
	add_signal("SIGCHLD", 17, SignalAction::ACTION_IGNORE,
	    "Child stopped or terminated.");
	add_signal("SIGCONT", 18, SignalAction::ACTION_CONTINUE,
	    "Continue if stopped.");
	add_signal("SIGSTOP", 19, SignalAction::ACTION_STOP,
	    "Stop process.");
	add_signal("SIGTSTP", 20, SignalAction::ACTION_STOP,
	    "Stop typed at terminal.");
	add_signal("SIGTTIN", 21, SignalAction::ACTION_STOP,
	    "Terminal input for background process.");
	add_signal("SIGTTOU", 22, SignalAction::ACTION_STOP,
	    "Terminal output for background process.");

	// susv2 / posix.1-2001 signals
	add_signal("SIGBUS", 7, SignalAction::ACTION_CORE,
	    "Bus error (bad memory access).");
	add_signal("SIGPOLL", 29, SignalAction::ACTION_TERM,
	    "Pollable event (Sys V). Synonym for SIGIO.");
	add_signal("SIGPROF", 27, SignalAction::ACTION_TERM,
	    "Profiling timer expired.");
	add_signal("SIGSYS", 31, SignalAction::ACTION_CORE,
	    "Bad argument to routine (SVr4).");
	add_signal("SIGTRAP", 5, SignalAction::ACTION_CORE,
	    "Trace/breakpoint trap.");
	add_signal("SIGURG", 23, SignalAction::ACTION_IGNORE,
	    "Urgent condition on socket (4.2BSD).");
	add_signal("SIGVTALRM", 26, SignalAction::ACTION_TERM,
	    "Virtual alarm clock (4.2BSD).");
	add_signal("SIGXCPU", 24, SignalAction::ACTION_CORE,
	    "CPU time limit exceeded (4.2BSD).");
	add_signal("SIGXFSZ", 25, SignalAction::ACTION_CORE,
	    "File size limit exceeded (4.2BSD).");

	// various other signals
	add_signal("SIGIOT", 6, SignalAction::ACTION_CORE,
	    "IOT trap. A synonym for SIGABRT.");
	add_signal("SIGSTKFLT", 16, SignalAction::ACTION_TERM,
	    "Stack fault on coprocessor (unused).");
	add_signal("SIGIO", 29, SignalAction::ACTION_TERM,
	    "I/O now possible (4.2BSD).");
	add_signal("SIGPWR", 30, SignalAction::ACTION_TERM,
	    "Power failure (System V).");
	add_signal("SIGWINCH", 28, SignalAction::ACTION_IGNORE,
	    "Window resize signal (4.3BSD, Sun).");
	add_signal("SIGUNUSED", 31, SignalAction::ACTION_CORE,
	    "Synonymous with SIGSYS.");

	for (auto it = signals.begin(); it != signals.end(); it++) {
		auto sig = it->second;
		signalstr[sig->name] = sig;
	}
}


static void
signal_check_and_init(void)
{
	if (signals.empty()) {
		signal_init();
	}

	assert(!(signals.empty()));
	assert(!(signalstr.empty()));
}


static bool
action_fatal(SignalAction a)
{
	auto console = logger::get_logger("posix_signals.cc");

	switch (a) {
	case SignalAction::ACTION_TERM:
	case SignalAction::ACTION_CORE:
		return true;
	case SignalAction::ACTION_IGNORE:
	case SignalAction::ACTION_CONTINUE:
	case SignalAction::ACTION_STOP:
		return false;
	// The default case includes ACTION_INVALID.
	default:
		console->error("[action_fatal] invalid action {}",
		    static_cast<std::underlying_type<SignalAction>::type>(a));
		abort();
	}
}


bool
signal_fatal(Signal *sig)
{
	return action_fatal(sig->action);
}


Signal *
get_signal(int v)
{
	Signal	*sig = nullptr;

	signal_check_and_init();
	auto console = logger::get_logger("posix_signals.cc");

	sig = signals[v];
	if (nullptr == sig) {
		console->error("[get_signal(int v)] signal {} "
		    "isn't a valid signal.", v);
		abort();
	}

	return sig;
}


Signal *
get_signal(std::string name)
{
	Signal		*sig = nullptr;
	std::string	 signame(name);

	auto console = logger::get_logger("posix_signals.cc");
	signame = signal_name(signame);
	signal_check_and_init();

	sig = signalstr[name];
	if (nullptr == sig) {
		console->error("[get_signal(std::string name)] "
		    "signal {} isn't a valid signal.", name);
		abort();
	}

	return sig;
}


bool
signal_fatal(int v)
{
	Signal	*sig = get_signal(v);

	return action_fatal(sig->action);
}


std::string
signal_name(std::string name)
{
	static auto	sigre = std::regex("^SIG[A-Z]+\\d?$");

	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	if (std::regex_match(name, sigre)) {
		return name;
	}

	return "SIG" + name;
}

} // namespace smsvc
