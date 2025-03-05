CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Ox /diagnostics:caret

SOURCES = bus_routes.cpp \
		  database.cpp \
		  geo.cpp \
		  json.cpp

HEADERS = database.h \
		  geo.h \
		  graph.h \
		  json.h \
		  router.h

all: bus_routes.exe

single: solution.cc

bus_routes.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Febus_routes.exe $(SOURCES)

solution.cc: $(SOURCES) $(HEADERS) single_file.h single_file.cc
	cl /P single_file.cc
	type single_file.h > solution.cc
	type single_file.i >> solution.cc

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe *.i 2>NUL
