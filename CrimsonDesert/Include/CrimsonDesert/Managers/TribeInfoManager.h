#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class TribeInfoManager : public BaseManager {
	public:
		inline static TribeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<TribeInfoManager>();
		}
	};
}
