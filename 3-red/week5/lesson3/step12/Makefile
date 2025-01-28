CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4514 /wd4626 /wd4820 /wd5027 /wd5045 /Od /diagnostics:caret

SOURCES = stats.cpp \
		  step12.cpp

HEADERS = stats.h \
		  http_request.h \
		  ../test_runner.h

all: stats.exe

.PHONY: stats.exe
stats.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Festats.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
