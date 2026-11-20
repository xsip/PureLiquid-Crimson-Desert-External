#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AIMoveSpeedInfoManager : public BaseManager {
	public:
		inline static AIMoveSpeedInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AIMoveSpeedInfoManager>();
		}
	};
}
