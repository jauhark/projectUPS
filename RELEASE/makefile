################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./project_functions.obj" \
"./project_main.obj" \
"./device/device.obj" \
"./device/f28004x_codestartbranch.obj" \
"./libraries/DCL/source/DCL_DF22_C1.obj" \
"./libraries/sfra/sfra_gui_scicomms_driverlib.obj" \
"../device/gridconnectedinvlclfltr_flash_cpu1.cmd" \
"../libraries/FPUfastRTS/rts2800_fpu32_fast_supplement.lib" \
"../libraries/FPUfastRTS/rts2800_fpu32_fast_supplement_eabi.lib" \
"../libraries/sfra/sfra_f32_eabi.lib" \
$(GEN_CMDS__FLAG) \
-l"D:/WORKSPACE/TI2/projectUPS/libraries/sfra/sfra_f32_eabi.lib" \
-ldriverlib_eabi.lib \
-lrts2800_fpu32_eabi.lib \
-lrts2800_fpu32_fast_supplement_eabi.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include device/subdir_vars.mk
-include libraries/DCL/source/subdir_vars.mk
-include libraries/FPUfastRTS/subdir_vars.mk
-include libraries/sfra/subdir_vars.mk
-include subdir_rules.mk
-include device/subdir_rules.mk
-include libraries/DCL/source/subdir_rules.mk
-include libraries/FPUfastRTS/subdir_rules.mk
-include libraries/sfra/subdir_rules.mk
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
projectUPS.out \

EXE_OUTPUTS__QUOTED += \
"projectUPS.out" \

BIN_OUTPUTS += \
projectUPS.hex \

BIN_OUTPUTS__QUOTED += \
"projectUPS.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "projectUPS.out"

# Tool invocations
projectUPS.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O2 --opt_for_speed=5 --fp_mode=relaxed --advice:performance=all --define=_DEBUG --define=_FLASH --define=GCI --define=F28x_DEVICE --define=CPU1 --define=LARGE_MODEL --float_operations_allowed=32 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"projectUPS.map" --stack_size=0x200 --warn_sections -i"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/lib" -i"D:/WORKSPACE/TI2/projectUPS/libraries/spll" -i"D:/WORKSPACE/TI2/projectUPS/libraries/sfra" -i"D:/WORKSPACE/TI2/projectUPS/libraries/FPUfastRTS" -i"D:/WORKSPACE/TI2/projectUPS/device/driverlib/ccs/Release" -i"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="projectUPS_linkInfo.xml" --entry_point=code_start --rom_model -o "projectUPS.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

projectUPS.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/SOFTWARES/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/hex2000" --diag_wrap=off -o "projectUPS.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "project_functions.obj" "project_main.obj" "device\device.obj" "device\f28004x_codestartbranch.obj" "libraries\DCL\source\DCL_DF22_C1.obj" "libraries\sfra\sfra_gui_scicomms_driverlib.obj" 
	-$(RM) "project_functions.d" "project_main.d" "device\device.d" "libraries\sfra\sfra_gui_scicomms_driverlib.d" 
	-$(RM) "device\f28004x_codestartbranch.d" "libraries\DCL\source\DCL_DF22_C1.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

