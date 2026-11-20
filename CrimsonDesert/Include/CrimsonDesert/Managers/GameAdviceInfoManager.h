#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameAdviceInfoManager : public BaseManager {
	public:
		inline static GameAdviceInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameAdviceInfoManager>();
		}
	};
}
