# stager_libpeconv
A basic meterpreter protocol stager using the libpeconv library by hasherezade for reflective loading

Don't forget to include submodules (actually libpeconv) while checking out:

```
git clone --recurse-submodules https://github.com/tothi/stager_libpeconv
```

Compiling should work on Linux (with [mingw-w64](https://www.mingw-w64.org/)
cross-compiler toolset installed) using the provided [Makefile](./Makefile)
(config params for make are mandatory):

```
make IMPLANT_IP=192.168.56.1 IMPLANT_PORT=8889
```

The stager.exe binary output is in the [dist](./dist) folder.

