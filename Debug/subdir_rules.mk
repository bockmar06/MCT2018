################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/opt/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__MSP432P401R__ -Dgcc -I"/home/student/git/workspace_mct/YAHAL/include/interface" -I"/home/student/git/workspace_mct/YAHAL/src/driver" -I"/home/student/git/workspace_mct/YAHAL/src/platform/msp432" -I"/home/student/git/workspace_mct/YAHAL/src/task" -I"/home/student/git/workspace_mct/YAHAL/src/util" -I"/home/student/git/workspace_mct/YAHAL/src/uGUI" -I"/home/student/git/workspace_mct/YAHAL/src/uGUI/fonts" -I"/opt/ti/ccsv7/ccs_base/arm/include" -I"/opt/ti/ccsv7/ccs_base/arm/include/CMSIS" -I"/media/sf_MCT2018" -I"/opt/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/opt/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__MSP432P401R__ -Dgcc -I"/home/student/git/workspace_mct/YAHAL/include/interface" -I"/home/student/git/workspace_mct/YAHAL/src/driver" -I"/home/student/git/workspace_mct/YAHAL/src/platform/msp432" -I"/home/student/git/workspace_mct/YAHAL/src/task" -I"/home/student/git/workspace_mct/YAHAL/src/util" -I"/home/student/git/workspace_mct/YAHAL/src/uGUI" -I"/home/student/git/workspace_mct/YAHAL/src/uGUI/fonts" -I"/opt/ti/ccsv7/ccs_base/arm/include" -I"/opt/ti/ccsv7/ccs_base/arm/include/CMSIS" -I"/media/sf_MCT2018" -I"/opt/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


