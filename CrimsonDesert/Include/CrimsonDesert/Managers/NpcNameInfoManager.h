#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NpcNameInfoManager : public BaseManager {
	public:
		inline static NpcNameInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<NpcNameInfoManager>();
		}
	};
}
