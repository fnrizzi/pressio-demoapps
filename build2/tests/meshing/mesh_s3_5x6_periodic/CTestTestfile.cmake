# CMake generated Testfile for 
# Source directory: /Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/tests/meshing/mesh_s3_5x6_periodic
# Build directory: /Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/build2/tests/meshing/mesh_s3_5x6_periodic
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(full_mesh_5x6_s3_periodic "/usr/local/Cellar/cmake/3.18.4/bin/cmake" "-DMESHDRIVER=/Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/tests/meshing/../../meshing_scripts/create_full_mesh.py" "-DOUTDIR=/Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/build2/tests/meshing/mesh_s3_5x6_periodic" "-Dnx=5" "-Dny=6" "-Dss=3" "-P" "/Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/tests/meshing/mesh_s3_5x6_periodic/../test2d.cmake")
set_tests_properties(full_mesh_5x6_s3_periodic PROPERTIES  _BACKTRACE_TRIPLES "/Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/tests/meshing/mesh_s3_5x6_periodic/CMakeLists.txt;8;add_test;/Users/fnrizzi/Desktop/work/ROM/gitrepos/pressio-demoapps/tests/meshing/mesh_s3_5x6_periodic/CMakeLists.txt;0;")
