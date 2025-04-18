from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11

ext_modules = [
    Pybind11Extension(
        "tokenizer",
        ["bindings.cpp", "tokenizer.cpp"],  # همه فایل‌های cpp
        include_dirs=[
            pybind11.get_include(),
            "extern"
        ],
        cxx_std=17
    ),
]

setup(
    name="tokenizer",
    version="0.1",
    author="Your Name",
    description="A BPE tokenizer with pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
