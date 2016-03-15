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
#include <cstdlib>

#include <job.hh>
#include <logging.hh>


namespace smsvc {


std::string
state_string(State state)
{
	assert(State::INVALID_STATE != state);

	switch (state) {
	case State::INIT_ERROR:
		return "failed to initialise";
	case State::READY:
		return "is ready";
	case State::RUNNNING:
		return "is running";
	case State::STOPPED:
		return "is stopped";
	case State::SIGNALLED:
		return "was killed via a Unix signal";
	case State::EXITED:
		return "exited";
	default:
		logger::console->error("[state_string] invalid state {}",
		    static_cast<std::underlying_type<SignalAction>::type>(state));
		std::abort();
	}
}


} // namespace smsvc

