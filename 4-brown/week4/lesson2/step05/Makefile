CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Od /diagnostics:caret /I..\..\..\utility

SOURCES = main.cpp \
		  Solution.cpp \
		  Textures.cpp

HEADERS = Common.h \
		  Textures.h \
		  ..\..\..\utility\test_runner.h

all: Solution.exe

.PHONY: Solution.exe
Solution.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /FeSolution.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
