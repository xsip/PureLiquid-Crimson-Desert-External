#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MiniGameDataInfoManager : public BaseManager {
	public:
		inline static MiniGameDataInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MiniGameDataInfoManager>();
		}
	};
}
