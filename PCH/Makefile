ifeq ($(type),release)
  WX_COMMAND=`wx-config --cflags --debug=no`
  FLAGS=-O2 -DNDEBUG -fPIC
  TARGET=precompiled_header_release.h.gch
else
  WX_COMMAND=`wx-config --cflags --debug=no`
  FLAGS=-g -O0 -fPIC
  TARGET=precompiled_header_dbg.h.gch
endif

all: $(TARGET)

precompiled_header_dbg.h.gch: precompiled_header_dbg.h
	@echo -- Building PCH file precompiled_header_dbg.h.gch
	$(CXX) $(FLAGS) $(CFLAGS) $(WX_COMMAND) -c precompiled_header_dbg.h -o precompiled_header_dbg.h.gch

precompiled_header_release.h.gch: precompiled_header_release.h
	@echo -- Building PCH file precompiled_header_release.h.gch
	$(CXX) $(FLAGS) $(CFLAGS) $(WX_COMMAND) -c precompiled_header_release.h -o precompiled_header_release.h.gch

clean:
	$(RM) -f $(TARGET)