#include <Windows.h>

DWORD ret_address = 0x0052D303;

__declspec(naked) void codecave() {

	__asm {
		pushad
		mov dword ptr ds:[ebx+4], 8
		popad
		mov dword ptr ds:[0x102AE98], ebx

		jmp ret_address
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	DWORD old_protect;
	unsigned char* hook_location = (unsigned char*)0x0052D2FD;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		VirtualProtect((void*)hook_location, 5, PAGE_EXECUTE_READWRITE, &old_protect);
		*hook_location = 0xE9;
		*(DWORD*)(hook_location + 1) = (DWORD)&codecave - ((DWORD)hook_location + 5);
		*(hook_location + 5) = 0x90;
	}

	return true;
}
