#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>
#include <functional>
#include <string>
#include <map>

namespace CrimsonDesert {
	class ManagerLoopDumper {
	private:
		inline static bool bOffsetsCollected = false;
		inline static std::map<std::string, uintptr_t> managerOffsetMap{};
		static void CollectOffsets();
		static void ForEachManager(
			const std::function<void(
				const std::string& className,
				uintptr_t pOffset,
				uintptr_t functionLocation
				)>& callback
		);
	public:
		static void Dump();
		static void GenerateFunctionRenameList();
		static void Generate(bool bStatic = false);
		static uintptr_t GetOffsetFor(std::string);
	};
}