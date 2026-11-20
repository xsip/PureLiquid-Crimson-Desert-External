#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MaterialBloodDecalInfoManager : public BaseManager {
	public:
		inline static MaterialBloodDecalInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MaterialBloodDecalInfoManager>();
		}
	};
}
