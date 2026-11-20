#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MaterialMatchInfoManager : public BaseManager {
	public:
		inline static MaterialMatchInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MaterialMatchInfoManager>();
		}
	};
}
