#include "dll.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllexport)

DWORD WINAPI MessageBoxThread(LPVOID lpParam) {
	
  MessageBox(NULL, "Coucou, l'injection a bien fonctionné! ", "Dll-test", MB_SYSTEMMODAL | MB_OK | MB_ICONINFORMATION);
  
  return 0;
  
}

extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) { // execute cette partie du code que si la dll est chargée en mémoire 
 
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      CreateThread(NULL, NULL, MessageBoxThread, NULL, NULL, NULL); // appel sous la frome de thread de notre messagebox
      break;
      
    case DLL_THREAD_ATTACH:
    	
    case DLL_THREAD_DETACH:
    	
    case DLL_PROCESS_DETACH:
    	
      break;
    }
   return TRUE;
}
