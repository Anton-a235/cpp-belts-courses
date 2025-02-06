CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Ox /diagnostics:caret

SOURCES = main.cpp \
		  parse.cpp \
		  search_server.cpp

HEADERS = iterator_range.h \
		  parse.h \
		  search_server.h

all: search_server.exe

.PHONY: search_server.exe
search_server.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Fesearch_server.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
