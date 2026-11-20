#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class UIFilterGroupInfoManager : public BaseManager {
	public:
		inline static UIFilterGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<UIFilterGroupInfoManager>();
		}
	};
}
