#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DailyRoutineInfoManager : public BaseManager {
	public:
		inline static DailyRoutineInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DailyRoutineInfoManager>();
		}
	};
}
