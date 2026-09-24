#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class CPosition {
	private:

		class CPosContainer_1 {
		public:
			PROPERTY(m_vLocalPos, Vector, 0x90);
		};
		inline static uintptr_t pInstAddr = 0x0;
		PROPERTY(pPosContainer, CPosContainer_1*, 0x8);

	public:



		__forceinline Vector _internal_GetPosition() {
			auto c = pPosContainer;
			if (!c)
				return { 0.0f,0.0f,0.0f };
			return c->m_vLocalPos;
		};

		__forceinline void _internal_SetPosition(Vector val) {
			auto c = pPosContainer;
			if (!c) {
				printf("Can't set vLocalPos! \"pPosContainer\" = 0x0!!\n");
				return;
			}
			c->m_vLocalPos = val;
		};
		__declspec(property(get = _internal_GetPosition, put = _internal_SetPosition)) Vector vLocalPos;

		static CPosition* GetInstance();


	};
}