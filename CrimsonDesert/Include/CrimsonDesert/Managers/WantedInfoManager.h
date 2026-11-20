#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class WantedInfoManager : public BaseManager {
	public:
		inline static WantedInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<WantedInfoManager>();
		}
	};
}
