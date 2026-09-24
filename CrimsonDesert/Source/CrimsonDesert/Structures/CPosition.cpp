#include <CrimsonDesert/Structures/CPosition.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CrimsonDesert {
	CPosition* CPosition::GetInstance() {
		if (!pInstAddr) {
			auto pPositionPatternResult = pExeMod->ScanMemory("C5 FA 7F 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4 ? E9 ? ? ? ? CC CC CC CC CC CC CC 41 B9");
			if (!pPositionPatternResult) {
				printf("Error finding  \"CPosition\"-Pattern\n");
				return nullptr;
			}

			pInstAddr = pExeMod->ResolveRIP(
				pPositionPatternResult,
				4,  
				8
			);

			if (!pInstAddr) {
				printf("Error finding  \"CPosition\"-Addr\n");
				return nullptr;
			}
		}
		return pProc->ReadDirect<CPosition*>(pInstAddr + 0x8);
	}
}