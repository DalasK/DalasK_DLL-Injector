#define MAX_PATH 1000
#include <Windows.h>
#include <TlHelp32.h> //On va en avoir besoin pour utiliser la fonction CreateToolhelp32Snapshot                                         
#include <iostream>
#include <string>

using namespace std;

bool injection(DWORD id_de_processus, char* dllacces) // le processus d'injection 
{
	bool retour = false;
	cout << "\n" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // Dans la suite du code, j'utiliserais beaucoup cette fonction pour l'esthétique (gris)
	cout << "Attachement du processus ............" << endl;
	// On va définir un handle vers le processus où nous allons créer un nouveau thread et y injecter la dll 
	HANDLE handletargetprocess = OpenProcess(PROCESS_ALL_ACCESS, false, id_de_processus); // Ouvre un processus local depuis son ID  

	//la dll à injecter dans un processus est placé dans le système cible

	if (handletargetprocess)
	{
		Sleep(25);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // (vert)
		cout << "Operation reussie!" << endl;
		cout << "\n" << endl; //Saut à la ligne simple

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		cout << "Allocation de la memoire en cours ........" << endl;

		LPVOID dllpathmemoryaddress = VirtualAllocEx(handletargetprocess, 0, strlen(dllacces), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		// alloue un espace de mémoire à l’intérieur de l’espace d’adressage du processus. La taille de cette région de mémoire doit être aussi grande pour correspondre au nom de la DLL qu’elle supportera 
		if (dllpathmemoryaddress)
		{
			Sleep(50);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			cout << "Operation reussie!" << endl;
			cout << "\n" << endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
			cout << "Ecriture dans le processus de memoire.......... " << endl;
			if (WriteProcessMemory(handletargetprocess, dllpathmemoryaddress, dllacces, strlen(dllacces), NULL))
			{
				//Le chemin d'accès où se trouve notre dll est copié dans l'adresse mémoire du processus
				Sleep(100);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				cout << "Operation reussie!" << endl;

				cout << "\n" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				cout << "Chargement de la librairie ......" << endl;
				//recherche la fonction LoadLibraryA qui permet de charger une DLL avec GetProcAddres : 
				LPVOID 	librairies_adresses = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"); // pour obtenir l’adresse de la fonction LoadLibraryA dans la bibliothèque kernel32.dll
				// Chaque programme utilise la bibliothèque kernel32.dll, donc la meilleure façon d’injecter une DLL dans l’espace d’adressage du processus ciblé est de rechercher la fonction LoadLibraryA et de l’appeler
				// (LoadLibraryA permettra de charger notre dll en mémoire au sein du processus ciblé) 
				Sleep(200);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				cout << "Operation reussie!" << endl;
				cout << "\n" << endl;

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				cout << "Creation du thread distant pour la dll....." << endl;
				HANDLE remotethread = CreateRemoteThread(handletargetprocess, NULL, 0, (LPTHREAD_START_ROUTINE)librairies_adresses, dllpathmemoryaddress, 0, NULL);
				// J'ai utilisé CreateRemoteThread pour démarrer un nouveau thread distant dans l’espace d’adressage d’un processus
				//permettant d'appeller la fonction LoadLibraryA dans l'espace d'adresses du processus ciblé pour y injecter la dll 

				if (remotethread)
				{
					retour = true;
				}
			}
		}

	}
	Sleep(250);
	CloseHandle(handletargetprocess); //ferme le handle 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	cout << "Operation reussie!" << endl;
	cout << "\n" << endl;

	Sleep(500);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "Injection de la DLL (" << dllacces << ") dans le processus (" << id_de_processus << ")....." << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	cout << "La dll a bien ete injectee! " << endl;
	return retour;
}

DWORD ProcessusID(LPCTSTR ProcessName) // On récupére l'ID du processus par son nom. Le but de cette partie du code est de retourner un message d'erreur si le nom du processus entré n'est pas en cours d'éxécution 
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Récupère instantanément un processus ciblé, les modules & threads qui lui sont associés
	pt.dwSize = sizeof(PROCESSENTRY32); // renvoie la quantité de mémoire occupée par pt

	if (Process32First(hsnap, &pt))
	{
		do {

			if (!lstrcmpi(pt.szExeFile, ProcessName))
			{
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // (rouge)
	cout << "\n" << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "| [Erreur] Impossible de trouver l'ID de ce processus!| " << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "\n" << endl;

	return 0;
}
// static string szString ?
LPCTSTR SzToLPCTSTR(char* szString) // le chemin d'accès de notre DLL ciblé qui pourra être copier et entré dans la console
{
	LPTSTR  lpszRet;
	size_t  size = strlen(szString) + 1;

	lpszRet = (LPTSTR)malloc(MAX_PATH);
	mbstowcs_s(NULL, lpszRet, size, szString, _TRUNCATE); // Convertit une séquence de caractères -> séquence de caractères larges.

	return lpszRet;
}

int main()
{
	SetConsoleTitleA("Dall4sK Injector"); //Le titre de la fenêtre 
	HWND hwnd = GetConsoleWindow();         //Le handle (la poignée) de la console
	MoveWindow(hwnd, 0, 0, 700, 500, TRUE); //redimmensionnement de la console
	char proc[MAX_PATH];
	char* szDllPath = (char*)malloc(MAX_PATH); // le chemin d'accès de notre dll qui sera entré avant l'injection 
	LPCTSTR Process = NULL;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); // (bleu foncé)
	cout << "=================================================================================" << endl;
	cout << "//                              DalasK Injector                                //" << endl;
	cout << "=================================================================================" << endl;
	cout << "\n ";

	/* le code qui suit se déroule comme cela :
	   - attente d'une entrée utilisateur (nom de processus) => si ce dernier n'est pas en cours d'éxécution -> message d'erreur
	   -                                                     => sinon partie opératoire de l'injection (cf. bool injection)
	   */
	while (true)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		
		cout << "Entrez le nom d'un processus en cours de fonctionnement: ";

		cin >> proc; // Récupère le processus 
		Process = SzToLPCTSTR(proc);  // processus introuvable = retour d'erreur
		DWORD id = ProcessusID(Process);
		if (id == 0)
		{
		}
		else // Entrées + injection 
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16); // (surlignage bleu)
			cout << "\n" << endl;
			cout << "L'ID du processus est [" << id << "] ......" << endl;
			cout << "\n" << endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			cout << "Entrez le chemin d'acces de votre DLL: ";
			cin >> szDllPath; // récupère le chemin d'accès de notre dll

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); //(jaune)
				cout << "\n" << endl;

			cout << "Le chemin d'acces de votre DLL est : " << szDllPath << endl;
			cout << "\n" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // (violet) 
			cout << "Injection de la DLL (" << szDllPath << ") dans le processus en cours de fonctionnement (" << proc << ")........." << endl;
			injection(id, szDllPath);
			//Notre DLL est exécutée et commence à exécuter ses processus !
			cout << "\n" << endl;
		}
	}

	system("pause");
	return 0;
}
