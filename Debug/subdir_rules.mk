################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
buffer.obj: G:/Spaghetti/Software/buffer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="buffer.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

buffer_printf.obj: G:/Spaghetti/Software/buffer_printf.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="buffer_printf.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

major.obj: ../major.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="major.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

midi.obj: ../midi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="midi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

motor.obj: ../motor.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="motor.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pwm.obj: E:/ti/TivaWare_C_Series-2.1.0.12573/driverlib/pwm.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="pwm.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sensor.obj: ../sensor.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="sensor.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

subsys.obj: G:/Spaghetti/Software/subsys.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="subsys.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

task.obj: G:/Spaghetti/Software/task.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="task.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timing.obj: G:/Spaghetti/Software/timing.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="timing.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: G:/Spaghetti/Software/uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="G:/Projects/Rowan/TeslaGallery" --include_path="G:/Spaghetti/Software" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=ccs="ccs" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


