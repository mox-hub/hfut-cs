## Makefile.am for pursuit server

## define the program that has to be built and the related sources
bin_PROGRAMS = trilearn_player trilearn_coach 

SOURCES =         Connection.cpp \
        SenseHandler.cpp \
        ActHandler.cpp \
        SoccerTypes.cpp \
        Objects.cpp \
        WorldModel.cpp \
        WorldModelHighLevel.cpp \
        WorldModelPredict.cpp \
        WorldModelUpdate.cpp \
        ServerSettings.cpp \
        PlayerSettings.cpp \
        GenericValues.cpp \
        Formations.cpp \
        Geometry.cpp \
        Parse.cpp \
        Logger.cpp \
	Connection.h \
        SenseHandler.h \
        ActHandler.h \
        SoccerTypes.h \
        Objects.h \
        WorldModel.h \
        ServerSettings.h \
        PlayerSettings.h \
        GenericValues.h \
        Formations.h \
        Geometry.h \
        Parse.h \
        Logger.h 

trilearn_player_SOURCES = ${SOURCES}    \
        BasicPlayer.cpp \
        Player.cpp \
        PlayerTeams.cpp \
        main.cpp \
        BasicPlayer.h \
        Player.h 

trilearn_coach_SOURCES = ${SOURCES} \
        BasicCoach.cpp \
        BasicCoach.h \
        mainCoach.cpp 

## also distribute configuration file with distribution 
EXTRA_DIST = formations.conf player.conf

