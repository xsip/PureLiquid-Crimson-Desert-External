#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SkillTreeInfoManager : public BaseManager {
	public:
		inline static SkillTreeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SkillTreeInfoManager>();
		}
	};
}
