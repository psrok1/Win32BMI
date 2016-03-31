/*******
Undefined Instruction Emulator Framework
Kernel-mode driver for Windows 7 x86

psrok1 @ 2016
********/
#include "Driver.h"

// Get IDTR register
IDTR GetIDTR() {
	IDTR idtr;
	__asm {
		cli;		// Lock interrupts
		sidt idtr;
		sti;		// Unlock interrupts
	}
	return idtr;
}

// Get address of ISR for specified service
__forceinline ISR GetISR(PIDT desc_table, UINT8 serv_no) {
	_asm cli;
	ISR routine = (ISR)((desc_table[serv_no].offs_hi << 16) + desc_table[serv_no].offs_lo);
	_asm sti;
	return routine;
}

// Set address of ISR for specified service
__forceinline void SetISR(PIDT desc_table, UINT8 serv_no, ISR isr) {
	IDTDescriptor* desc = &(desc_table[serv_no]);
	UINT32 i_isr = (UINT32)isr;
	_asm cli; // lock
	desc->offs_hi = i_isr >> 16;
	desc->offs_lo = i_isr & 0xFFFF;
	_asm sti; // unlock
}

ISR old_israddr = NULL;

// UD exception handler
__declspec(naked) HookRoutine() {
	__asm {
		// Store context
		pushfd;
		pushad;

		// Get address to interrupt handlers chain
		mov esi, offset handlers_chain;
	handlerLookup:
		// Load address of interrupt handler
		mov edi, [esi];
		lea esi, [esi + 4];
		// If NULL - end of chain, go to system handler
		test edi, edi;
		jz unhandledExc;
		
		// Get fault address
		lea eax, [esp + 0x24];
		// Push pointer to stored context
		push esp;
		// Push fault address
		push eax;
		// Call handler
		call edi;
		
		// If handler refuses to handle exception: try next
		test eax, eax;
		jz handlerLookup;

		// If exception is handled:
		// Restore context and return from interrupt
		popad;
		popfd;
		iretd;
	unhandledExc:
		// Jumping to system handler
		popad
		popfd
		jmp old_israddr
	}
}

// Install custom UD handler
void InstallHookISR(ISR hook_addr) {
	IDTR idtr = GetIDTR();
	ISR israddr = GetISR(idtr.desc_table, 0x06);
	if (old_israddr == israddr)
		return; // Already hooked
	
	old_israddr = israddr;
	SetISR(idtr.desc_table, 0x06, hook_addr);
}

// Restore system UD handler
void UninstallHookISR() {
	if (!old_israddr)
		return; // Not hooked

	IDTR idtr = GetIDTR();
	SetISR(idtr.desc_table, 0x06, old_israddr);
	old_israddr = 0;
}

/********************/
/* Entry/Exit point */
/********************/

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UninstallHookISR();
	DbgPrint("Driver unloaded\n");
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	InstallHookISR(HookRoutine);
	DbgPrint("Driver loaded\n");
	return STATUS_SUCCESS;
}