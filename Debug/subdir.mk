################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BabyOrangutanMotors.cpp \
../GlobalTimer.cpp \
../HardwareSerial.cpp \
../Math.cpp \
../PWMMotorDigitalReverse.cpp \
../PololuEncoder.cpp \
../Print.cpp \
../ServoTimer.cpp \
../Sonar.cpp \
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
./BabyOrangutanMotors.o \
./GlobalTimer.o \
./HardwareSerial.o \
./Math.o \
./PWMMotorDigitalReverse.o \
./PololuEncoder.o \
./Print.o \
./ServoTimer.o \
./Sonar.o \
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
./BabyOrangutanMotors.d \
./GlobalTimer.d \
./HardwareSerial.d \
./Math.d \
./PWMMotorDigitalReverse.d \
./PololuEncoder.d \
./Print.d \
./ServoTimer.d \
./Sonar.d \
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
	avr-gcc -I/usr/local/avr/avr/include/avr -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


