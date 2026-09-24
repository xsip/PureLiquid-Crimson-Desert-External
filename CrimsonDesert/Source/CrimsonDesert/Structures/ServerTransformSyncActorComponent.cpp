#include <CrimsonDesert/Structures/ServerTransformSyncActorComponent.h>
#include <GlobalData/Include.h>
using namespace Globals;

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