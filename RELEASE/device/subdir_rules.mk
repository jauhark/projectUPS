################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --opt_for_speed=5 --fp_mode=relaxed --include_path="D:/SOFTWARES/TI/C2000Ware_4_02_00_00/libraries/control/DCL/c28/include" --include_path="D:/WORKSPACE/backup/projectUPS/libraries" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/LcdDrive" --include_path="D:/WORKSPACE/backup/projectUPS" --include_path="D:/WORKSPACE/backup/projectUPS/device" --include_path="D:/WORKSPACE/backup/projectUPS/device/driverlib" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/power_measurement" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/sfra" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/spll" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/dlog" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/emavg" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/rampgen" --include_path="D:/SOFTWARES/TI/extras/DigitalPowerSDK/C2000Ware_DigitalPower_SDK_4_01_00_00/libraries/sfra/gui/include" --include_path="D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all --define=_DEBUG --define=_FLASH --define=GCI --define=F28x_DEVICE --define=CPU1 --define=LARGE_MODEL --float_operations_allowed=32 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --opt_for_speed=5 --fp_mode=relaxed --include_path="D:/SOFTWARES/TI/C2000Ware_4_02_00_00/libraries/control/DCL/c28/include" --include_path="D:/WORKSPACE/backup/projectUPS/libraries" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/LcdDrive" --include_path="D:/WORKSPACE/backup/projectUPS" --include_path="D:/WORKSPACE/backup/projectUPS/device" --include_path="D:/WORKSPACE/backup/projectUPS/device/driverlib" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/power_measurement" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/sfra" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/spll" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/dlog" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/emavg" --include_path="D:/WORKSPACE/backup/projectUPS/libraries/utilities/rampgen" --include_path="D:/SOFTWARES/TI/extras/DigitalPowerSDK/C2000Ware_DigitalPower_SDK_4_01_00_00/libraries/sfra/gui/include" --include_path="D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all --define=_DEBUG --define=_FLASH --define=GCI --define=F28x_DEVICE --define=CPU1 --define=LARGE_MODEL --float_operations_allowed=32 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


