CLFLAGS = /Zi /EHsc /nologo /std:c++17 /Wall /WX /wd4514 /wd4626 /wd4820 /wd5027 /wd5045 /Od /diagnostics:caret

SOURCES = condition_parser_test.cpp \
		  condition_parser.cpp \
		  database.cpp \
		  date.cpp \
		  main.cpp \
		  node.cpp \
		  token.cpp

HEADERS = condition_parser.h \
		  database.h \
		  date.h \
		  node.h \
		  test_runner.h \
		  token.h

all: database.exe

.PHONY: database.exe
database.exe: $(SOURCES) $(HEADERS)
	cl $(CLFLAGS) /Fedatabase.exe $(SOURCES)

.PHONY: clean
clean:
	del *.obj *.ilk *.pdb *.exe
