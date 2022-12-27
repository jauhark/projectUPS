################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
libraries/DCL/source/%.obj: ../libraries/DCL/source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --opt_for_speed=0 --fp_mode=relaxed --include_path="D:/WORKSPACE/TI2/projectUPS" --include_path="D:/WORKSPACE/TI2/projectUPS/device" --include_path="D:/WORKSPACE/TI2/projectUPS/device/driverlib" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/DCL" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/power_measurement" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/sfra" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/spll" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/utilities/dlog" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/utilities/emavg" --include_path="D:/WORKSPACE/TI2/projectUPS/libraries/utilities/rampgen" --include_path="D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all --define=_DEBUG --define=_FLASH --define=GCI --define=F28x_DEVICE --define=CPU1 --define=LARGE_MODEL --float_operations_allowed=32 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="libraries/DCL/source/$(basename $(<F)).d_raw" --obj_directory="libraries/DCL/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


