PROG = $(patsubst %.cpp,%,$(wildcard *Test.cpp))

REQUIRES =

include $(shell smartbuildcfg --prefix)/share/smartmet/devel/makefile.inc

FLAGS = -std=$(CXX_STD) -Wall -W -fdiagnostics-color=$(GCC_DIAG_COLOR) -Wno-unused-parameter -DMYDEBUG

ifeq ($(TSAN), yes)
  FLAGS += -fsanitize=thread
endif
ifeq ($(ASAN), yes)
  FLAGS += -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize-address-use-after-scope
endif

CFLAGS = -DUNIX -O0 -g $(FLAGS)

INCLUDES += -I../timeseries

LIBS += ../libsmartmet-timeseries.so \
	-lsmartmet-spine \
	-lsmartmet-macgyver \
	$(REQUIRED_LIBS) \
	-lboost_date_time \
	-lboost_system \
	-lpthread

all: $(PROG)
clean:
	rm -f $(PROG) *~

test: $(PROG)
	@rm -rf /tmp/$$UID/bscachetest #Cache test uses this
	@rm -rf /tmp/$$UID/bscachetest2 #Cache test uses this
	@rm -rf /tmp/$$UID/bscachetest3 #Cache test uses this
	@mkdir -p /tmp/$$UID
	@echo Running tests:
	@ok=true; \
	for prog in $(PROG); do \
	./$$prog || ok=false; \
	done; \
	$$ok

$(PROG) : % : %.cpp Makefile ../libsmartmet-timeseries.so
	$(CXX) $(CFLAGS) -o $@ $@.cpp $(INCLUDES) $(LIBS)
