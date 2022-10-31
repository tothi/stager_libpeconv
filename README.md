# stager_libpeconv
A basic meterpreter protocol stager using the libpeconv library by hasherezade for reflective loading

Don't forget to include submodules (actually libpeconv) while checking out:

```
git clone --recurse-submodules https://github.com/tothi/stager_libpeconv
```

Compiling should work on Linux (with [mingw-w64](https://www.mingw-w64.org/)
cross-compiler toolset installed) using the provided [Makefile](./Makefile):

```
make
```

The stager.exe binary output is in the [dist](./dist) folder.

The stager server IP and port is hardcoded in the [stager/stager.cpp](./stager/stager.cpp) currently (IMPLANT_IP and IMPLANT_PORT definitions).
