CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Ox /diagnostics:caret /I..\..\..\utility

SOURCES = step01.cc \
		  ini.cpp

HEADERS = ini.h \
		  ..\..\..\utility\test_runner.h

all: step01.exe

.PHONY: step01.exe
step01.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Festep01.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
