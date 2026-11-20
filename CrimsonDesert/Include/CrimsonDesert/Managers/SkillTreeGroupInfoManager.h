#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SkillTreeGroupInfoManager : public BaseManager {
	public:
		inline static SkillTreeGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SkillTreeGroupInfoManager>();
		}
	};
}
