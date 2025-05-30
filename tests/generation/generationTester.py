import subprocess
from os import listdir, remove, makedirs
from os.path import isfile, join
import re

RED = "\x1b[31m"
GREEN = "\x1b[32m"
YELLOW = "\x1b[33m"
BLUE = "\x1b[34m"
RESET = "\x1b[0m"
CURSOR_UP = "\x1b[1A"

RUN = f"[{BLUE}RUN {RESET}]"
PASS = f"[{GREEN}PASS{RESET}]"
FAIL = f"[{RED}FAIL{RESET}]"
CRASH = f"[{RED}CRSH{RESET}]"
INFO = f"[{BLUE}----{RESET}]"
DELIMITER = f"[{BLUE}===={RESET}]"

print(f"{INFO} {__file__.split('/')[-1]}")

tests_directory = "./tests/generation/test_files"

test_files = [join(tests_directory, f) for f in listdir(tests_directory) if isfile(join(tests_directory, f))]
test_files.sort()

print(f"{DELIMITER} Running {BLUE}{len(test_files)}{RESET} tests from {YELLOW}generation{RESET}:")

passing = 0
failing = 0
crashing = 0

try:
    makedirs("./_tests_build/mid_prod/generation")
except OSError as err:
    pass

for test_file in test_files:
    with open(test_file, "r") as f:
        
        test_filename = test_file.split('/')[-1]
        print(f"{RUN} generation::{test_filename}")
        
        file_stringified = f.read()
        comments = [match[0] for match in re.findall("\/\*(([^*]|(\*+([^*/])))*)\*+\/", file_stringified)]
        expected_output = comments[0]
        stdin = comments[1] if len(comments) >= 2 else ""
        
        p = subprocess.Popen(["./_build/bin/epicCompiler3000", ""], stdout=subprocess.PIPE,
                                                                    stdin=subprocess.PIPE,
                                                                    stderr=subprocess.PIPE)
        out, err = p.communicate(file_stringified.encode())
        out = out.decode('utf-8')
        err = err.decode('utf-8')
        
        if err != "":
            # crash happend
            print(f"{CURSOR_UP}{CRASH} generation::{test_filename}: {err[:-1]}")
            crashing = crashing + 1
            continue
        elif p.returncode != 0:
            # unexpected compiler return code
            print(f"{CURSOR_UP}{FAIL} generation::{test_filename}: Test file is not valid IFJ23. Compiler returned: {p.returncode}")
            failing = failing + 1
            continue
        
        # pass compiler
        with open("./_tests_build/mid_prod/generation/" + test_filename[:-6], "w") as temp:
            temp.write(out)
            
        p = subprocess.Popen(["./ic23int", "./_tests_build/mid_prod/generation/" + test_filename[:-6]], stdout=subprocess.PIPE,
                                                                                                       stdin=subprocess.PIPE,
                                                                                                       stderr=subprocess.PIPE)
        
        out, err = p.communicate(stdin.encode())
        out = out.decode('utf-8')
        err = err.decode('utf-8')
        
        if p.returncode != 0:
            # unexpected interpreter return code
            print(f"{CURSOR_UP}{FAIL} generation::{test_filename}: Interpreter returned error code: {p.returncode}")
            print(INFO + " " + err[:-1].replace("\n", f"\n{INFO} "))
            failing = failing + 1
            continue
        elif out != expected_output:
            # unexpected output
            print(f"{CURSOR_UP}{FAIL} generation::{test_filename}: Output does not match with expected")
            print("-------------------------------- expected --------------------------------")
            print(expected_output)
            print("-------------------------------- received --------------------------------")
            print(out)
            print("--------------------------------------------------------------------------")
            failing = failing + 1
            continue
        
        # pass interpreter
        print(f"{CURSOR_UP}{PASS} generation::{test_filename}")
        passing = passing + 1
        
failing_s = "0" if failing == 0 else f"{RED}{failing}{RESET}"
crashing_s = "0" if crashing == 0 else f"{RED}{crashing}{RESET}"
print(f"{DELIMITER} Synthesis: Tested: {BLUE}{len(test_files)}{RESET} | Passing: {GREEN}{passing}{RESET} | Failing: {failing_s} | Crashing: {crashing_s}")
        