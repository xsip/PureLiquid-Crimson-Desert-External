#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class UISocialActionInfoManager : public BaseManager {
	public:
		inline static UISocialActionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<UISocialActionInfoManager>();
		}
	};
}
