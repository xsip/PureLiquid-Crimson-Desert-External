#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AIActionAttributeInfoManager : public BaseManager {
	public:
		inline static AIActionAttributeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AIActionAttributeInfoManager>();
		}
	};
}
