# Authors: <CMS>
#          Filip Bartek (2013)

all: class_checker.o mutable_member.o const_cast.o const_cast_away.o global_static.o static_local.o global_static_edm.o varname.o finitemath.o usingnamespace.o checkerdisabler.o globalaccinctor.o

mutable_member.o: mutable_member.cpp
	$(CXX) mutable_member.cpp

const_cast.o: ThreadSafety/const_cast.cpp
	$(CXX) ThreadSafety/const_cast.cpp

const_cast_away.o: ThreadSafety/const_cast_away.cpp
	$(CXX) ThreadSafety/const_cast_away.cpp

global_static.o: ThreadSafety/global_static.cpp
	$(CXX) ThreadSafety/global_static.cpp

global_static_edm.o: ThreadSafety/global_static_edm.cpp
	$(CXX) ThreadSafety/global_static_edm.cpp

static_local.o: ThreadSafety/static_local.cpp
	$(CXX) ThreadSafety/static_local.cpp

class_checker.o: class_checker.cpp
	$(CXX) -std=c++0x class_checker.cpp

varname.o: varname.c
	$(CC) varname.c

finitemath.o: finitemath.c
	$(CC) finitemath.c

usingnamespace.o: usingnamespace.cpp usingnamespace.h
	$(CXX) usingnamespace.cpp

checkerdisabler.o: checkerdisabler.cpp
	$(CXX) checkerdisabler.cpp

globalaccinctor.o: globalaccinctor.cpp
	$(CXX) globalaccinctor.cpp
