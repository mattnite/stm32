from conans import ConanFile, CMake, tools

class Stm32Conan(ConanFile):
    name = "stm32"
    version = "0.1"
    license = "MIT"
    author = "Matthew Knight"
    url = "https://github.com/matt1795/stm32"
    description = "Example program for an STM32"
    topics = ("embedded", "mcu", "stm32")
    generators = "cmake_find_package", "virtualenv"
    build_requires = "gcc-arm-none-eabi_installer/8-2018-q4-major@matt1795/testing"
    requires = "register-alias/0.3@matt1795/testing"
    options = {
        "thumb": [True, False],
    }

    default_options = {
        "thumb": True,
    }

    def build(self):
        cmake = CMake(self)
        cmake.definitions["THUMB"] = self.options.thumb
        cmake.configure(source_folder=self.source_folder)
        cmake.build()
