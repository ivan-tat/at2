# Adlib Tracker 2

This is a fork of the Adlib Tracker 2 project (GNU GPL 3+) based on the latest sources from the official website (released on 2019-09-10, DOS version is 2.3.57, SDL version is 2.4.24).
Original project is abandoned by it's authors since 2019.

Examples of compatible tracked music files (modules) and instruments can be found on the official website.

3rd-party software used:

* DOS version:

  + CWSDPMI r5 DOS extender by CW Sandmann (GNU GPL 2).

* Unix, Linux, Windows version:

  + SDL library version 1.2 (GNU LGPL 2.1).

My interests in this project:

* Just for fun.
* Clean code: eliminate and separate platform-specific code.
* Remove all unneeded assembler optimizations and obfuscations.
* Make it portable: completely rewrite in C.
* Keep it simple for portability.
* Practice Git.
* Practice good programming technics.
* Translate into Russian.
* Make this program available on the territory of Ukraine.
* Add support for 4K displays.
* Speed-up screen drawing routines and sound rendering.
* Play audio streams.
* Don't forget about media centers.

On the first seeing:

* There are many extra media-files in the project tree (instruments and music modules).
* DOS, Windows and Unix/Linux sources are merged together but have different versioning.
* All sources use a 437 code page (CP437) and have DOS line endings (CR+LF).
* There is no Unicode support.
* It is written in Object Pascal (Turbo Pascal compatible mode) with Intel 386-specific assembler code.
* There are 4 separate ways to compile the project (a batch file for DOS, a batch file for Windows, two separate Makefiles for Linux - one for native build and a template for cross-compilation).
* Precompiled DOS and Windows only helper executables (located in the `utils/` directory) without source code are used during compilation.
* DOS port uses GO32 Free Pascal's library to deal with DPMI.
* It uses SDL library (version 1.2) with bindings for 32-bits CPUs only.
* Compiled object files were placed inside source code tree making a mess and it's not possible to cross-compile without cleaning first.
* It won't compile for actual OSes using actual Free Pascal compiler.
* To compile the project you will need retro-compilers (Free Pascal compiler version 2.6.4 will be fine) and a proper environment (appropriate tools for cross-compilation or: Debian GNU/Linux "Wheezy" i386 OS, DOS environment for a DOS version, QEMU or similar software).

Now:

* Fixed binding with SDL library on 64-bit platforms.
* To build `adtrack2` under DOS (also true for `adt2play`):
  + Install DJGPP environment and Free Pascal compiler in DOSBox-X with LFN support or on a real hardware.
  + Run `./Make-DOS.sh`.
* To build `adtrack2` under Windows (works for i386 target for now):
  + Install Cygwin/MSYS2/MinGW, Free Pascal compiler.
  + Run `./Make-Windows-i386.sh` or `./Make-Windows-x86_64.sh`.
* To build `adtrack2` under GNU/Linux (works for i386 target for now):
  + Install Free Pascal compiler and SDL library.
  + Run `./Make-Linux-i386.sh` or `./Make-Linux-x86_64.sh`.
* These scripts can also be run in any supported OS for cross-compilation. See [docs/build.md](docs/build.md) for details.
* The compiled object files have been moved to the target directory and are no longer annoying lying around everywhere.
* Many extra and unneeded files were removed.
* Compilation is no longer dependent on unknown binary executables.

See [docs/pascal-to-c.md](docs/pascal-to-c.md) for information about porting Pascal code to C.

## References

* [Adlib Tracker 2](http://www.adlibtracker.net) - official website

### Operating systems

* [GNU Operating System](https://www.gnu.org/)
* [Debian GNU/Linux](https://www.debian.org/) - a complete Free Operating System
* [FreeDOS](https://freedos.org/) - an open source DOS-compatible operating system

### Virtual machines (platform emulators)

* [QEMU](https://www.qemu.org/) ([package](https://pkgs.org/search/?q=qemu)) - a generic and open source machine emulator and virtualizer
* [DOSBox](https://www.dosbox.com/) ([package](https://pkgs.org/search/?q=dosbox)) - a /free and open source DOS-emulator that uses the SDL library
* [DOSBox-X](https://dosbox-x.com/) ([package](https://pkgs.org/search/?q=dosbox-x)) - an open-source DOS emulator names that supports long file names for running DOS applications and games
* [DOSBox Staging](https://dosbox-staging.github.io/) - a modern continuation of DOSBox with advanced features and current development practices

### Compilers

* [GNU Binutils](https://www.gnu.org/software/binutils/) ([package](https://pkgs.org/search/?q=binutils), [package for DOS target](https://pkgs.org/search/?q=binutils-djgpp)) - a collection of binary tools (linker, assembler and others)
* [GNU Compiler Collection](https://www.gnu.org/software/gcc/) ([package](https://pkgs.org/search/?q=gcc))
* [Free Pascal](https://www.freepascal.org/) ([package](https://pkgs.org/search/?q=fpc)) - an open source Object Pascal compiler supporting both Delphi and Turbo Pascal 7.0 dialects, as well as Mac Pascal dialects
* [DJGPP](https://www.delorie.com/djgpp/) - DOS ports of many GNU development utilities
* [GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler) - creating a GCC cross-compiler for your own operating system
* [Cross compiling](https://wiki.lazarus.freepascal.org/Cross_compiling)

### Libraries

* [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/) - a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D
* [SDL4Freepascal](https://sdl4fp.sourceforge.net/) - a set of FreePascal modules (units) with bindings to the SDL library
* [GNU GnuDOS](https://www.gnu.org/software/gnudos/) - a library designed to help new users of the GNU system, who are coming from a DOS background
* [CWSDPMI](http://sandmann.dotster.com/cwsdpmi/) - provides DPMI services for V2 of DJGPP
* [aPLib](https://ibsensoftware.com/products_aPLib.html) - a compression library based on the algorithm used in aPACK
* [Nuked-OPL3](https://github.com/nukeykt/Nuked-OPL3) - highly accurate Yamaha OPL3 (YMF262, CT1747) emulator

### Tools

* [Linux utilities](https://www.kernel.org/pub/linux/utils/util-linux/) ([package](https://pkgs.org/search/?q=util-linux), [Debian package](https://pkgs.org/search/?q=bsdextrautils)) - miscellaneous system utilities
* [GNU Core Utilities](https://www.gnu.org/software/coreutils/) ([package](https://pkgs.org/search/?q=coreutils))
* [GNU Bash](https://www.gnu.org/software/bash/) ([package](https://pkgs.org/search/?q=bash)) - GNU Bourne Again SHell
* [GNU Grep](https://www.gnu.org/software/grep/) ([package](https://pkgs.org/search/?q=grep)) - a non-interactive command-line utility to search for text in files
* [GNU sed](https://www.gnu.org/software/sed/) ([package](https://pkgs.org/search/?q=sed)) - a non-interactive command-line text editor
* [GNU Make](https://www.gnu.org/software/make/) ([package](https://pkgs.org/search/?q=make)) - utility for directing compilation
* [GNU Tar](https://www.gnu.org/software/tar/) ([package](https://pkgs.org/search/?q=tar)) - a utility to create tar archives, as well as various other kinds of manipulation
* [GNU Gzip](https://www.gnu.org/software/gzip/) ([package](https://pkgs.org/search/?q=gzip)) - a popular data compression program originally written for the GNU project
* [Zip](https://infozip.sourceforge.net/Zip.html) ([package](https://pkgs.org/search/?q=zip)) - a compression and file packaging/archive utility
* [dos2unix](https://waterlan.home.xs4all.nl/dos2unix.html) ([package](https://pkgs.org/search/?q=dos2unix)) - convert text file line endings between CRLF and LF
* [Netpbm](https://sourceforge.net/projects/netpbm/) - utilities for primitive manipulation of graphic images
* [GraphicsMagick](http://www.graphicsmagick.org/) ([package](https://pkgs.org/search/?q=graphicsmagick)) - collection of image processing tools
* [Cygwin](https://cygwin.com/) - a large collection of GNU and Open Source tools which provide functionality similar to a Linux distribution on Windows
* [MSYS2](https://www.msys2.org/) - a collection of tools and libraries providing you with an easy-to-use environment for building, installing and running native Windows software
* [MinGW](https://osdn.net/projects/mingw/) - Minimalist GNU for Windows
* [MinGW-w64](https://mingw-w64.org/) ([package](https://pkgs.org/search/?q=mingw-w64)) - an advancement of the original mingw.org project, created to support the GCC compiler on Windows systems
* [cmd](https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/cmd) - command interpreter in Microsoft Windows
* [Windows commands](https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/windows-commands)
* [GNU Midnight Commander](https://www.midnight-commander.org) ([package](https://pkgs.org/search/?q=mc)) - a text-mode full-screen file manager for Unix/Linux
* [Far Manager](https://www.farmanager.com/) - a free console file manager (NC clone) for Microsoft Windows
* [far2l](https://github.com/elfmz/far2l) ([package](https://pkgs.org/search/?q=far2l)) - Linux fork of FAR Manager v2
* [The Doszip Commander](https://sourceforge.net/projects/doszip/) - a LFN-aware TUI file manager (NC clone) with built-in Zip and UnZip for DOS and Windows
* [UPX](https://upx.github.io/) ([package](https://pkgs.org/search/?q=upx)) - efficient live-compressor for executables

### Fonts

* [Code page 437](https://en.wikipedia.org/wiki/Code_page_437)
* [UTF-8](https://en.wikipedia.org/wiki/UTF-8)
* [Unicode Consortium](https://home.unicode.org/)

### Hardware

* [Programmable interrupt controller](https://en.wikipedia.org/wiki/Programmable_interrupt_controller)
* [x86 Assembly/Programmable Interval Timer](https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interval_Timer)
* [Programmable Interval Timer](https://wiki.osdev.org/Programmable_Interval_Timer)
* [VGA Hardware](https://wiki.osdev.org/VGA_Hardware)
* [VGA Signal Timing](http://tinyvga.com/vga-timing)
* [Hardware Level VGA and SVGA Video Programming Information](http://www.osdever.net/FreeVGA/vga/vga.htm)
* [Keyboard scancodes](https://www.win.tue.nl/~aeb/linux/kbd/scancodes.html)
* [Yamaha OPL](https://en.wikipedia.org/wiki/Yamaha_OPL#OPL3)
* [Programming the AdLib/Sound Blaster FM Music Chips](https://bochs.sourceforge.io/techspec/adlib_sb.txt) (text)
* [AdLib Functions](https://cosmodoc.org/topics/adlib-functions/)
* [Yamaha YMF262 (OPL3) emulator using YMF262 die shot](https://github.com/nukeykt/YMF262-LLE)
* [OPL2/3 Frequency - The 1Hz-ish Difference](http://nerdlypleasures.blogspot.com/2018/01/opl23-frequency-1hz-ish-difference.html)

### Documentation

* [TECH Help!](http://www.techhelpmanual.com/) - The Electronic Technical Reference Manual
* [BIOS Data Area (BDA)](https://stanislavs.org/helppc/bios_data_area.html) - x86 PC memory map
* [DOS Protected Mode Interface](https://en.wikipedia.org/wiki/DOS_Protected_Mode_Interface)
* [DPMI version 0.9 specification (text format)](https://web.archive.org/web/20120425162013/http://homer.rice.edu/~sandmann/cwsdpmi/dpmispec.txt)
* [DPMI version 0.9 specification (HTML format)](https://web.archive.org/web/20160521221856/http://www.tenberry.com/dpmi/index.html)
* [VESA BIOS Extensions](https://en.wikipedia.org/wiki/VESA_BIOS_Extensions)
* [VESA Super VGA BIOS Extension 1.0](https://web.archive.org/web/20131211151957/http://www.gemixtes.de/super-vga-bios-extension-standard-vs891001/) - Standard # VS891001, 1 October 1989
* [VESA BIOS Extension 1.2 (text)](https://web.archive.org/web/20090114055246/http://docs.ruudkoot.nl/vesasp12.txt)
* [VESA BIOS Extension 2.0 (text)](https://web.archive.org/web/20081211174813/http://docs.ruudkoot.nl/vbe20.txt)
* [VESA BIOS Extension 3.0 (PDF)](http://www.petesqbsite.com/sections/tutorials/tuts/vbe3.pdf)

### Recommendations

* [Semantic Versioning](https://semver.org/)
* [GNU Standards](https://savannah.gnu.org/projects/gnustandards) ([package](https://pkgs.org/search/?q=gnu-standards)) - GNU coding and package maintenance standards
* [REUSE SOFTWARE](https://reuse.software/) - a set of recommendations to make licensing your projects easier
* [The Software Package Data Exchange (SPDX)](https://spdx.dev/) - an open standard for communicating software bill of material information, including components, licenses, copyrights, and security references
