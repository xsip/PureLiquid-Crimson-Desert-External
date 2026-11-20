#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class PatternDescriptionInfoManager : public BaseManager {
	public:
		inline static PatternDescriptionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<PatternDescriptionInfoManager>();
		}
	};
}
