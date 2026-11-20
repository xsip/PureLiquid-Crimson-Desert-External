#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SkillInfoManager : public BaseManager {
	public:
		inline static SkillInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SkillInfoManager>();
		}
	};
}
