#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionRelationGroupInfoManager : public BaseManager {
	public:
		inline static FactionRelationGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionRelationGroupInfoManager>();
		}
	};
}
