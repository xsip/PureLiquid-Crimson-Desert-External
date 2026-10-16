#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;
// requires trade ( give goin for example ) to update coins
// combine with UIGamePlayControlCommonCoinage to set directly.
namespace CrimsonDesert {
	class UIGamePlayControlCommonCurrencyIndicator {
	private:


		class CUnknownData_4 {
		public:
			PROPERTY(pCurrencyIndicator, UIGamePlayControlCommonCurrencyIndicator*, 0x1F8);
		};

		class CUnknownData_3 {
		public:
			PROPERTY(pUnknown4, CUnknownData_4*, 0x158);
		};

		class CUnknownData_2 {
		public:
			PROPERTY(pUnknown3, CUnknownData_3*, 0x20);
		};
		class CUnknownData_1 {
		public:
			PROPERTY(pUnknown2, CUnknownData_2*, 0x40);
		};

		class CUnknownContainer {
		public:
			PROPERTY(pUnknown1, CUnknownData_1*, 0x4D8);
		};

		inline static UIGamePlayControlCommonCurrencyIndicator *pInstAddr = 0x0;
	public:
		PROPERTY(m_iCoins, int, 0xD8);
		
		static UIGamePlayControlCommonCurrencyIndicator* GetInstance();
	};
}