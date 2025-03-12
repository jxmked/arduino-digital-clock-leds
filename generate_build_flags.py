import os

build_flags = [
    "#define stringify(s) _stringifyDo(s)",
    "#define _stringifyDo(s) #s"
]

with open("./.env", "r") as file:
    for line in file.readlines():
        line = line[:-1:]  # remove newline

        key = line.split("=")[0]
        value = line.split("=")[1]

        build_flags.append(f"#define {key} {value}")

with open("include/DOTENV.h", "w+") as file:
    file.write("\n".join(build_flags))
