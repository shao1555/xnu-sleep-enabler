xnu-sleep-enabler
=================

add compatibility for 10.6.6 release

Build
-----
to build, you must install Xcode and other utilities.

    xcodebuild -configuration 'Release 10.6.6'

Install
-------
under chameleon, you have to install /Extra/Extensions

    sudo cp -rfv build/Release\ 10.6.6/SleepEnabler.kext /Extra/Extensions/

after installation, you have to rebuild kextcache.

    chown -R root:wheel /System/Library/Extensions/*
    chmod -R 755 /System/Library/Extensions/*
    rm -R /System/Library/Caches
    kextcache -v 1 -t -l -m /System/Library/Caches/com.apple.kext.caches/Startup/Extensions.mkext /System/Library/Extensions
    kextcache -v 1 -t -l -m /Extra/Extensions.mkext /Extra/Extensions /System/Library/Extensions

or use kext utility :p

no longer needs 'pmVersion=21' to boot arguments.

Apply for future release
------------------------
if you want to change this patch for newer version, you have to get newer release of pmCPU.h file from apple opensource site.

http://www.opensource.apple.com/

download latest xnu archives and extract it, you can find pmCPU.h file under "./osfmk/i386" directory.

copy `pmCPU.h` and related header files (in 10.6.6 release, `cpu_topology.h` and `rtclock.h` are required by pmCPU.h)

open Xcode project file and create new configuration from Project Inspector.

modify `KERNEL_VERSION` valiant and build it.

