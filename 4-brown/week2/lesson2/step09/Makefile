CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Od /diagnostics:caret /I..\..\..\utility

SOURCES = main.cpp \
		  stats_aggregator.cpp \
		  stats_aggregator_test.cpp

HEADERS = stats_aggregator.h \
		  ..\..\..\utility\test_runner.h

all: step09.exe

.PHONY: step09.exe
step09.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Festep09.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
