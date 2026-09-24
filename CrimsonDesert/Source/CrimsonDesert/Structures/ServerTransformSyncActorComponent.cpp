#include <CrimsonDesert/Structures/ServerTransformSyncActorComponent.h>
#include <GlobalData/Include.h>
using namespace Globals;


/*
pa::ServerTransformSyncActorComponent => + 30 = WilLReceiveFallDamage

CrimsonDesert.exe+21A1DB5:
1421A1DAF - 48 8B FA  - mov rdi,rdx
1421A1DB2 - 48 8B F1  - mov rsi,rcx
1421A1DB5 - 48 8B 01  - mov rax,[rcx] << pa::ServerTransformSyncActorComponent
1421A1DB8 - 48 8D 54 24 40  - lea rdx,[rsp+40]
1421A1DBD - FF 90 40010000  - call qword ptr [rax+00000140]



CrimsonDesert.exe+21A01A5 - C6 00 01              - mov byte ptr [rax],01 { 1 } // moves into FallDamageEnabled
*/


namespace CrimsonDesert {
	ServerTransformSyncActorComponent* ServerTransformSyncActorComponent::GetInstance() {
		return nullptr;
	}
#define FALL_DAMAGE_ENABLED_INSTRUCTION_PATTERN ""
	void ServerTransformSyncActorComponent::DisableFallDamage() {
		
		auto addr = reinterpret_cast<uintptr_t>(pExeMod->ScanMemory(FALL_DAMAGE_ENABLED_INSTRUCTION_PATTERN));
		if (!addr) {
			printf("Error locating FALL_DAMAGE_ENABLED_INSTRUCTION_PATTERN!\n");
			return;
		}
		
		bool bFallDamageEnabled = pProc->ReadDirect<bool>(addr + 0x2);
		if (bFallDamageEnabled) {
			printf("Fall damage enabled. Patching now..!\n");
			pProc->Write<bool>(addr + 0x2, false);
			return;
		}

		printf("Fall damage already disabled.\n");
	
	}
}