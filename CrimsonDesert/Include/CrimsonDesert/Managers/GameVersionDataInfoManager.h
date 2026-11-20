#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameVersionDataInfoManager : public BaseManager {
	public:
		inline static GameVersionDataInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameVersionDataInfoManager>();
		}
	};
}
