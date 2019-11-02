#CANtact 

TARGET_NAME = CANtact_diag
TARGET_PATH = binary

#source files
USER_DRIVER_PATH = ./User_Drivers/Src
USER_BUILD_DIR = $(TARGET_PATH)/user
SRCS = uart.c spi.c i2c.c can.c tui.c connector_pin.c led.c diag.c system_config.c usbd_conf.c usbd_cdc_if.c  \
              usb_device.c usbd_desc.c stm32f0xx_it.c \
                      system_stm32f0xx.c 
SRCS += main.c
SRC_OBJS = $(addprefix $(USER_BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
SRC_OBJS += $(USER_BUILD_DIR)/startup_stm32f042x6.o 
SRC_INCLUDES = -IUser_Drivers/Inc 


#usb driver includes
USB_DRIVER_PATH = ./STM32_USB_Device_Library/
USB_BUILD_DIR = $(TARGET_PATH)/usb
USB_SRCS = usbd_ctlreq.c usbd_ioreq.c usbd_core.c usbd_cdc.c
USB_OBJS += $(addprefix $(USB_BUILD_DIR)/,$(notdir $(USB_SRCS:.c=.o)))
USB_INCLUDES = -ISTM32_USB_Device_Library/Core/Inc
USB_INCLUDES += -ISTM32_USB_Device_Library/Class/CDC/Inc

#stm32 hal driver includes
STM32_HAL_DRIVER_PATH = ./STM32_HAL_Drivers/STM32F0xx_HAL_Driver
CUBELIB_BUILD_DIR = $(TARGET_PATH)/STM32Cube
CUBELIB = $(CUBELIB_BUILD_DIR)/libstm32cube.a
CUBELIB_DRIVER_OBJS = $(addprefix $(CUBELIB_BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard $(STM32_HAL_DRIVER_PATH)/Src/*.c))))
CMSIS_PATH = STM32_HAL_Drivers/CMSIS
CMSIS_DEVICE_PATH = $(CMSIS_PATH)/Device/ST/STM32F0xx



#cpu type
LDSCRIPT = STM32F042C6_FLASH.ld
TARGET_DEVICE = STM32F042x6
CPU = cortex-m0

#binary
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
RANLIB = arm-none-eabi-ranlib
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
MKDIR = mkdir -p


#CFLAGS
INCLUDES = -I $(CMSIS_PATH)/Include
INCLUDES += -I $(CMSIS_DEVICE_PATH)/Include
INCLUDES += -I $(STM32_HAL_DRIVER_PATH)/Inc
INCLUDES += $(SRC_INCLUDES)
INCLUDES += $(USB_INCLUDES)

CFLAGS  = -D HSI48_VALUE=48000000 -D HSE_VALUE=16000000 -D$(TARGET_DEVICE) $(INCLUDES) 
CFLAGS += -g -std=c99 -Os 
CFLAGS += -mlittle-endian -mcpu=$(CPU) -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(TARGET_PATH)/$(TARGET_NAME).map

.PHONY: clean all cubelib usb

all: $(TARGET_PATH)/$(TARGET_NAME).elf 
flash: all
	dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(TARGET_PATH)/$(TARGET_NAME).bin

$(TARGET_PATH)/$(TARGET_NAME).elf: $(SRC_OBJS) $(USB_OBJS) $(CUBELIB) | $(USER_BUILD_DIR)
	$(CC) $(CFLAGS) -o $@  $(SRC_OBJS) $(USB_OBJS) -L$(CUBELIB_BUILD_DIR) -lstm32cube -lc -lm -T$(LDSCRIPT) 
	$(OBJCOPY) -O ihex $(TARGET_PATH)/$(TARGET_NAME).elf $(TARGET_PATH)/$(TARGET_NAME).hex
	$(OBJCOPY) -O binary $(TARGET_PATH)/$(TARGET_NAME).elf $(TARGET_PATH)/$(TARGET_NAME).bin
	$(SIZE) $(TARGET_PATH)/$(TARGET_NAME).elf

$(USER_BUILD_DIR)/%.o: $(USER_DRIVER_PATH)/%.c | $(USER_BUILD_DIR)
	$(CC) $(CFLAGS) -Os -c -o $@ $^

$(USER_BUILD_DIR)/%.o: %.c | $(USER_BUILD_DIR)
	$(CC) $(CFLAGS) -Os -c -o $@ $^

$(USER_BUILD_DIR)/%.o: $(USER_DRIVER_PATH)/%.s | $(USER_BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^

$(USER_BUILD_DIR):
	$(MKDIR) $@

clean:	
	rm -f $(TARGET_PATH)/*.o
	rm -f $(TARGET_PATH)/*.elf
	rm -f $(TARGET_PATH)/*.hex
	rm -f $(TARGET_PATH)/*.bin
	rm -f $(TARGET_PATH)/*.map

usb: $(USB_OBJS)

$(USB_BUILD_DIR)/%.o: $(USB_DRIVER_PATH)/Core/Src/%.c | $(USB_BUILD_DIR)
	$(CC) -Os $(CFLAGS) -c -o $@ $^

$(USB_BUILD_DIR)/%.o: $(USB_DRIVER_PATH)/Class/CDC/Src/%.c | $(USB_BUILD_DIR)
	$(CC) -Os $(CFLAGS) -c -o $@ $^

$(USB_BUILD_DIR):
	@echo $(USB_BUILD_DIR)
	$(MKDIR) $@

cubelib: $(CUBELIB)

$(CUBELIB): $(CUBELIB_DRIVER_OBJS)
	$(AR) rv $@ $(CUBELIB_DRIVER_OBJS)
	$(RANLIB) $@

$(CUBELIB_BUILD_DIR)/%.o: $(STM32_HAL_DRIVER_PATH)/Src/%.c | $(CUBELIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -o $@ $^

$(CUBELIB_BUILD_DIR):
	$(MKDIR) $@







	
