bin_PROGRAMS = runner
runner_CXXFLAGS = -I/usr/include/python2.7
runner_LDFLAGS = -pthread
runner_LDADD = -lpython2.7
runner_SOURCES = src/datafeed.cc \
	src/job.cc \
	src/jobctrl.cc \
	src/main.cc \
	src/runner.cc \
	src/strategy/strategy1.cc \
	src/strategy/strategy2.cc \
	src/strategy_factory.cc \
	src/util.cc
