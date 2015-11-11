=======================================
CMake assisted automated build facility
=======================================

CMakeLists.txt top file for configuring and creating build resources
for common platforms.

Purpose
=======

The CMakeLists.txt file contained in this directory (rti_me.2.0/source) may be used in
conjunction with Cross-Platform Make utility cmake (www.cmake.org) in to configure and
generate make resources to support the creation of RTI Connext Micro libraries for
popular platforms, including Linux, Windows, Mac, and others. 

Source Overview
===============

RTI Connext Micro is built from the source code contained in the sub-directories of rti_me.2.0/source/srcC.

Applications built to utilize the RTI Connext Micro library should include the files
found in rti_me.2.0/source/include.

How to Configure and Create make resources
==========================================
CMake can be invoked either on the command line (e.g., "cmake <location of the CmakeLists.txt file>")
or using the gui (cmake-gui).

When using cmake from the command line, it is important that cmake be in the path, and not
invoked simply by specifying its location, as cmake apparently needs access to its components.

Note that if using the command line version, if changes are made to the build options,
it may be necessary to delete CMakeCache.txt in order for the new selections to take effect.

It may also be necessary to delete CMakeCache.txt when switching from command line cmake to cmake-gui,
in the same build directory.

----------------------------------------------
Building RTI Connext Micro Libraries for LINUX
----------------------------------------------

1) cd to the directory where the build results are to be placed 

2) Run cmake to create Makefile for buildable source:
 
	cmake <path to rti_me.2.0/source>

3) Run gmake to build the buildable source:

	gmake -f Makefile 

------------------------------------------------
Building RTI Connext Micro Libraries for Windows
------------------------------------------------

1) Using Graphical Interface for Cmake (cmake-gui.exe), specify the path of the buildable
   source (<path to rti_me.2.0/source>) and the destination location to build the binaries.
   
2) Select the "Configure" button to load the configuration. When queried, select the desired
   target environment (Visual Studio 10, in this example), and in the case multiple compilers
   are available on your host, ensure that the proper compilers are selected.

   Once the configuration has been loaded, a number of options should appear, with a red background.
   
3) Select the option BUILD_FOR_WINDOWS_TARGET, and ensure any other BUILD_FOR...TARGET are unselcted.
   You may also at this time select or deselect the desired options for BUILD_DEBUG_VERSION,
   BUILD_DYNAMIC_ENDPOINT_DISCOVERY, and BUILD_SHARED_LIBS.
   
   De-selecting BUILD_DYNAMIC_ENDPOINT_DISCOVERY configures for STATIC endpoint discovery. 
   
4) Select the "Configure" button to verify the configuration. When configuration and verification is
   complete, the options screen background should turn from red to white.
   
5) Select the "Generate" button to create the Visual Studio project and solution files.          

6) Launch Microsoft Visual Studio or Microsfot Visual C++, and from the destination location selected
   in step 1 above, open the project file rti_me.sln.
   
7) Right click the solution and select "Build Solution" to build RTI Connext Micro.  

------------------------------------------------
Building RTI Connext Micro Libraries for Targets
------------------------------------------------

Building Example HelloWorld to Verify Connext Micro
===================================================

If it desired to build the examples (such as the HelloWorld publisher and subscriber), it will
be necessary to update the location where the resultant library is built, and to replace the
multiple libraries in link command with the single library reference rti_me.

-------------------------------------
Building Example HelloWorld for LINUX
-------------------------------------

1) Edit the HelloWorld Makefile to change the following locations:
   - ensure RTIMEHOME is set to the proper path
   - change the location of the include files to use the buildable source
   - change the location of the library files, and change from the multiple
     shipped libraries to the single library librti_me built from the buildable source:

   INCLUDES = -I. -I$(RTIMEHOME)/source/include -I$(RTIMEHOME)/source/include/rti_me
   LIBS = -L<location of rti_me library> -lrti_me $(ARCH_LIBS)

2) Build the example:

   gmake -f Makefile RTIMEARCH=<target architecture> DEBUG=<Y|N>
   
3) If using shared libraries, set the LD_LIBRARY_PATH environment variable:

   setenv LD_LIBRARY_PATH <location of rti_me library>:${LD_LIBRARY_PATH}
   
4) Run the example:

---------------------------------------
Building Example HelloWorld for Windows
---------------------------------------

The HelloWorld example can be built on its own, or added as individual projects for
the publisher and subscriber to the Visual Studio solution created to build rti_me.
In addition to adding the source files to the projects, the following items must be
configured: 

"VC++ Directories - Include Directories" must add the path to "rti_me.2.0/source/include"

"VC++ Directories - Library Directories" must add the path to rti_me.lib created from source.

"C++ - Preprocessor - Preprocessor Definitions" must be added for RTI_WIN32,
and if linking with shared libraries, RTIME_DLL_IMPORT.

"Linker - Input - Additional Dependencies" must add the libraries rti_me.lib and ws2_32.lib.
  
In the file HelloWorldApplication.c, it is necessary to edit the line

    discovery_plugin_properties.allowed_interface = DDS_String_dup("{6E0A4753-DB2C-489C-8158-7954EE5EDF7B}");
    
and replace the interface address with the interface address from your own test machine. This address
may be found by running "regedit" on your Windows system, and looking for the registry entry for:

    HKEY_LOCAL_MACHINE - SYSTEM - CurrentControlSet - Services - Tcpip - Parameters - Interfaces
    
If there are multiple interfaces, chose the active one (such as one possessing a valid DhcpIpAddress).
 