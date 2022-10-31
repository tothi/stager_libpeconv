src_libpeconv = $(wildcard libpeconv/libpeconv/src/*.cpp)
obj_libpeconv = $(src_libpeconv:.cpp=.o)

INC = -Ilibpeconv/libpeconv/include
CFLAGS = -Wno-conversion-null -Wno-pointer-arith
LDFLAGS = -lntdll -static
LDFLAGS2 = -Ldist -l:libpeconv.a -lntdll -lws2_32 -static
# static linking is needed for cross compiling c++ to windows
# TODO: solve it dynamically in order to reduce stager size

CXX_x32 := i686-w64-mingw32-g++
CXX_x64 := x86_64-w64-mingw32-g++
AR_x86 := i686-w64-mingw32-ar
AR_x64 := x86_64-w64-mingw32-ar

all: dist/libpeconv.a dist/stager.exe

libpeconv/libpeconv/src/%.o: libpeconv/libpeconv/src/%.cpp
	$(CXX_x64) -c $(INC) ${CFLAGS} $< -o $@

dist/libpeconv.a: $(obj_libpeconv)
#	$(CXX_x64) -o dist/libpeconv.dll -shared $^ ${LDFLAGS}
	${AR_x64} rcs dist/libpeconv.a $^

dist/stager.exe: dist/libpeconv.a
	$(CXX_x64) stager/stager.cpp -o dist/stager.exe $(CFLAGS) $(INC) $(LDFLAGS2)

clean:
	rm -f libpeconv/libpeconv/src/*.o
	rm -f dist/libpeconv.dll dist/libpeconv.a dist/stager.exe
