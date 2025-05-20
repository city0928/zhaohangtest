#!/bin/bash

# Lib_Cp_Path="../../get-all-so/"
# Protected_Lib_Cp_Path="../../get-all-so-protected/"

# run this script before build to remove installed libs and symlinks

# rm -f ../../BuildTarget/Release/InsBase/lib/*  || true
rm -f ../../BuildTarget/Release/InsTorch/lib/* || true
rm -f ../../BuildTarget/Release/InsTorch/include/* || true
rm -f ../../BuildTarget/Release/LibDcsMaker/lib/* || true
rm -f ../../BuildTarget/Release/LibDcsMaker/include/* || true
rm -f ../../BuildTarget/Release/LibParam/lib/* || true
rm -f ../../BuildTarget/Release/LibParam/include/* || true
rm -f ../../BuildTarget/Release/LibErgebnis/lib/* || true
rm -f ../../BuildTarget/Release/LibErgebnis/include/* || true

# rm -f ../../BuildTarget/Release/InsBase-protected/lib/* || true
rm -f ../../BuildTarget/Release/InsTorch-protected/lib/* || true
rm -f ../../BuildTarget/Release/InsTorch-protected/include/* || true
rm -f ../../BuildTarget/Release/LibDcsMaker-protected/lib/* || true
rm -f ../../BuildTarget/Release/LibDcsMaker-protected/include/* || true
rm -f ../../BuildTarget/Release/LibParam-protected/lib/* || true
rm -f ../../BuildTarget/Release/LibParam-protected/include/* || true
rm -f ../../BuildTarget/Release/LibErgebnis-protected/lib/* || true
rm -f ../../BuildTarget/Release/LibErgebnis-protected/include/* || true
