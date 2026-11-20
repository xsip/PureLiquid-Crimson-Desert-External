/*
#include <CrimsonDesert/Structures/NpcInfoManager.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CrimsonDesert {
	NpcInfoManager* NpcInfoManager::GetInstance() {
		return pProc->ReadDirect<NpcInfoManager*>(pExeMod->GetAddr() + 0x724E118);
		//NpcInfoManager =  pProc->ReadDirect<NpcInfoManager*>(pExeMod->GetAddr() + 0x724E118);
		//CharacterInfoManager =  pProc->ReadDirect<NpcInfoManager*>(pExeMod->GetAddr() + 0x7246310);
	}
	NpcListEntry* NpcInfoManager::GetNpc(int i) {
		return reinterpret_cast<NpcListEntry*>(pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x20) + (sizeof(NpcListEntry) * i));
	}
	void ForceEntityEnricher(int i) {

	}
}
*/