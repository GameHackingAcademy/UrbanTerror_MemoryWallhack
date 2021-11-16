# Urban Terror Memory Wallhack
Referenced in https://gamehacking.academy/lesson/5/2.

A wallhack for Urban Terror 4.3.4 that reveals entities through walls by disabling depth testing.

This is done by modifying each entity's render flag, which is responsible for determining how the entity should be rendered. By setting this value to the in-game value for disabled depth testing (0xD), entities will be drawn whether or not they should be visible. The code hooked is a mov instruction, which occurs after ebx is loaded with a valid entity structure.

This must be injected into the Urban Terror process to work. One way to do this is to use a DLL injector. Another way is to enable AppInit_DLLs in the registry.
