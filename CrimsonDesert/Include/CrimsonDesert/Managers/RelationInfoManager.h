#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class RelationInfoManager : public BaseManager {
	public:
		inline static RelationInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<RelationInfoManager>();
		}
	};
}
