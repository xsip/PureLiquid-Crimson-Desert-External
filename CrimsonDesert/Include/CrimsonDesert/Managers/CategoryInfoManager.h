#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class CategoryInfoManager : public BaseManager {
	public:
		inline static CategoryInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<CategoryInfoManager>();
		}
	};
}
