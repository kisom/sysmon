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


#include <stdlib.h>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <parser.hh>


namespace smparser {

static std::string
trim(std::string& str)
{
	size_t	first;
	size_t	last;

	if (0 == str.size()) {
		return "";
	}

	first = str.find_first_not_of(' ');
	last = str.find_last_not_of(' ');

	if (first == last) {
		return "";
	}

	return str.substr(first, (last-first+1));
}


static std::vector<std::string>
split_pair(std::string line)
{
	std::vector<std::string>	vec;
	std::string			elem;
	auto				index = line.find('=');

	if (std::string::npos != index) {
		elem = line.substr(0, index);
		elem = trim(elem);
		vec.push_back(elem);

		elem = line.substr(index+1);
		elem = trim(elem);
		vec.push_back(elem);
	}

	return vec;
}


std::string
Parser::file()
{
	std::string	s(this->path);
	return s;
}


int
Parser::parse()
{
	std::ifstream	fileh;
	std::string	line;

	try {
		fileh.open(this->path);
	} catch (std::ifstream::failure &e) {
		return -1;
	};

	if (!fileh.good()) {
		return -1;
	}

	while (std::getline(fileh, line)) {
		if ('#' == line[0]) {
			continue;
		}

		line = trim(line);
		if (line.empty()) {
			continue;
		}

		std::vector<std::string> kv = split_pair(line);
		if (kv.empty()) {
			return -1;
		}

		this->store[kv[0]] = kv[1];
	}

	return 0;
}


std::string
Parser::getv(std::string key)
{
	return this->store[key];
}


std::vector<std::string>
Parser::keys()
{
	std::vector<std::string>	keyv;

	for (auto it = this->store.begin(); it != this->store.end(); it++) {
		keyv.push_back(it->first);
	}

	return keyv;
}


} // namespace smparser
