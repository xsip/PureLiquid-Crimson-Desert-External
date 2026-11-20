#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class QuickTimeEventInfoManager : public BaseManager {
	public:
		inline static QuickTimeEventInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<QuickTimeEventInfoManager>();
		}
	};
}
