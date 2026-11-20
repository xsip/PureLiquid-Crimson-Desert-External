#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NPCActivityInfoManager : public BaseManager {
	public:
		inline static NPCActivityInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<NPCActivityInfoManager>();
		}
	};
}
