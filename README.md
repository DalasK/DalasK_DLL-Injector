# DalasK_DLL-Injector
FR : Injecteur de dll se présentant sous la forme d'une console demandant à l'utilisateur de rentrer un processus ciblé et le chemin d'accès de la dll à injecter.

# Qu'est-ce qu'un injecteur de dll ? 
Il s'agit d'un programme (ou d'une action/attaque) consistant à injecter et exécuter du code (une bibliothèque de liens dynamiques [dll]) dans l'espace d'adressage d'un processus en cours d'exécution. Cela permet de modifier le comportement de ce programme, augmenter des droits d'autorité dessus ou bien ajouter des fonctionnalités externes. En outre, ce type d'injecteur peut être utilisé pour tricher dans un jeu vidéo (en chargeant une dll externe afin d'obtenir des avantages/privilèges) mais aussi pour nuire à un système informatique en injectant un code malveillant dans un processus essentiel à Windows. 

# Comment ça fonctionne ?
En fait, l'injection se déroule en 5 étapes  :
1. Après l'entrée d'un processus ciblé par l'utilisateur, l'injecteur commence repérer ce processus ou la dll sera injectée (OpenProcess).

2. Il alloue ensuite un espace de mémoire dans la zone d'adressage du processus ciblé (la taille de cet espace doit être suffisamment grande pour supporter le code à injecter). (VirtualAlloc) 

3. Une fois cette espace alloué, l'injecteur demande à l'utilisateur le chemin d'accès de sa dll et l'écrit ensuite vers l'espace d'adressage (WriteProcessMemory).

4. Il recherche ensuite la fonction LoadLibraryA qui permet de charger une DLL avec GetProcAddress().

5. L'injecteur démarre un thread distant en y appellant la fonction LoadLibraryA afin de charger le code qui s'éxcutera au sein du processus (CreateRemoteThread). 

Notes /!\: 
- Veillez à bien vérifier que votre DLL, l'injecteur et le programme ciblé ont tous les même architecture (x64 ou x86).
- Je n'incite aucunement au piratage d'application et ne suis pas responsable de ce que vous ferez de cet injecteur (allez-y molo quoi). 

# -------------------------------------------------------------------------

EN: Dll injector in the form of a console asking the user to enter a targeted process and the path of the dll to inject.

# What is a dll injector ? 
It is a program (or an action/attack) consisting in injecting and executing code (a dynamic link library [dll]) in the address space of a running process. This allows to modify the behavior of this program, to increase authority rights on it or to add external features. Moreover, this type of injector can be used to cheat in a video game (by loading an external dll in order to obtain advantages/privileges) but also to harm a computer system by injecting malicious code into a process essential to Windows. 

# How does it work ?
Actually, the injection takes place in 5 steps:
1. After the entry of a process targeted by the user, the injector starts to locate this process where the dll will be injected (OpenProcess).

2. It then allocates a memory space in the address area of the targeted process (the size of this space must be large enough to support the code to be injected). (VirtualAlloc) 

3. Once this space is allocated, the injector asks the user for the path of his dll and then writes it to the address space (WriteProcessMemory).

4. it then looks for the LoadLibraryA function which allows to load a DLL with GetProcAddress().

5. The injector starts a remote thread by calling the LoadLibraryA function in order to load the code that will be executed within the process (CreateRemoteThread). 

# Notes /!\: 
- Make sure that your DLL, the injector and the targeted program have the same architecture (x64 or x86)
- I do not incite any application hacking and I am not responsible of what you will do with this injector (just go easy lol). 
