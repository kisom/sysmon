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


// parser_test reads a file containing key = value pairs and prints the keys
// and values stored in it.


#include <iostream>
#include <vector>

#include <getopt.h>

#include <logging.hh>
#include <parser.hh>

using namespace std;


static void
usage(bool err, char *argv0)
{
	std::basic_ostream<char>	*disp = &cout;
	if (err) {
		disp = &cerr;
	}

	*disp << "Usage: " << argv0 << " [-d files ...]\n";
	*disp << "With no arguments, run the unit tests.\n";
	*disp << "With arguments, " << argv0 << " attempts to read and parse "
	      << "key=value\npairs from the listed files.\n";
}


static int
test_parser(smparser::Parser &p, unordered_map<string, string> m)
{
	if (-1 == p.parse()) {
		cerr << "FAILED: parse " << p.file() << "\n";
		return -1;
	}

	vector<string>	keys = p.keys();
	if (keys.size() != m.size()) {
		cerr << "FAILED: expected " << m.size() << " keys, "
		     << "but have " << keys.size() << " keys.\n";
		return -1;
	}

	for (size_t i = 0; i < keys.size(); i++) {
		string	v = p.getv(keys[i]);
		string	e = m[keys[i]];

		if (v != e) {
			cerr << "FAILED: for key " << keys[i]
			     << "\n\texpected value is: " << e
			     << "\n\t  actual value is: " << v << endl;
			return -1;
		}
	}

	return 0;
}


static int
test_docker_svc(void)
{
	smparser::Parser		p("src/testdata/docker.svc");
	unordered_map<string, string>	m{
		{"env:PATH", "/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin"},
		{"errfile", "/var/log/docker.err"},
		{"logfile", "/var/log/docker.log"},
		{"pidfile", "/var/run/docker.pid"},
		{"desc", "the future of computerising"},
		{"name", "docker"},
	};

	return test_parser(p, m);
}


static int
test_fetchmail_job(void)
{
	smparser::Parser		p("src/testdata/fetchmail.job");
	unordered_map<string, string>	m{
		{"frequency", "300"},
		{"program", "/home/kyle/bin/fetchmail.sh"},
		{"desc", "fetch mail"},
		{"name", "fetchmail"},
	};

	return test_parser(p, m);
}


static void
standard_tests(void)
{
	int	failed = 0;

	auto console = logger::get_logger("service.cc");

	console->info("testing docker service file");
	if (-1 == test_docker_svc()) {
		console->error("docker service file test failed");
		failed++;
	}

	console->info("testing fetchmail job file");
	if (-1 == test_fetchmail_job()) {
		console->error("fetchmail job file test failed");
		failed++;
	}

	if (failed > 0) {
		exit(1);
	}
	exit(0);
}


static void
dump_keys(std::string path, smparser::Parser &p)
{
	std::vector<std::string>	keys;

	keys = p.keys();

	cout << keys.size() << " keys in " << path << std::endl;
	for (auto it = keys.begin(); it != keys.end(); it++) {
		cout << "key: " << *it << endl;
		cout << "value: " << p.getv(*it) << endl;
		cout << endl;
	}
}


int
main(int argc, char *argv[])
{
	int	opt;
	bool	dump = false;

	while (-1 != (opt = getopt(argc, argv, "dh"))) {
		switch (opt) {
		case 'd':
			dump = true;
			break;
		case 'h':
			usage(false, argv[0]);
			exit(0);
		default:
			usage(true, argv[0]);
			exit(1);
		}
	};

	if (!dump) {
		standard_tests();
	}

	argc -= optind;
	argv += optind;

	for (int i = 0; i < argc; i++) {
		smparser::Parser	p(argv[i]);
		if (-1 == p.parse()) {
			cerr << "failed to parse " << argv[i] << endl;
			continue;
		}

		dump_keys(argv[i], p);
	}
}
