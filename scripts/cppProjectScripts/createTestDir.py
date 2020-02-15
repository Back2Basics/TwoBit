#!/usr/bin/env python3

import argparse
import shutil
import sys

import CppHeaderParser
from pathlib import Path

sys.path.append((Path(__file__).parent / "pyUtils").absolute())
from color_text import ColorText as CT
from headInGraph import *

testerBodyTemplate = """
/*
TEST_CASE("Basic tests for {REPLACETHIS}", "[{REPLACETHIS_DETAILED}]" ){{
  SECTION("GIVE SECTION NAME"){{
      YOUR CODE GOES HERE
        NORMALLY END WITH A REQUIRE STATEMENT e.g.
        REQUIRE(TESTVAL1 == YOURVAL);
  }}
}}
*/
"""


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--src', type=str, required=True)
    parser.add_argument('--outDir', type=str, required=True)
    parser.add_argument("--overWrite", action='store_true')
    # parser.add_argument("--update", action = 'store_true')
    return parser.parse_args()


def getFuncDetailed(func):
    ret = ""
    ret += (func["rtnType"] + " ")
    ret += (func["name"] + " (")
    count = 0
    for par in func["parameters"]:
        if (count != 0):
            ret = ret + (",")
        count += 1
        ret = ret + (par["raw_type"])
        if (par["reference"]):
            ret = ret + ("&")
        elif par["pointer"]:
            ret = ret + ("*")
        ret = ret + (" " + par["name"])
    ret = ret + ")"
    return ret


def createTestMain(path, overWrite):
    mainBody = """
// based off https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main()
#include <catch.hpp>

    """
    mainPath = path / "main.cpp"
    if mainPath.exists() and overWrite:
        mainPath.write_text(mainBody)
    elif mainPath.exists():
        print(mainPath, "already exists, use --overWrite to remove current")
        return


def copyMakefile(fromLoc, dest, overWrite):
    if dest.exists() and overWrite:
        dest.unlink()
    elif dest.exists():
        print(dest, "already exists, use --overWrite to replace it")
        return
    shutil.copy(fromLoc, dest)


def main():
    args = parse_args()
    headers = fileCollection.getHeaderFiles(args.src)

    for head in headers:
        try:
            cppHeader = CppHeaderParser.CppHeader(head)
        except CppHeaderParser.CppParseError as e:
            print(e)
            sys.exit(1)
            print(CT.boldBlack("Class public methods"))

        if (len(cppHeader.classes) + len(cppHeader.functions) > 0):
            testerCppPath = Path(args.outDir) / head.replace(".hpp", "Tester.cpp")
            testerCppPath.parent.mkdir(parents=True, exist_ok=True)

            if testerCppPath.exists() and args.overWrite:
                testerCppPath.unlink()
            elif testerCppPath.exists():
                print("Skipping", testerCppPath, "it already exist, use --overWrite to replace")
            with open(testerCppPath, "w") as testerFile:
                data = "#include <catch.hpp>\n" + "#include \"" + "../" + head + "\"\n"
                data += '\n'.join(
                    [testerBodyTemplate.format(REPLACETHIS=func["name"], REPLACETHIS_DETAILED=getFuncDetailed(func)) for
                     func in cppHeader.functions])

                for k in cppHeader.classes.keys():
                    for count, _ in enumerate(cppHeader.classes[k]["methods"]["public"]):
                        data += testerBodyTemplate.format(
                            REPLACETHIS=cppHeader.classes[k]["methods"]["public"][count]["name"],
                            REPLACETHIS_DETAILED=getFuncDetailed(
                                cppHeader.classes[k]["methods"]["public"][count]))
                testerCppPath.write_text(data)

    createTestMain(Path(args.outDir) / args.src, args.overWrite)
    copyMakefile(Path("scripts/cppMakefiles/unitTest/Makefile"), Path(args.outDir) / "Makefile", args.overWrite)
    return 0


main()
