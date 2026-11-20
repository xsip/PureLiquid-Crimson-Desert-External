#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class KeyMapSettingListInfoManager : public BaseManager {
	public:
		inline static KeyMapSettingListInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<KeyMapSettingListInfoManager>();
		}
	};
}
