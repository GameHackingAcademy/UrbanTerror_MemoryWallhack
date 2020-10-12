/*
A wallhack for Urban Terror 4.3.4 that reveals entities through walls by disabling depth-testing.

This is done by modifying each entity's render flag, which is responsible for determining how the entity should be rendered. 
By setting this value to the in-game value for disabled depth testing (0xD), entities will be drawn regardless of if it should visible. 
The code hooked is a mov instruction which occurs after ebx is loaded with a valid entity structure.

This must be injected into the Urban Terror process to work. One way to do this is to use a DLL injector. 
Another way is to enable AppInit_DLLs in the registry.

The offsets and method to discover them are discussed in the article at: https://gamehacking.academy/lesson/22
*/
#include <Windows.h>

DWORD ret_address = 0x0052D303;

// Our codecave that program execution will jump to. The declspec naked attribute tells the compiler to not add any function
// headers around the assembled code
__declspec(naked) void codecave() {
	// Asm blocks allow you to write pure assembly
	// In this case, we use it to save all the registers
	// Then set the entity's render value at [ebx+4] to disabled depth testing (0xD)
	// Then we restore the registers, recreate the original instruction, and jump back to the program code
	__asm {
		pushad
		mov dword ptr ds:[ebx+4], 0xD
		popad
		mov dword ptr ds:[0x102AE98], ebx

		jmp ret_address
	}
}

// When our DLL is attached, unprotect the memory at the code we wish to write at
// Then set the first opcode to E9, or jump
// Caculate the location using the formula: new_location - original_location+5
// Finally, since the original instructions totalled 6 bytes, NOP out the last remaining byte
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
