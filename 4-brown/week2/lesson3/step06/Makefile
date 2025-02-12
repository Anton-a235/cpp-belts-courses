CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Od /diagnostics:caret /I..\..\..\utility

SOURCES = collide.cpp \
		  geo2d.cpp

HEADERS = geo2d.h \
		  game_object.h \
		  ..\..\..\utility\test_runner.h

all: step06.exe

.PHONY: step06.exe
step06.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Festep06.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
