#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class UIMapTextureInfoManager : public BaseManager {
	public:
		inline static UIMapTextureInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<UIMapTextureInfoManager>();
		}
	};
}
