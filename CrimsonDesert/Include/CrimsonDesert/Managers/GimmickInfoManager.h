#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GimmickInfoManager : public BaseManager {
	public:
		inline static GimmickInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GimmickInfoManager>();
		}
	};
}
