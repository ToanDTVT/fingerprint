# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ESP/ESP/Espressif/frameworks/esp-idf-v5.2.2/components/bootloader/subproject"
  "D:/ESP/ESP project/fingerprint/build/bootloader"
  "D:/ESP/ESP project/fingerprint/build/bootloader-prefix"
  "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/tmp"
  "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/src/bootloader-stamp"
  "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/src"
  "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/ESP/ESP project/fingerprint/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
