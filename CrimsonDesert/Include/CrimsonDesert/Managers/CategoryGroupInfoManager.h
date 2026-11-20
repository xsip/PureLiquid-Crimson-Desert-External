#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class CategoryGroupInfoManager : public BaseManager {
	public:
		inline static CategoryGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<CategoryGroupInfoManager>();
		}
	};
}
