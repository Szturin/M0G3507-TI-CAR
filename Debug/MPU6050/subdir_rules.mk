################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MPU6050/%.o: ../MPU6050/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"S:/ti/ccstheia141/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/Driver" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/MPU6050" -I"C:/Users/123/workspace_ccstheia/empty_LP_MSPM0G3507_nortos_ticlang/System" -I"S:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"S:/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"MPU6050/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


