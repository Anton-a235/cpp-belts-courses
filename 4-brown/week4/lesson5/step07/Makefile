CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4267 /wd4365 /wd4514 /wd4625 /wd4626 \
		  /wd4710 /wd4711 /wd4820 /wd5026 /wd5027 /wd5045 /Od /diagnostics:caret /I..\..\..\utility

SOURCES = test.cpp \

HEADERS = booking.h \
		  new_booking_providers.h \
		  ..\..\..\utility\test_runner.h

all: test.exe

.PHONY: test.exe
test.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Fetest.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
