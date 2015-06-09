#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/minisat/core/Solver.o \
	${OBJECTDIR}/lib/minisat/simp/SimpSolver.o \
	${OBJECTDIR}/lib/minisat/utils/Options.o \
	${OBJECTDIR}/lib/minisat/utils/System.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/AllSat.o \
	${OBJECTDIR}/src/CNF.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/allsat

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/allsat: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/allsat ${OBJECTFILES} ${LDLIBSOPTIONS} -lz

${OBJECTDIR}/lib/minisat/core/Solver.o: lib/minisat/core/Solver.cc 
	${MKDIR} -p ${OBJECTDIR}/lib/minisat/core
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/minisat/core/Solver.o lib/minisat/core/Solver.cc

${OBJECTDIR}/lib/minisat/simp/SimpSolver.o: lib/minisat/simp/SimpSolver.cc 
	${MKDIR} -p ${OBJECTDIR}/lib/minisat/simp
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/minisat/simp/SimpSolver.o lib/minisat/simp/SimpSolver.cc

${OBJECTDIR}/lib/minisat/utils/Options.o: lib/minisat/utils/Options.cc 
	${MKDIR} -p ${OBJECTDIR}/lib/minisat/utils
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/minisat/utils/Options.o lib/minisat/utils/Options.cc

${OBJECTDIR}/lib/minisat/utils/System.o: lib/minisat/utils/System.cc 
	${MKDIR} -p ${OBJECTDIR}/lib/minisat/utils
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/minisat/utils/System.o lib/minisat/utils/System.cc

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/AllSat.o: src/AllSat.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AllSat.o src/AllSat.cpp

${OBJECTDIR}/src/CNF.o: src/CNF.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CNF.o src/CNF.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/allsat

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
