################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HardwareSerial.cpp \
../Math.cpp \
../PWMMotorDigitalReverse.cpp \
../PololuEncoder.cpp \
../Print.cpp \
../ServoTimer.cpp \
../Stream.cpp \
../WString.cpp \
../cppstub.cpp \
../hw_spi.cpp \
../nRF24L01.cpp \
../nRF24L01_helper.cpp \
../pcint_dispatch.cpp \
../pin_to_addr.cpp \
../usart_spi.cpp 

OBJS += \
./HardwareSerial.o \
./Math.o \
./PWMMotorDigitalReverse.o \
./PololuEncoder.o \
./Print.o \
./ServoTimer.o \
./Stream.o \
./WString.o \
./cppstub.o \
./hw_spi.o \
./nRF24L01.o \
./nRF24L01_helper.o \
./pcint_dispatch.o \
./pin_to_addr.o \
./usart_spi.o 

CPP_DEPS += \
./HardwareSerial.d \
./Math.d \
./PWMMotorDigitalReverse.d \
./PololuEncoder.d \
./Print.d \
./ServoTimer.d \
./Stream.d \
./WString.d \
./cppstub.d \
./hw_spi.d \
./nRF24L01.d \
./nRF24L01_helper.d \
./pcint_dispatch.d \
./pin_to_addr.d \
./usart_spi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I/usr/avr/include/avr -Wall -g3 -gstabs -Os -fpack-struct -fshort-enums -mrelax -ffunction-sections  -fdata-sections -Wl,--gc-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


