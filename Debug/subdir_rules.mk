################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Driver" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MPU6050" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/System" -I"S:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"S:/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1815687809: ../main.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"S:/ti/ccstheia141/ccs/utils/sysconfig_1.20.0/sysconfig_cli.bat" --script "C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/main.syscfg" -o "." -s "S:/ti/mspm0_sdk_2_00_01_00/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1815687809 ../main.syscfg
device.opt: build-1815687809
device.cmd.genlibs: build-1815687809
ti_msp_dl_config.c: build-1815687809
ti_msp_dl_config.h: build-1815687809
Event.dot: build-1815687809

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Driver" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MPU6050" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/System" -I"S:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"S:/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: S:/ti/mspm0_sdk_2_00_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Driver" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MPU6050" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/System" -I"S:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"S:/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


