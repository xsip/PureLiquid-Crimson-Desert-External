#include <CrimsonDesert/Structures/UIGamePlayControlCommonCurrencyIndicator.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CrimsonDesert {
	UIGamePlayControlCommonCurrencyIndicator* UIGamePlayControlCommonCurrencyIndicator::GetInstance() {
		if (!pInstAddr) {
			auto pPositionPatternResult = pExeMod->ScanMemory("48 8B 1D ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8B 03 48 8B CB FF 50 ? 90 41 0F B7 06 66 89 44 24 ? 89 6C 24 ? 48 89 74 24 ? 48 8B 84 24");
			if (!pPositionPatternResult) {
				printf("Error finding  \"UIGamePlayControlCommonCurrencyIndicator\"-Pattern\n");
				return nullptr;
			}

			auto pInst = pExeMod->ResolveRIP(
				pPositionPatternResult,
				3,
				7
			);

			if (!pInst) {
				printf("Error finding  \"UIGamePlayControlCommonCurrencyIndicator\"-Addr\n");
				return nullptr;
			}

			auto c = pProc->ReadDirect<CUnknownContainer*>(pInst);
			if (!c)
				return nullptr;

			auto pUnknown1 = c->pUnknown1;

			if (!pUnknown1)
				return nullptr;

			auto pUnknown2 = pUnknown1->pUnknown2;

			if (!pUnknown2)
				return nullptr;

			auto pUnknown3 = pUnknown2->pUnknown3;

			if (!pUnknown3)
				return nullptr;

			auto pUnknown4 = pUnknown3->pUnknown4;

			if (!pUnknown4)
				return nullptr;

			auto pCurrencyIndicator = pUnknown4->pCurrencyIndicator;

			if (!pCurrencyIndicator)
				return nullptr;

			return pInstAddr = pCurrencyIndicator;
		}
		return pInstAddr;

	}
}