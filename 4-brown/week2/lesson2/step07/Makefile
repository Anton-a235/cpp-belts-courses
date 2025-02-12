CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Od /diagnostics:caret /I..\..\..\utility

SOURCES = step07.cc \
		  json.cpp \
		  xml.cpp

HEADERS = json.h \
		  xml.h \
		  ..\..\..\utility\test_runner.h

all: step07.exe

.PHONY: step07.exe
step07.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Festep07.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
