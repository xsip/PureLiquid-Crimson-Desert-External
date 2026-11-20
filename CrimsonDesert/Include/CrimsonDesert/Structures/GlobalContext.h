#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	

	class UIGamePlayControlCommonCurrencyIndicator2 {
	public:
		PROPERTY(m_iCoins, int, 0xD8);
	};

	class UIGamePlayControlCommonCoinage2 {
	public:
		PROPERTY(m_iCoins, int, 0xD8);
	};


	class UIGamePlayControlCommon_WorldMapIcon {
	private:
		PROPERTY(m_vPosition, Vector3, 0xE0);
	public:

		inline Vector3 GetWorldPosition(bool bOriginal = false) {
			Vector3 vPosition = pProc->ReadDirect<Vector3>(reinterpret_cast<uintptr_t>(this) + 0xE0);
			if (bOriginal)
				return vPosition;
			vPosition.x += 10000;
			vPosition.z += 3000;
			return vPosition;
		};

		inline std::string GetIconName() {
			return pProc->ReadString(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x238)));
		}
	};
	class HoveringItemContainer {
	public:
		PROPERTY(m_pIcon, UIGamePlayControlCommon_WorldMapIcon*, 0x0);
	};
	class UIGamePlayControlRootWorldMap {
	public:

	public:
		PROPERTY(m_hoverdMapIconContainer, HoveringItemContainer*, 0x338);
		PROPERTY(m_bIsHoveringMapItem, bool, 0x340);
	};

	class GlobalContext {
	private:
		inline static GlobalContext* pInstAddr = 0x0;
		UIGamePlayControlCommonCoinage2* GetUIGamePlayControlCommonCoinage2(); // Need to implement
	public:
		static GlobalContext* GetInstance();
		UIGamePlayControlRootWorldMap* GetUIGamePlayControlRootWorldMap();
	
		UIGamePlayControlCommonCurrencyIndicator2* GetUIGamePlayControlCommonCurrencyIndicator();

	};
}