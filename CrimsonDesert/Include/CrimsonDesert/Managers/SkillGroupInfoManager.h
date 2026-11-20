#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SkillGroupInfoManager : public BaseManager {
	public:
		inline static SkillGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SkillGroupInfoManager>();
		}
	};
}
