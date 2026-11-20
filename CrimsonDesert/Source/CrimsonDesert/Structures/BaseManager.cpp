#include <CrimsonDesert/Structures/BaseManager.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CrimsonDesert {
	
	ManagerListEntry* BaseManager::GetEntry(int i) {
		return reinterpret_cast<ManagerListEntry*>(pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x20) + (sizeof(ManagerListEntry) * i));
	}

}