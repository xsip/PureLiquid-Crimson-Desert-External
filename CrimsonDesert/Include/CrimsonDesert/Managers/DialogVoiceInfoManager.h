#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DialogVoiceInfoManager : public BaseManager {
	public:
		inline static DialogVoiceInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DialogVoiceInfoManager>();
		}
	};
}
