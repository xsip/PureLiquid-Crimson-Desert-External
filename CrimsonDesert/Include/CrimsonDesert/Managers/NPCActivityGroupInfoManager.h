#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NPCActivityGroupInfoManager : public BaseManager {
	public:
		inline static NPCActivityGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<NPCActivityGroupInfoManager>();
		}
	};
}
