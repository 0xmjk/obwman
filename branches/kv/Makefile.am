AM_YFLAGS=-d

CC=@CXX@

bin_PROGRAMS = obwman

obwman_SOURCES = configuration.cc configuration.h control_parameters.cc \
control_parameters.h main.cc conf-parser.y conf-lexer.l \
parser.cc parser.h util.h util.cc  call.cc call.h 

obwman_LDADD = @LEXLIB@

conf-lexer.o: conf-parser.h

