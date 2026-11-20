#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class PersonalityInfoManager : public BaseManager {
	public:
		inline static PersonalityInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<PersonalityInfoManager>();
		}
	};
}
