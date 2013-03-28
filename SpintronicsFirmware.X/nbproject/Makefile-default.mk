#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/traps.o ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o ${OBJECTDIR}/_ext/1360937237/i2sDrv.o ${OBJECTDIR}/_ext/1360937237/uartDrv.o ${OBJECTDIR}/_ext/1360937237/spiDrv.o ${OBJECTDIR}/_ext/1360937237/asm.o ${OBJECTDIR}/_ext/1360937237/cosLUT.o ${OBJECTDIR}/_ext/1360937237/CS4272config.o ${OBJECTDIR}/_ext/1360937237/muxControl.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/traps.o.d ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d ${OBJECTDIR}/_ext/1360937237/uartDrv.o.d ${OBJECTDIR}/_ext/1360937237/spiDrv.o.d ${OBJECTDIR}/_ext/1360937237/asm.o.d ${OBJECTDIR}/_ext/1360937237/cosLUT.o.d ${OBJECTDIR}/_ext/1360937237/CS4272config.o.d ${OBJECTDIR}/_ext/1360937237/muxControl.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/traps.o ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o ${OBJECTDIR}/_ext/1360937237/i2sDrv.o ${OBJECTDIR}/_ext/1360937237/uartDrv.o ${OBJECTDIR}/_ext/1360937237/spiDrv.o ${OBJECTDIR}/_ext/1360937237/asm.o ${OBJECTDIR}/_ext/1360937237/cosLUT.o ${OBJECTDIR}/_ext/1360937237/CS4272config.o ${OBJECTDIR}/_ext/1360937237/muxControl.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MC806
MP_LINKER_FILE_OPTION=,-Tp33EP512MC806.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/asm.o: ../src/asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/asm.o.ok ${OBJECTDIR}/_ext/1360937237/asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/asm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  ../src/asm.s -o ${OBJECTDIR}/_ext/1360937237/asm.o -omf=coff -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_PK3=1 -g  -MD "${OBJECTDIR}/_ext/1360937237/asm.o.d" -I".." -g$(MP_EXTRA_AS_POST)
	
else
${OBJECTDIR}/_ext/1360937237/asm.o: ../src/asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/asm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/asm.o.ok ${OBJECTDIR}/_ext/1360937237/asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/asm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  ../src/asm.s -o ${OBJECTDIR}/_ext/1360937237/asm.o -omf=coff -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 -g  -MD "${OBJECTDIR}/_ext/1360937237/asm.o.d" -I".." -g$(MP_EXTRA_AS_POST)
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.ok ${OBJECTDIR}/_ext/1360937237/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/traps.o: ../src/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/traps.o.ok ${OBJECTDIR}/_ext/1360937237/traps.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/traps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/traps.o.d" -o ${OBJECTDIR}/_ext/1360937237/traps.o ../src/traps.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o: ../src/generateAndProcessSamples.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.ok ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d" -o ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o ../src/generateAndProcessSamples.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/i2sDrv.o: ../src/i2sDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.ok ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/i2sDrv.o ../src/i2sDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/uartDrv.o: ../src/uartDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uartDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uartDrv.o.ok ${OBJECTDIR}/_ext/1360937237/uartDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uartDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/uartDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/uartDrv.o ../src/uartDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/spiDrv.o: ../src/spiDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spiDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spiDrv.o.ok ${OBJECTDIR}/_ext/1360937237/spiDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spiDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/spiDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/spiDrv.o ../src/spiDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/cosLUT.o: ../src/cosLUT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cosLUT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cosLUT.o.ok ${OBJECTDIR}/_ext/1360937237/cosLUT.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/cosLUT.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/cosLUT.o.d" -o ${OBJECTDIR}/_ext/1360937237/cosLUT.o ../src/cosLUT.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/CS4272config.o: ../src/CS4272config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CS4272config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CS4272config.o.ok ${OBJECTDIR}/_ext/1360937237/CS4272config.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/CS4272config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/CS4272config.o.d" -o ${OBJECTDIR}/_ext/1360937237/CS4272config.o ../src/CS4272config.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/muxControl.o: ../src/muxControl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/muxControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/muxControl.o.ok ${OBJECTDIR}/_ext/1360937237/muxControl.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/muxControl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/muxControl.o.d" -o ${OBJECTDIR}/_ext/1360937237/muxControl.o ../src/muxControl.c    -fast-math
	
else
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.ok ${OBJECTDIR}/_ext/1360937237/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/traps.o: ../src/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/traps.o.ok ${OBJECTDIR}/_ext/1360937237/traps.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/traps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/traps.o.d" -o ${OBJECTDIR}/_ext/1360937237/traps.o ../src/traps.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o: ../src/generateAndProcessSamples.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.ok ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o.d" -o ${OBJECTDIR}/_ext/1360937237/generateAndProcessSamples.o ../src/generateAndProcessSamples.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/i2sDrv.o: ../src/i2sDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.ok ${OBJECTDIR}/_ext/1360937237/i2sDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/i2sDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/i2sDrv.o ../src/i2sDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/uartDrv.o: ../src/uartDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uartDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uartDrv.o.ok ${OBJECTDIR}/_ext/1360937237/uartDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uartDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/uartDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/uartDrv.o ../src/uartDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/spiDrv.o: ../src/spiDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spiDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spiDrv.o.ok ${OBJECTDIR}/_ext/1360937237/spiDrv.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spiDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/spiDrv.o.d" -o ${OBJECTDIR}/_ext/1360937237/spiDrv.o ../src/spiDrv.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/cosLUT.o: ../src/cosLUT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cosLUT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cosLUT.o.ok ${OBJECTDIR}/_ext/1360937237/cosLUT.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/cosLUT.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/cosLUT.o.d" -o ${OBJECTDIR}/_ext/1360937237/cosLUT.o ../src/cosLUT.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/CS4272config.o: ../src/CS4272config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CS4272config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CS4272config.o.ok ${OBJECTDIR}/_ext/1360937237/CS4272config.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/CS4272config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/CS4272config.o.d" -o ${OBJECTDIR}/_ext/1360937237/CS4272config.o ../src/CS4272config.c    -fast-math
	
${OBJECTDIR}/_ext/1360937237/muxControl.o: ../src/muxControl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/muxControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/muxControl.o.ok ${OBJECTDIR}/_ext/1360937237/muxControl.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/muxControl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"../../h" -I"../h" -MMD -MF "${OBJECTDIR}/_ext/1360937237/muxControl.o.d" -o ${OBJECTDIR}/_ext/1360937237/muxControl.o ../src/muxControl.c    -fast-math
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files/Microchip/MPLAB\ C30/lib/libq-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libc-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libc-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libfastm-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libfastm-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libm-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libm-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libpic30-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libpic30-elf.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "C:\Program Files\Microchip\MPLAB C30\lib\libq-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libq-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libc-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libc-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libfastm-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libfastm-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libm-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libm-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libpic30-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libpic30-elf.a"     -fast-math -Wl,--defsym=__MPLAB_BUILD=1,-L"../../lib",-Map="SpintronicsFirmware.X.${IMAGE_TYPE}.map",--report-mem,--defsym=__ICD2RAM=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files/Microchip/MPLAB\ C30/lib/libq-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libc-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libc-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libfastm-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libfastm-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libm-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libm-elf.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libpic30-coff.a C:/Program\ Files/Microchip/MPLAB\ C30/lib/libpic30-elf.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "C:\Program Files\Microchip\MPLAB C30\lib\libq-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libq-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libc-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libc-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libfastm-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libfastm-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libm-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libm-elf.a" "C:\Program Files\Microchip\MPLAB C30\lib\libpic30-coff.a" "C:\Program Files\Microchip\MPLAB C30\lib\libpic30-elf.a"     -fast-math -Wl,--defsym=__MPLAB_BUILD=1,-L"../../lib",-Map="SpintronicsFirmware.X.${IMAGE_TYPE}.map",--report-mem,--defsym=__ICD2RAM=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/SpintronicsFirmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=coff
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
