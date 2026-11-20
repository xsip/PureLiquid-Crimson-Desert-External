#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FieldInfoManager : public BaseManager {
	public:
		inline static FieldInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FieldInfoManager>();
		}
	};
}
