#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GimmickGroupInfoManager : public BaseManager {
	public:
		inline static GimmickGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GimmickGroupInfoManager>();
		}
	};
}
