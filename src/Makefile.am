AM_CPPFLAGS  =	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
AM_CPPFLAGS +=	-Wwrite-strings -Wmissing-declarations -Wno-long-long -Werror
AM_CPPFLAGS +=	-Wunused-variable -std=c++11 -D_XOPEN_SOURCE -Os -I.
AM_CPPFLAGS +=	-fno-elide-constructors	 -Weffc++
LDADD =		-lpthread
SYSMONCC =	parser.cc parser.hh 				\
		posix_signals.hh posix_signals.cc		\
		job.hh job.cc					\
		service.hh service.cc


noinst_PROGRAMS =	monitor signal_lookup
monitor_SOURCES =	$(SYSMONCC) monitor.cc
signal_lookup_SOURCES =	$(SYSMONCC) signal_lookup.cc

check_PROGRAMS =	parser_test
parser_test_SOURCES =	parser.cc parser.hh parser_test.cc

.PHONY: scanners clang-scanner cppcheck-scanner
scanners: clang-scanner cppcheck-scanner
clang-scanner:
	clang++ $(AM_CPPFLAGS) --analyze $(SYSMONCC)

cppcheck-scanner:
	cppcheck --quiet --enable=all -I ./ $(SYSMONCC)

.PHONY: coverity-scan
coverity-scan: clean
	cov-build --dir cov-int make all check
	tar czf $(PACKAGE_NAME)-$(PACKAGE_VERSION)_coverity.tar.gz cov-int
	rm -rf cov-int

