#!/bin/bash

Lib_Cp_Path="../../get-all-so/"
Protected_Lib_Cp_Path="../../get-all-so-protected/"

mkdir ${Lib_Cp_Path}
mkdir ${Protected_Lib_Cp_Path}

rm -r -f ${Lib_Cp_Path}*.so*
rm -r -f ${Protected_Lib_Cp_Path}*.so*

# cp -r -P ../../BuildTarget/Release/InsBase/lib/*  ${Lib_Cp_Path}
cp -r -P ../../BuildTarget/Release/InsTorch/lib/*  ${Lib_Cp_Path}
cp -r -P ../../BuildTarget/Release/LibDcsMaker/lib/*  ${Lib_Cp_Path}
cp -r -P ../../BuildTarget/Release/LibParam/lib/*  ${Lib_Cp_Path}
cp -r -P ../../BuildTarget/Release/LibErgebnis/lib/*  ${Lib_Cp_Path}

# cp -r -P ../../BuildTarget/Release/InsBase-protected/lib/*  ${Protected_Lib_Cp_Path}
cp -r -P ../../BuildTarget/Release/InsTorch-protected/lib/*  ${Protected_Lib_Cp_Path} || true
cp -r -P ../../BuildTarget/Release/LibDcsMaker-protected/lib/*  ${Protected_Lib_Cp_Path} || true
cp -r -P ../../BuildTarget/Release/LibParam-protected/lib/*  ${Protected_Lib_Cp_Path} || true
cp -r -P ../../BuildTarget/Release/LibErgebnis-protected/lib/*  ${Protected_Lib_Cp_Path} || true
