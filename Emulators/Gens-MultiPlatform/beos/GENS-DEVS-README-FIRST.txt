January 28, 2005 - c0d3h4x0r@hotmail.com

This directory contains the complete BeOS port, which was based on the original Gens for Windows source tree.  It has not yet been merged into the new Gens multi-platform structure.

This tree needs to be diff'ed against the ..\common tree.  Any duplicated files with differences need to be applied to the files in the ..\common tree in a way that will not disrupt other platform-specific builds, and then the duplicated files should be removed from here.


The following comments were written by whomever did the BeOS port.

-------

The Assembly portions of gens have been modified to work on BeOS and should also work on Linux, gens uses fastcall calling semantics normally, the modified sections of code are marked with %ifdef __GCC2, the asm code has been modified in such a way that the code should build normally on Win32 too, hopefully my modifications will be accepted into the gens source tree so the authors of gens can continue working on their port normally for Windows and also making ports to Linux/BeOS possible. The c/cpp files have been modified and the modifications in these files are marked with #ifdef __PORT__, there is also the port.h file which has the portable definitions.
e.g ym2612.c
#ifdef __PORT__
int _YM2612_Reset(void) __attribute__ (( alias ("YM2612_Reset")));
int _YM2612_Read(void) __attribute__ ((alias ("YM2612_Read")));
int _YM2612_Write(unsigned char adr, unsigned char data) __attribute__ ((alias ("YM2612_Write")));
#endif

vc++ fastcall = first arg<ecx>, second arg<edx>
gcc fastcall = first arg<eax>, second arg<edx>

I have been unable to get in contact with the gens author, at this time.

Caz