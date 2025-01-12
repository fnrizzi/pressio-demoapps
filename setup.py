#!/usr/bin/env python

import os
import sys
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install as _install

topdir = os.path.abspath(os.path.dirname(__file__))

# ----------------------------------------------
# Metadata
# ----------------------------------------------
def myname():
  return "pressiodemoapps"

def myversion():
  return "0.1.0rc1"

def description():
  with open(os.path.join(topdir, 'DESCRIPTION.rst')) as f:
    return f.read()

# ----------------------------------------------
# overload install command
# ----------------------------------------------
class install(_install):
  user_options = _install.user_options + [
    ('enable-openmp=', None, "Enable OpenMP")
  ]

  def initialize_options(self):
    _install.initialize_options(self)
    self.enable_openmp = False

  def finalize_options(self):
    _install.finalize_options(self)

  def run(self):
    global enableOpenMP
    enableOpenMP = self.enable_openmp
    print("GIGI = ", self.enable_openmp)
    _install.run(self)

# ----------------------------------------------
# A CMakeExtension needs a sourcedir instead of a file list.
class CMakeExtension(Extension):
  def __init__(self, name, sourcedir=""):
    Extension.__init__(self, name, sources=[])
    self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
  def build_extension(self, ext):
    # print(build_ext.user_options)
    # user_options = build_ext.user_options + [
    #   ('enable-openmp=', None, "Enable OpenMP")
    # ]

    extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
    if not extdir.endswith(os.path.sep): extdir += os.path.sep
    print("self.extdir ",extdir)

    # create build directory
    if not os.path.exists(self.build_temp): os.makedirs(self.build_temp)
    print("self.build_temp ", self.build_temp)

    # debug/release mode
    buildMode = "Debug" if self.debug else "Release"
    print("self.debug = ", self.debug)

    # CMake lets you override the generator - we need to check this.
    # Can be set with Conda-Build, for example.
    cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

    #--------------------------
    # --- pressio-demoapps ----
    #---------------------------
    # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
    # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
    # from Python.
    ompstring = "Off"
    if enableOpenMP:
      print("Enabling OMP")
      ompstring = "On"

    cmake_args = [
      "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
      "-DPYTHON_EXECUTABLE={}".format(sys.executable),
      "-DCMAKE_BUILD_TYPE={}".format(buildMode),
      "-DPRESSIODEMOAPPS_ENABLE_BINDINGS=On",
      "-DCMAKE_VERBOSE_MAKEFILE=On",
      "-DPRESSIODEMOAPPS_ENABLE_OPENMP={}".format(ompstring)
      #"-DVERSION_INFO={}".format(self.distribution.get_version()),
    ]
    build_args = []

    if "CXX" not in os.environ:
      msg = "CXX env var missing, needs to point to your target C++ compiler"
      raise RuntimeError(msg)

    # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
    # across all generators.
    if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
      # for now, set parallel level to 4
      build_args += ["-j4"]
      # # self.parallel is a Python 3 only way to set parallel jobs by hand
      # # using -j in the build_ext call, not supported by pip or PyPA-build.
      # if hasattr(self, "parallel") and self.parallel:
      #   # CMake 3.12+ only.
      #   build_args += ["-j{}".format(self.parallel)]

    if not os.path.exists(self.build_temp):
      os.makedirs(self.build_temp)

    subprocess.check_call(
      ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
    )
    subprocess.check_call(
      ["cmake", "--build", "."] + build_args, cwd=self.build_temp
    )

# -----------------------------
# setup
# -----------------------------
def run_setup():
  setup(
    name=myname(),
    version=myversion(),
    author="F.Rizzi, E.Parish, P.Blonigan, J.Tencer",
    author_email="fnrizzi@sandia.gov",
    description="pressio-demoapps: Python bindings",
    long_description=description(),

    project_urls={
      'Source': 'https://github.com/Pressio/pressio-demoapps'
    },

    ext_modules=[CMakeExtension("pressiodemoapps._pressiodemoappsimpl")],
    cmdclass={"build_ext": CMakeBuild,
              "install"  : install},
    #cmdclass={"build_ext": CMakeBuild},

    install_requires=["numpy", "scipy", "matplotlib"],
    zip_safe=False,

    python_requires='>=3',
    classifiers=[
      "License :: OSI Approved :: BSD License",
      "Operating System :: Unix",
      "Programming Language :: C++",
      "Programming Language :: Python :: 3 :: Only",
      "Topic :: Scientific/Engineering",
      "Topic :: Scientific/Engineering :: Mathematics",
      "Topic :: Scientific/Engineering :: Physics",
      "Topic :: Software Development :: Libraries",
      "Development Status :: 4 - Beta"
    ],

    keywords=["model reduction",
              "sample mesh",
              "scientific computing",
              "linear algebra",
              "pressio",
              "HPC"],

    packages=['pressiodemoapps'],
  )

if __name__ == '__main__':
  run_setup()
