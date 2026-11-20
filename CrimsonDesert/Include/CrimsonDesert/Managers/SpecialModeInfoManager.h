#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SpecialModeInfoManager : public BaseManager {
	public:
		inline static SpecialModeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SpecialModeInfoManager>();
		}
	};
}
