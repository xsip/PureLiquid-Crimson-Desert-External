#include <CrimsonDesert/Structures/GlobalContext.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CrimsonDesert {
	GlobalContext* GlobalContext::GetInstance() {
		if (!pInstAddr) {
			auto pGlobalCtxRes = pExeMod->ScanMemory("48 8B 1D ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8B 03 48 8B CB FF 50 ? 90 41 0F B7 06 66 89 44 24 ? 89 6C 24 ? 48 89 74 24 ? 48 8B 84 24");
			if (!pGlobalCtxRes) {
				printf("Error finding  \"GlobalContext\"-Pattern\n");
				return nullptr;
			}

			pInstAddr = pProc->ReadDirect<GlobalContext*>(pExeMod->ResolveRIP(
				pGlobalCtxRes,
				3,
				7
			));

		}
		return pInstAddr;

	}

	UIGamePlayControlRootWorldMap* GlobalContext::GetUIGamePlayControlRootWorldMap() {
		static bool bInitialized = false;
		static UIGamePlayControlRootWorldMap* pUIGamePlayControlRootWorldMap = nullptr;
		if (!bInitialized) {
			auto tmp = pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x850);

			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlRootWorldMap;
			}

			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x40);


			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlRootWorldMap;
			}

			pUIGamePlayControlRootWorldMap = pProc->ReadDirect<UIGamePlayControlRootWorldMap*>(tmp);
			bInitialized = true;
		}

		return pUIGamePlayControlRootWorldMap;
	}
	UIGamePlayControlCommonCoinage2* GlobalContext::GetUIGamePlayControlCommonCoinage2()
	{
		static UIGamePlayControlCommonCoinage2* pUIGamePlayControlCommonCoinage2 = nullptr;


		static bool bInitialized = false;
		
		if (!bInitialized) {
			auto tmp = pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x4D8);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCoinage2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x40);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCoinage2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x20);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCoinage2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x158);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCoinage2;
			}
			pUIGamePlayControlCommonCoinage2 = pProc->ReadDirect<UIGamePlayControlCommonCoinage2*>(tmp + 0x1F8);

			bInitialized = true;
		}


		return pUIGamePlayControlCommonCoinage2;
	}
	UIGamePlayControlCommonCurrencyIndicator2* GlobalContext::GetUIGamePlayControlCommonCurrencyIndicator()
	{
		static bool bInitialized = false;
		static UIGamePlayControlCommonCurrencyIndicator2* pUIGamePlayControlCommonCurrencyIndicator2 = nullptr;
		
		if (!bInitialized) {
			auto tmp = pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x4E8);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCurrencyIndicator2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x40);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCurrencyIndicator2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x20);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCurrencyIndicator2;
			}
			tmp = pProc->ReadDirect<uintptr_t>(tmp + 0x158);
			if (!tmp) {
				bInitialized = true;
				return pUIGamePlayControlCommonCurrencyIndicator2;
			}
			pUIGamePlayControlCommonCurrencyIndicator2 = pProc->ReadDirect<UIGamePlayControlCommonCurrencyIndicator2*>(tmp + 0x1F8);

			bInitialized = true;
		}
		
		
		return pUIGamePlayControlCommonCurrencyIndicator2;
	}
}