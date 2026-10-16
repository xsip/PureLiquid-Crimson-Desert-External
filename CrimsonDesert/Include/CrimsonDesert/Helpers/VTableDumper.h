#include <GlobalData/Include.h>
using namespace Globals;
#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>

#pragma comment(lib, "dbghelp.lib")

namespace CrimsonDesert {
    class VTableDumper {
    private:
        struct Module
        {
            uintptr_t base = 0;
            size_t size = 0;
        };

        struct COLInfo
        {
            uintptr_t address = 0;
            uintptr_t typeDescriptor = 0;
        };

        struct VTableInfo
        {
            uintptr_t address = 0;
            COLInfo col;
        };

        template <typename T>
        inline static bool readMemory(
            uintptr_t address,
            T& value
        )
        {
            return pProc->Read(address, &value);
        }

        inline static std::string readString(
            uintptr_t address,
            size_t maxLength = 1024
        )
        {
            std::string result;

            for (size_t i = 0; i < maxLength; ++i)
            {
                char c = 0;

                if (!readMemory(
                    address + i,
                    c
                ))
                {
                    break;
                }

                if (c == '\0')
                    break;

                result += c;
            }

            return result;
        }

        inline static std::vector<uint8_t> readModule(
            const Module& module
        )
        {
            std::vector<uint8_t> buffer(
                module.size
            );

            SIZE_T bytesRead = 0;

            if (!ReadProcessMemory(
                pProc->m_hProc,
                reinterpret_cast<void*>(
                    module.base
                    ),
                buffer.data(),
                buffer.size(),
                &bytesRead
            ))
            {
                buffer.clear();
                return buffer;
            }

            buffer.resize(
                bytesRead
            );

            return buffer;
        }



        inline static std::string demangle(
            const std::string& decorated
        )
        {
            if (decorated.empty())
                return "<unknown>";

            char buffer[4096]{};

            DWORD length =
                UnDecorateSymbolName(
                    decorated.c_str(),
                    buffer,
                    sizeof(buffer),
                    UNDNAME_NAME_ONLY
                );

            if (length)
                return buffer;

            return decorated;
        }

        inline static std::string getTypeName(
            uintptr_t typeDescriptor
        )
        {
            // x64 MSVC:
            //
            // TypeDescriptor:
            //
            // +00 vfptr
            // +08 spare
            // +10 decorated name

            std::string decorated =
                readString(
                    typeDescriptor + 0x10
                );

            return demangle(
                decorated
            );
        }


        inline static bool inModule(
            uintptr_t address,
            const Module& module,
            size_t size = 1
        )
        {
            return
                address >= module.base &&
                address + size <=
                module.base + module.size;
        }


        inline static bool isExecutable(
            uintptr_t address
        )
        {
            MEMORY_BASIC_INFORMATION mbi{};

            if (VirtualQueryEx(
                pProc->m_hProc,
                reinterpret_cast<void*>(address),
                &mbi,
                sizeof(mbi)
            ) != sizeof(mbi))
            {
                return false;
            }

            if (mbi.State != MEM_COMMIT)
                return false;

            if (mbi.Protect & PAGE_GUARD)
                return false;

            if (mbi.Protect & PAGE_NOACCESS)
                return false;

            return
                (mbi.Protect & PAGE_EXECUTE) ||
                (mbi.Protect & PAGE_EXECUTE_READ) ||
                (mbi.Protect & PAGE_EXECUTE_READWRITE) ||
                (mbi.Protect & PAGE_EXECUTE_WRITECOPY);
        }


        inline static std::vector<VTableInfo> findVTables(
            const Module& module,
            const std::vector<uint8_t>& image,
            const COLInfo& col
        )
        {
            std::vector<VTableInfo> result;

            uint64_t target =
                static_cast<uint64_t>(
                    col.address
                    );

            for (
                size_t offset = 0;
                offset + sizeof(uint64_t) <= image.size();
                offset += 8
                )
            {
                uint64_t value = 0;

                std::memcpy(
                    &value,
                    image.data() + offset,
                    sizeof(value)
                );

                if (value != target)
                    continue;

                uintptr_t vtable =
                    module.base +
                    offset +
                    8;

                if (!inModule(
                    vtable,
                    module,
                    sizeof(uintptr_t)
                ))
                {
                    continue;
                }

                uintptr_t firstFunction = 0;

                if (!readMemory(
                    vtable,
                    firstFunction
                ))
                {
                    continue;
                }

                if (!isExecutable(
                    firstFunction
                ))
                {
                    continue;
                }

                result.push_back({
                    vtable - module.base,
                    col
                    });
            }

            return result;
        }


        inline static void dumpVTable(
            std::ostream& out,
            const VTableInfo& info,
            const Module& module
        )
        {
            std::string decorated =
                readString(
                    info.col.typeDescriptor + 0x10
                );

            std::string className =
                demangle(decorated);

            out
                << "\n"
                << "============================================================\n";

            out
                << "VTABLE : CrimsonDesert.exe + 0x"
                << std::hex
                << std::uppercase
                << info.address
                << "\n";

            out
                << "CLASS  : "
                << className
                << "\n";

            out
                << "TYPE   : "
                << decorated
                << "\n";

            out
                << "COL    : 0x"
                << std::hex
                << info.col.address
                << "\n";

            out
                << "TD     : 0x"
                << info.col.typeDescriptor
                << "\n";

            out
                << "============================================================\n";

            constexpr size_t MAX_FUNCTIONS = 512;

            for (
                size_t index = 0;
                index < MAX_FUNCTIONS;
                ++index
                )
            {
                uintptr_t function = 0;

                if (!readMemory(
                    info.address +
                    index * sizeof(uintptr_t),
                    function
                ))
                {
                    break;
                }

                if (function == 0)
                    break;

                if (!isExecutable(function))
                    break;

                out
                    << "["
                    << std::dec
                    << std::setw(3)
                    << index
                    << "]  0x"
                    << std::hex
                    << std::uppercase
                    << function;

                if (inModule(
                    function,
                    module
                ))
                {
                    out
                        << "    +0x"
                        << std::hex
                        << (
                            function -
                            module.base
                            );
                }

                out << "\n";
            }
        }

        inline static
            std::vector<COLInfo> findCOLs(
                const Module& module,
                const std::vector<uint8_t>& image
            )
        {
            std::vector<COLInfo> result;

            if (image.size() < 0x18)
                return result;

            for (
                size_t offset = 0;
                offset + 0x18 <= image.size();
                offset += 4
                )
            {
                uint32_t signature = 0;

                std::memcpy(
                    &signature,
                    image.data() + offset,
                    sizeof(signature)
                );

                if (signature != 1)
                    continue;

                uint32_t typeRVA = 0;
                uint32_t chdRVA = 0;
                uint32_t selfRVA = 0;

                std::memcpy(
                    &typeRVA,
                    image.data() + offset + 0x0C,
                    4
                );

                std::memcpy(
                    &chdRVA,
                    image.data() + offset + 0x10,
                    4
                );

                std::memcpy(
                    &selfRVA,
                    image.data() + offset + 0x14,
                    4
                );

                uintptr_t col =
                    module.base + offset;

                // Validate self RVA.

                if (selfRVA !=
                    static_cast<uint32_t>(
                        offset
                        ))
                {
                    continue;
                }

                // TypeDescriptor must be inside module.

                uintptr_t typeDescriptor =
                    module.base + typeRVA;

                if (!inModule(
                    typeDescriptor,
                    module,
                    0x18
                ))
                {
                    continue;
                }

                // CHD must be inside module.

                uintptr_t chd =
                    module.base + chdRVA;

                if (!inModule(
                    chd,
                    module,
                    0x10
                ))
                {
                    continue;
                }

                // Check CHD.

                uint32_t numBaseClasses = 0;

                if (!readMemory(
                    chd + 0x08,
                    numBaseClasses
                ))
                {
                    continue;
                }

                if (numBaseClasses == 0 ||
                    numBaseClasses > 1024)
                {
                    continue;
                }

                result.push_back({
                    col,
                    typeDescriptor
                    });
            }

            return result;
        }



    public:
        inline static bool Extract(
            std::string szDumpFileName,
            bool bLog = true
        )
        {
            std::ofstream dumpFile(
                szDumpFileName,
                std::ios::out |
                std::ios::trunc
            );

            if (!dumpFile.is_open())
                return false;

            Module m;

            m.base = pExeMod->GetAddr();
            m.size = pExeMod->GetSize();

            // --------------------------------------------------------
            // Output helper
            // --------------------------------------------------------

            auto log =
                [&](const std::string& text)
                {
                    dumpFile << text;

                    if (bLog)
                        std::cout << text;
                };

            // --------------------------------------------------------
            // Module information
            // --------------------------------------------------------

            {
                std::ostringstream ss;

                ss
                    << "[+] Module base: 0x"
                    << std::hex
                    << m.base
                    << "\n";

                log(ss.str());
            }

            {
                std::ostringstream ss;

                ss
                    << "[+] Module size: 0x"
                    << std::hex
                    << m.size
                    << "\n";

                log(ss.str());
            }

            // --------------------------------------------------------
            // Read module once
            // --------------------------------------------------------

            auto image =
                readModule(m);

            if (image.empty())
            {
                log(
                    "[-] Failed to read module.\n"
                );

                return false;
            }

            // --------------------------------------------------------
            // Find ALL COLs
            // --------------------------------------------------------

            auto cols =
                findCOLs(
                    m,
                    image
                );

            {
                std::ostringstream ss;

                ss
                    << "[+] COLs found: "
                    << std::dec
                    << cols.size()
                    << "\n";

                log(ss.str());
            }

            // --------------------------------------------------------
            // Find ALL vtables
            // --------------------------------------------------------

            std::vector<VTableInfo>
                vtables;

            std::unordered_set<uintptr_t>
                seen;

            for (const auto& col : cols)
            {
                auto found =
                    findVTables(
                        m,
                        image,
                        col
                    );

                for (const auto& vtable : found)
                {
                    if (seen.insert(
                        vtable.address
                    ).second)
                    {
                        vtables.push_back(
                            vtable
                        );
                    }
                }
            }

            {
                std::ostringstream ss;

                ss
                    << "[+] Vtables found: "
                    << std::dec
                    << vtables.size()
                    << "\n";

                log(ss.str());
            }

            // --------------------------------------------------------
            // Dump vtables
            // --------------------------------------------------------

            for (const auto& vtable : vtables)
            {
                // Always write to file.
                dumpVTable(
                    dumpFile,
                    vtable,
                    m
                );

                // Optionally write the same output to console.
                if (bLog)
                {
                    dumpVTable(
                        std::cout,
                        vtable,
                        m
                    );
                }
            }

            dumpFile.flush();

            return true;
        }
    };
}