# Global COSMOS qmake include file
# Include this file if you want to use COSMOS within Qt Creator
# if you are compiling with a GCC compiler we suggest that you add '-j'
# to the make arguments to speed up the compilation time


message(" ")
message(" -- COSMOS.pri --")
#--------------------------------------------------------------------
# Windows config
#--------------------------------------------------------------------
win32 {
    message( "Building on Win32" )

    #QMAKE_CXXFLAGS += -std=c++0x -pthread

    # add libraries for MinGW
    *-g++* {
        message("Compiler: MinGW")
        LIBS += -lpthread -lwsock32 -lwinmm -lws2_32 -liphlpapi
        QMAKE_CXXFLAGS += -Wall -pedantic -std=c++11 -pthread
    }

    *-msvc* {
        message("Compiler: MSVC")
        LIBS += -lwsock32 -lwinmm -lws2_32 -liphlpapi

        QMAKE_CXXFLAGS += -W4 -D_CRT_NONSTDC_NO_DEPRECATE

        # available modules for MSVC 2013
        #MODULES += elapsedtime
        #MODULES += timeutils
        #MODULES += socketlib
        #MODULES += agentlib
        #MODULES += jsonlib
        #MODULES += mathlib
        #MODULES += timelib
        #MODULES += datalib
        #MODULES += convertlib
        #MODULES += convertdef
        #MODULES += stringlib
        #MODULES += jpleph
        #MODULES += ephemlib
        #MODULES += geomag
        #MODULES += sliplib
        #MODULES += zlib


        # include dirent for MSVC
        INCLUDEPATH     += $$COSMOS/core/libraries/thirdparty/dirent
        SOURCES         += $$files($$COSMOS/core/libraries/thirdparty/dirent/*.c)
        HEADERS         += $$files($$COSMOS/core/libraries/thirdparty/dirent/*.h)
    }

}

#contains(QMAKE_CC, cl){
#    # Visual Studio
#    message("Compiler: Visual Studio")
#    #CONFIG += precompile_header

#    win32{
#    INCLUDEPATH     += $$COSMOS/core/libraries/thirdparty/dirent
#    SOURCES         += $$files($$COSMOS/core/libraries/thirdparty/dirent/*.c)
#    HEADERS         += $$files($$COSMOS/core/libraries/thirdparty/dirent/*.h)
#    }
#    #SOURCES += $$COSMOS_CORE/programs/agents/agent_soh.cpp

#    #LIBS += -lpthread -lwsock32 -lwinmm -lws2_32 -liphlpapi
#    LIBS += -lws2_32 -lwsock32 -liphlpapi

#}



################################################################################
# Mac config
################################################################################
macx { #mac
    message( "Building on MAC OS X" )
}


################################################################################
# Unix config
################################################################################
unix:!macx{
    message( "Building on Unix" )
    # add libraries
    LIBS += -pthread #-ljpeg
}

message("")
message("COSMOS Modules >>")

#--------------------------------------------------------------------
#add COSMOS support to the path
INCLUDEPATH     += $$COSMOS/core/libraries/support

# Add all COSMOS support libraries
contains(MODULES, SUPPORT){
    message( "Add library: SUPPORT" )
    INCLUDEPATH     += $$COSMOS/core/libraries/support
    #SOURCES         += $$files($$COSMOS/core/libraries/support/*.cpp)
    #HEADERS         += $$files($$COSMOS/core/libraries/support/*.h)
    MODULES += ZLIB
}

contains(MODULES, elapsedtime){
    message( "- support/elapsedtime" )
    SOURCES     += $$COSMOS/core/libraries/support/elapsedtime.cpp
    HEADERS     += $$COSMOS/core/libraries/support/elapsedtime.hpp
}

contains(MODULES, timeutils){
    message( "- support/timeutils" )
    SOURCES += $$COSMOS/core/libraries/support/timeutils.cpp
    HEADERS += $$COSMOS/core/libraries/support/timeutils.h
}

contains(MODULES, socketlib){
    message( "- support/socketlib" )
    SOURCES += $$COSMOS/core/libraries/support/socketlib.cpp
    HEADERS += $$COSMOS/core/libraries/support/socketlib.h
}

contains(MODULES, agentlib){
    message( "- support/agentlib" )
    SOURCES += $$COSMOS/core/libraries/support/agentlib.cpp
    HEADERS += $$COSMOS/core/libraries/support/agentlib.h
}

contains(MODULES, jsondef){
    message( "- support/jsondef" )
    HEADERS += $$COSMOS/core/libraries/support/jsondef.h
}

contains(MODULES, jsonlib){
    message( "- support/jsonlib" )
    SOURCES += $$COSMOS/core/libraries/support/jsonlib.cpp
    HEADERS += $$COSMOS/core/libraries/support/jsonlib.h
}

contains(MODULES, mathlib){
    message( "- support/mathlib" )
    SOURCES += $$COSMOS/core/libraries/support/mathlib.cpp
    HEADERS += $$COSMOS/core/libraries/support/mathlib.h
}

contains(MODULES, timelib){
    message( "- support/timelib" )
    SOURCES += $$COSMOS/core/libraries/support/timelib.cpp
    HEADERS += $$COSMOS/core/libraries/support/timelib.h
}

contains(MODULES, datalib){
    message( "- support/datalib" )
    SOURCES += $$COSMOS/core/libraries/support/datalib.cpp
    HEADERS += $$COSMOS/core/libraries/support/datalib.h
}

contains(MODULES, convertlib){
    message( "- support/convertlib" )
    SOURCES += $$COSMOS/core/libraries/support/convertlib.cpp
    HEADERS += $$COSMOS/core/libraries/support/convertlib.h
}

contains(MODULES, convertdef){
    message( "- support/convertdef" )
    HEADERS += $$COSMOS/core/libraries/support/convertdef.h
}

contains(MODULES, stringlib){
    message( "- support/stringlib" )
    SOURCES         += $$COSMOS/core/libraries/support/stringlib.cpp
    HEADERS         += $$COSMOS/core/libraries/support/stringlib.h
}

contains(MODULES, jpleph){
    message( "- support/jpleph" )
    SOURCES         += $$COSMOS/core/libraries/support/jpleph.cpp
    HEADERS         += $$COSMOS/core/libraries/support/jpleph.h
}

contains(MODULES, ephemlib){
    message( "- support/ephemlib" )
    SOURCES         += $$COSMOS/core/libraries/support/ephemlib.cpp
    HEADERS         += $$COSMOS/core/libraries/support/ephemlib.h
}

contains(MODULES, geomag){
    message( "- support/geomag" )
    SOURCES         += $$COSMOS/core/libraries/support/geomag.cpp
    HEADERS         += $$COSMOS/core/libraries/support/geomag.h
}

contains(MODULES, sliplib){
    message( "- support/support/sliplib" )
    SOURCES         += $$COSMOS/core/libraries/support/sliplib.cpp
    HEADERS         += $$COSMOS/core/libraries/support/sliplib.h
}

contains(MODULES, qtsupport){
    message( "- source/tools/libraries/qtsupport" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qtsupport
    MODULES += cosmosdata
    MODULES += cosmosdatum
    MODULES += event
}

contains(MODULES, cosmosdata){
    message( "- tools/libraries/qtsupport/cosmosdata" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qtsupport
    SOURCES += $$COSMOS/tools/libraries/qtsupport/cosmosdata.cpp
    HEADERS += $$COSMOS/tools/libraries/qtsupport/cosmosdata.h
}

contains(MODULES, cosmosdatum){
    message( "- tools/libraries/qtsupport/cosmosdatum" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qtsupport
    SOURCES += $$COSMOS/tools/libraries/qtsupport/cosmosdatum.cpp
    HEADERS += $$COSMOS/tools/libraries/qtsupport/cosmosdatum.h
}

contains(MODULES, event){
    message( "- tools/libraries/qtsupport/event" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qtsupport
    SOURCES += $$COSMOS/tools/libraries/qtsupport/event.cpp
    HEADERS += $$COSMOS/tools/libraries/qtsupport/event.h
}

contains(MODULES, qmlsupport){
    message( "- tools/libraries/qmlsupport" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qmlsupport
    MODULES += sharedobjectlist
    MODULES += miscqmlsupport
}

contains(MODULES, sharedobjectlist){
    message( "- tools/libraries/qmlsupport/sharedobjectlist" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qmlsupport
    SOURCES += $$COSMOS/tools/libraries/qmlsupport/sharedobjectlist.cpp
    HEADERS += $$COSMOS/tools/libraries/qmlsupport/sharedobjectlist.h
}

contains(MODULES, miscqmlsupport){
    message( "- tools/libraries/qmlsupport/sharedobjectlist" )
    INCLUDEPATH     += $$COSMOS/tools/libraries/qmlsupport
    SOURCES += $$COSMOS/tools/libraries/qmlsupport/miscqmlsupport.cpp
    HEADERS += $$COSMOS/tools/libraries/qmlsupport/miscqmlsupport.h
}


#--------------------------------------------------------------------
# Add COSMOS device
contains(MODULES, DEVICE){
    message( "Add library: DEVICES" )
    INCLUDEPATH     += $$COSMOS/core/libraries/device
    SOURCES         += $$files($$COSMOS/core/libraries/device/*.cpp)
    HEADERS         += $$files($$COSMOS/core/libraries/device/*.h)
}



#--------------------------------------------------------------------
# Add COSMOS core thirdparty libraries
INCLUDEPATH     += $$COSMOS/core/libraries/thirdparty
#INCLUDEPATH     += $$COSMOS/thirdparty

# Add Zlib
contains(MODULES, zlib){
    message( "- thirdparty/zlib" )
    INCLUDEPATH     += $$COSMOS/core/libraries/thirdparty/zlib
    SOURCES         += $$files($$COSMOS/core/libraries/thirdparty/zlib/*.c)
    HEADERS         += $$files($$COSMOS/core/libraries/thirdparty/zlib/*.h)
}

# Add JPEG
contains(MODULES, jpeg){
    message( "- thirdparty/jpeg" )
    INCLUDEPATH     += $$COSMOS/core/libraries/thirdparty/jpeg
    SOURCES         += $$files($$COSMOS/core/libraries/thirdparty/jpeg/*.c)
    HEADERS         += $$files($$COSMOS/core/libraries/thirdparty/jpeg/*.h)
}

