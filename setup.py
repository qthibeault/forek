from skbuild import setup

setup(
    name="forek",
    version="0.1.0",
    description="",
    author="Quinn Thibeault",
    license="BSD-3-Clause",
    packages=["forek"],
    package_dir={
        "": "python"
    },
    package_data={
        "forek": ["py.typed", "*.pyi"]
    },
    python_requires=">=3.8",
    cmake_args = ["-DFOREK_PYTHON_BINDINGS:BOOL=ON"],
    cmake_install_dir="python/forek",
    cmake_install_target="_forek"
)
