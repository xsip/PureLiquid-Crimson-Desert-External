#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class EffectInfoManager : public BaseManager {
	public:
		inline static EffectInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<EffectInfoManager>();
		}
	};
}
