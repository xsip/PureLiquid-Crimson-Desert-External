
import ida_funcs
import ida_name
import ida_kernwin
import os


def rename_functions():

    # Open file selection dialog
    txt_file = ida_kernwin.ask_file(
        False,
        "*.txt",
        "Select function_names.txt"
    )

    if not txt_file:
        print("[!] File selection cancelled.")
        return

    renamed = 0
    failed = 0

    print(f"\n[*] Loading: {txt_file}\n")

    with open(txt_file, "r", encoding="utf-8") as f:
        for line_num, line in enumerate(f, 1):

            line = line.strip()

            # Skip empty lines and comments
            if not line or line.startswith("#"):
                continue

            try:
                parts = line.split(maxsplit=1)

                if len(parts) != 2:
                    print(f"[!] Invalid line {line_num}: {line}")
                    failed += 1
                    continue

                ea = int(parts[0], 0)
                prefix = parts[1].strip()

                # Find function
                func = ida_funcs.get_func(ea)

                if func is None:
                    print(f"[-] No function at {hex(ea)}")
                    failed += 1
                    continue

                # Get original IDA function name
                original_name = ida_name.get_name(func.start_ea)

                if not original_name:
                    original_name = f"sub_{func.start_ea:X}"

                # Construct final name
                final_name = prefix.rstrip("_") + "_" + original_name

                # Rename function
                if ida_name.set_name(
                    func.start_ea,
                    final_name,
                    ida_name.SN_FORCE
                ):
                    print(
                        f"[+] {hex(func.start_ea)}: "
                        f"{original_name} -> {final_name}"
                    )
                    renamed += 1
                else:
                    print(f"[-] Failed to rename: {final_name}")
                    failed += 1

            except Exception as e:
                print(f"[!] Error on line {line_num}: {e}")
                failed += 1

    print("\n========== SUMMARY ==========")
    print(f"Renamed: {renamed}")
    print(f"Failed:  {failed}")
    print(f"File: {os.path.basename(txt_file)}")


if __name__ == "__main__":
    rename_functions()
