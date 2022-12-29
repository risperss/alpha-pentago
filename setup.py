import sys
from glob import glob

from pybind11 import get_cmake_dir
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

source_files = glob("src/**/*.cc")
source_files.append("src/pentago.cc")

ext_modules = [
    Pybind11Extension(
        "pentago",
        source_files,
        include_dirs=["src"],
        define_macros=[('VERSION_INFO', __version__)],
    ),
]

setup(
    name="pentago",
    version=__version__,
    author="Gaetano Rispoli",
    author_email="grispoli@uwaterloo.ca",
    url="https://github.com/risperss/alpha_pentago",
    description="Pentago Game and Engine",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)