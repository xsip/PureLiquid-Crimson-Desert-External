#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameEventHandlerInfoManager : public BaseManager {
	public:
		inline static GameEventHandlerInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameEventHandlerInfoManager>();
		}
	};
}
