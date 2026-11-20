#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class PartPrefabDyeTexturePalleteInfoManager : public BaseManager {
	public:
		inline static PartPrefabDyeTexturePalleteInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<PartPrefabDyeTexturePalleteInfoManager>();
		}
	};
}
