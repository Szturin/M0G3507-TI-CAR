################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS

GEN_OPTS__FLAG := @"./device.opt" 
GEN_CMDS__FLAG := -Wl,-l"./device_linker.cmd" 

ORDERED_OBJS += \
"./main.o" \
"./ti_msp_dl_config.o" \
"./startup_mspm0g350x_ticlang.o" \
"./Driver/JY61P.o" \
"./Driver/Key.o" \
"./Driver/OLED.o" \
"./Driver/OLED_Data.o" \
"./Driver/PID.o" \
"./Driver/PWM.o" \
"./Driver/Serial.o" \
"./Driver/motor.o" \
"./Driver/nqei.o" \
"./System/Delay.o" \
"./System/Timer.o" \
$(GEN_CMDS__FLAG) \
-Wl,-ldevice.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Driver/subdir_vars.mk
-include System/subdir_vars.mk
-include subdir_rules.mk
-include Driver/subdir_rules.mk
-include System/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
M0G3507-TI-CAR-main.out 

EXE_OUTPUTS__QUOTED += \
"M0G3507-TI-CAR-main.out" 

BIN_OUTPUTS += \
M0G3507-TI-CAR-main.txt 

BIN_OUTPUTS__QUOTED += \
"M0G3507-TI-CAR-main.txt" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "M0G3507-TI-CAR-main.out" secondary-outputs

# Tool invocations
M0G3507-TI-CAR-main.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -gdwarf-3 -Wl,-m"M0G3507-TI-CAR-main.map" -Wl,-i"S:/ti/mspm0_sdk_2_00_01_00/source" -Wl,-i"C:/Users/123/workspace_ccstheia/M0G3507-TI-CAR-main" -Wl,-i"C:/Users/123/workspace_ccstheia/M0G3507-TI-CAR-main/Debug/syscfg" -Wl,-i"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="M0G3507-TI-CAR-main_linkInfo.xml" -Wl,--rom_model -o "M0G3507-TI-CAR-main.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

M0G3507-TI-CAR-main.txt: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmhex.exe" --diag_wrap=off --ti_txt -o "M0G3507-TI-CAR-main.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.o" "ti_msp_dl_config.o" "startup_mspm0g350x_ticlang.o" "Driver\JY61P.o" "Driver\Key.o" "Driver\OLED.o" "Driver\OLED_Data.o" "Driver\PID.o" "Driver\PWM.o" "Driver\Serial.o" "Driver\motor.o" "Driver\nqei.o" "System\Delay.o" "System\Timer.o" 
	-$(RM) "main.d" "ti_msp_dl_config.d" "startup_mspm0g350x_ticlang.d" "Driver\JY61P.d" "Driver\Key.d" "Driver\OLED.d" "Driver\OLED_Data.d" "Driver\PID.d" "Driver\PWM.d" "Driver\Serial.d" "Driver\motor.d" "Driver\nqei.d" "System\Delay.d" "System\Timer.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

