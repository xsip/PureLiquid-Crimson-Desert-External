#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class BitmapPositionInfoManager : public BaseManager {
	public:
		inline static BitmapPositionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<BitmapPositionInfoManager>();
		}
	};
}
