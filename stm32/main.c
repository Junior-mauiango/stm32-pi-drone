#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
//BASE registers
#define SCB_CPACR (*(volatile unsigned int*)0xE000ED88)
#define RCC_BASE 0x40023800UL
#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL
#define GPIOF_BASE 0x40021400UL
#define GPIOG_BASE 0x40021800UL
#define GPIOH_BASE 0x40021C00UL
#define TIM2_BASE 0x40000000UL //TIM2 timer
#define TIM3_BASE 0x40000400UL
#define USART1_BASE 0x40011000UL
#define TIM4_BASE 0x40000800UL
#define ADC1_BASE 0x40012000UL
#define I2C1_BASE 0x40005400
#define USART2_BASE 0x40004400UL
#define PI 3.14159265358979323846f

//RCC registers
#define RCC_AHB1ENR (*(volatile unsigned int*)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int*)(RCC_BASE + 0x40))// yo enable clock for I2c1
#define RCC_APB2ENR (*(volatile unsigned int*)(RCC_BASE + 0x44))

//GPIOA registers
#define GPIOA_MODER (*(volatile unsigned int*)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile unsigned int*)(GPIOA_BASE + 0x14))
#define GPIOA_BSRR  (*(volatile unsigned int*)(GPIOA_BASE + 0x18))
#define GPIOA_PUPDR (*(volatile unsigned int*)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR (*(volatile unsigned int*)(GPIOA_BASE + 0x10))
#define GPIOA_AFRL (*(volatile unsigned int*)(GPIOA_BASE + 0x20))//to select alternate function for PA)
#define GPIOA_AFRH (*(volatile unsigned int*)(GPIOA_BASE + 0x24))

//GPIOB registers
#define GPIOB_MODER (*(volatile unsigned int*)(GPIOB_BASE + 0x00))
#define GPIOB_ODR (*(volatile unsigned int*)(GPIOB_BASE + 0x14))
#define GPIOB_BSRR (*(volatile unsigned int*)(GPIOB_BASE + 0x18))
#define GPIOB_PUPDR (*(volatile unsigned int*)(GPIOB_BASE + 0x0C))
#define GPIOB_IDR (*(volatile unsigned int*)(GPIOB_BASE + 0x10))
#define GPIOB_AFRL (*(volatile unsigned int*)(GPIOB_BASE + 0x20))
#define GPIOB_AFRH (*(volatile unsigned int*)(GPIOB_BASE + 0x24))
#define GPIOB_OTYPER (*(volatile unsigned int*)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR (*(volatile unsigned int*)(GPIOB_BASE + 0x08))

//TIM2 registers
#define TIM2_CR1 (*(volatile unsigned int*)(TIM2_BASE + 0x00))
#define TIM2_CCMR1 (*(volatile unsigned int*)(TIM2_BASE + 0x18))
#define TIM2_CCER (*(volatile unsigned int*)(TIM2_BASE + 0x20))
#define TIM2_PSC (*(volatile unsigned int*)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile unsigned int*)(TIM2_BASE + 0x2C))
#define TIM2_CCR1 (*(volatile unsigned int*)(TIM2_BASE + 0x34))
#define TIM2_CCMR2 (*(volatile unsigned int*)(TIM2_BASE + 0x1C))
#define TIM2_CCR2 (*(volatile unsigned int*)(TIM2_BASE + 0x38))
#define TIM2_CCR3 (*(volatile unsigned int*)(TIM2_BASE + 0x3C))
#define TIM2_SR (*(volatile unsigned int *)(TIM2_BASE + 0x10))
#define TIM2_EGR (*(volatile unsigned int *)(TIM2_BASE + 0x14))
#define TIM2_CCR4 (*(volatile unsigned int*)(TIM2_BASE + 0x40))

//I2C1 registers
#define I2C1_CR1 (*(volatile unsigned int *)(I2C1_BASE + 0x00))
#define I2C1_CR2 (*(volatile unsigned int *)(I2C1_BASE + 0x04))
#define I2C1_OAR1 (*(volatile unsigned int *)(I2C1_BASE + 0x08))
#define I2C1_OAR2 (*(volatile unsigned int *)(I2C1_BASE + 0x0C))
#define I2C1_DR (*(volatile unsigned int *)(I2C1_BASE + 0x10))
#define I2C1_SR1 (*(volatile unsigned int *)(I2C1_BASE + 0x14))
#define I2C1_SR2 (*(volatile unsigned int *)(I2C1_BASE + 0x18))
#define I2C1_CCR (*(volatile unsigned int *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE (*(volatile unsigned int *)(I2C1_BASE + 0x20))
#define I2C1_FLTR (*(volatile unsigned int *)(I2C1_BASE + 0x24))

//USART2 registers
#define USART2_SR   (*(volatile unsigned int*)(USART2_BASE + 0x00))
#define USART2_DR   (*(volatile unsigned int*)(USART2_BASE + 0x04))
#define USART2_BRR  (*(volatile unsigned int*)(USART2_BASE + 0x08))
#define USART2_CR1  (*(volatile unsigned int*)(USART2_BASE + 0x0C))
#define USART2_CR2  (*(volatile unsigned int*)(USART2_BASE + 0x10))
#define USART2_CR3  (*(volatile unsigned int*)(USART2_BASE + 0x14))

//USART1 registers
#define USART1_SR (*(volatile unsigned int*)(USART1_BASE + 0x00))
#define USART1_DR (*(volatile unsigned int*)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile unsigned int*)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile unsigned int*)(USART1_BASE + 0x0C))
#define NVIC_ISER1 (*(volatile uint32_t*)0xE000E104)

#define SYST_CSR (*(volatile unsigned int*)0xE000E010)//turn it on or off
#define SYST_RVR (*(volatile unsigned int*)0xE000E014)// this is where counter start
#define SYST_CVR (*(volatile unsigned int*)0xE000E018)//current value

//registers
// Start reading from ACCEL_XOUT_H
uint8_t sensor_reg[1] = {0x3B};
uint8_t data_reg[1] = {0xF7};
uint8_t calib_reg[1] = {0x88};

unsigned int duty = 500;
int16_t accel_x_bias = 0;
int16_t accel_y_bias = 0;
int16_t accel_z_bias = 0;

int16_t gyro_x_bias = 0;
int16_t gyro_y_bias = 0;
int16_t gyro_z_bias = 0;

// 14-byte buffer
uint8_t sensor_data_bmp280[6];
uint8_t sensor_data_mpu[14];
uint8_t calibration_data[24];
int16_t accel_x;
int16_t accel_y;
int16_t accel_z;
int32_t t_fine;

int16_t temperature;
int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;

//raw bmp280 calibration bits
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

float roll = 0.0f;
float pitch = 0.0f;
float alpha = 0.98f;
float dt = 0.1f;
float reference_pressure;
float filtered_altitude = 0.0f;

//drone data variables
int throttle_percent = 0;
int pitch_percent = 0;
int roll_percent = 0;
int yaw_percent = 0;

char uart_rx_buffer[50];
char uart_buffer[50];

volatile uint8_t joystick_packet_ready = 0;
uint8_t joystick_receiving = 0;
uint8_t uart_rx_index = 0;

typedef enum
{
    CMD_NONE,
    CMD_ARM,
    CMD_DISARM,
    CMD_LAND,
    CMD_ESTOP

} DroneCommand;

DroneCommand command = CMD_NONE;

void clock_init()
{
	RCC_AHB1ENR |= (1U << 0);// enable GPIOA clock
	RCC_APB1ENR |= (1U <<0);// enable TIM2 clock
	RCC_APB1ENR |= (1U << 1);//enable TIM3 clock
	RCC_APB1ENR |= (1U << 21);//e//enable I2C1 clock
	RCC_APB2ENR |= (1U <<4);//emable USART1 clcok
	RCC_APB2ENR |= (1U << 8); // enable ADC1 clock

	// enable GPIOB clock
	RCC_AHB1ENR |= (1U << 1);
	RCC_AHB1ENR |= (1U << 2); // enable GPIOC clock
}

void USART2_init(void)
{
    // Enable GPIOA clock
    RCC_AHB1ENR |= (1U << 0);

    // Enable USART2 clock
    RCC_APB1ENR |= (1U << 17);

    // PA2 = TX, PA3 = RX
    GPIOA_MODER &= ~((3U << 4) | (3U << 6));
    GPIOA_MODER |=  ((2U << 4) | (2U << 6));

    // AF7 for USART2
    GPIOA_AFRL &= ~((0xF << 8) | (0xF << 12));
    GPIOA_AFRL |=  ((7U << 8) | (7U << 12));

    // Baud rate = 115200 @ 16 MHz
    USART2_BRR = 0x008B;

    // Enable transmitter
    USART2_CR1 |= (1U << 3);

    // Enable receiver
    USART2_CR1 |= (1U << 2);

    // Enable USART
    USART2_CR1 |= (1U << 13);
}

void usart1_init()
{
	//PA9 on AF7
	GPIOA_MODER &= ~(3U << 18);
	GPIOA_MODER |= (2U << 18);
	GPIOA_AFRH &= ~(0xFU << 4);
	GPIOA_AFRH |= (7U << 4);

	//PA10 on AF7
	GPIOA_MODER &= ~(3U << 20);
	GPIOA_MODER |= (2U << 20);
	GPIOA_AFRH &= ~(0xFU << 8);
	GPIOA_AFRH |= (7U << 8);

	 // Baud rate = 115200 assuming 16 MHz clock
	 USART1_BRR = 0x008B;

	 USART1_CR1 = 0;//clearing control register

	 // Enable transmitter
	 USART1_CR1 |= (1U << 3);   // TE

	 // Enable receiver
	 USART1_CR1 |= (1U << 2);   // RE

	 // Enable USART1
	 USART1_CR1 |= (1U << 13);  // UE

	 // Enable USART1
	 USART1_CR1 |= (1U << 5);  // RXNEIE

	 NVIC_ISER1 |= (1U << 5);// To allow USARt1 to interrupt

}

void ESC_init()
{
	 //PA0 as alternate function TIM2_CH1
	 GPIOA_MODER &= ~(3U << 0);
	 GPIOA_MODER |= (2U << 0);
	 GPIOA_AFRL &= ~(0xFU << 0);
	 GPIOA_AFRL |=  (1U << 0);

	 //PA1 as alternate function TIM2_CH2
	 GPIOA_MODER &= ~(3U << 2);
	 GPIOA_MODER |= (2 << 2);
	 GPIOA_AFRL &= ~(0xFU << 4);
	 GPIOA_AFRL |=  (1U << 4);

	 //PB10 = alternate function mode TIM2_CH3
	 GPIOB_MODER &= ~(3U << 20);
	 GPIOB_MODER |=  (2U << 20);
	 GPIOB_AFRH &= ~(0xFU << 8);
	 GPIOB_AFRH |=  (1U << 8); // AF1

	 //PB2 as alternate function TIM2_CH4
	 GPIOB_MODER &= ~(3U << 4);
	 GPIOB_MODER |=  (2U << 4);
	 GPIOB_AFRL &= ~(0xFU << 8);
	 GPIOB_AFRL |=  (1U << 8);// AF1 = TIM2_CH4

}

void pwm_mode(void)
{
    TIM2_PSC = 16 - 1;
    TIM2_ARR = 19999;

    //initial throttle for all four motors
    TIM2_CCR1 = 1000;
    TIM2_CCR2 = 1000;
    TIM2_CCR3 = 1000;
    TIM2_CCR4 = 1000;

    //TIM2_CH1 PWM mode 1
    TIM2_CCMR1 &= ~(7U << 4);
    TIM2_CCMR1 |=  (6U << 4);
    TIM2_CCMR1 |=  (1U << 3);

    //TIM2_CH2 PWM mode 1
    TIM2_CCMR1 &= ~(7U << 12);
    TIM2_CCMR1 |=  (6U << 12);
    TIM2_CCMR1 |=  (1U << 11);

	//TIM2_CH3 PWM mode 1
	TIM2_CCMR2 &= ~(7U << 4);
	TIM2_CCMR2 |=  (6U << 4);
	TIM2_CCMR2 |=  (1U << 3);

	//TIM2_CH4 PWM mode 1
	TIM2_CCMR2 &= ~(7U << 12);
	TIM2_CCMR2 |=  (6U << 12);
	TIM2_CCMR2 |=  (1U << 11);

    // Enable CH1, CH2, CH3, CH4 outputs
    TIM2_CCER |= (1U << 0);
    TIM2_CCER |= (1U << 4);
   	TIM2_CCER |= (1U << 8);
   	TIM2_CCER |= (1U << 12);

    // Enable ARR preload and start timer
    TIM2_CR1 |= (1U << 7);
    TIM2_EGR |= (1U << 0);   // UG bit
    TIM2_CR1 |= (1U << 0);
}
void uart2_send_char(char c);
void USART1_IRQHandler(void)// reads data from pi
{
    char c;

    if (USART1_SR & (1U << 5))
    {
        c = (char)(USART1_DR & 0xFF);

        if (c == 'A')
        	{
        		command = CMD_ARM;
        		uart_rx_index = 0;
        		joystick_receiving = 0;
        		return;
       		}
    	else if (c == 'D')
        	{
       			command = CMD_DISARM;
       			uart_rx_index = 0;
       			joystick_receiving = 0;
       			return;
       		}
        else if (c == 'L')
        	{
        		command = CMD_LAND;
        		uart_rx_index = 0;
        		joystick_receiving = 0;
        		return;
        	}
        else if (c == 'E')
        	{
        		command = CMD_ESTOP;
        		uart_rx_index = 0;
        		joystick_receiving = 0;
        		return;
        	}

        if (joystick_packet_ready)
        {
            return;
        }

        else if(c == 'J')
        {
        	uart_rx_index = 0;
        	joystick_receiving = 1;
        	uart_rx_buffer[uart_rx_index] = c;
        	uart_rx_index++;
        	return;
        }
        if (c == '\n')
        {
            uart_rx_buffer[uart_rx_index] = '\0';

            if (uart_rx_buffer[0] == 'J')
            {
            		joystick_packet_ready = 1;
            }

            uart_rx_index = 0;
            joystick_receiving = 0;
        }

        else
        {
            if (joystick_receiving && uart_rx_index < 49)
            {
                uart_rx_buffer[uart_rx_index] = c;

                uart_rx_index++;
            }
        }
    }
}

void uart1_send_char(char c)
{
    // Wait until transmit data register is empty(SR becomes 1 when data is transmitted)
    while ((USART1_SR & (1U << 7)) == 0);

    USART1_DR = c;
}


void uart1_send_string(char *str)
{
    while (*str != '\0')
    {
        uart1_send_char(*str);
        str++;
    }
}


char uart1_read_char(void)
{
    // check if receive data register is not empty
    if ((USART1_SR & (1U << 5)))
    {
    	return (char)(USART1_DR & 0xFF);
    }
    else
    {
    	return '\0';
    }
}


void uart2_send_string(char *str);

void uart2_send_char(char c)
{
	while ((USART2_SR & (1U << 7)) == 0);

	USART2_DR = c;
}

void uart2_send_string(char *str)
{
    while (*str != '\0')
    {
        uart2_send_char(*str);
        str++;
    }
}

void i2c_init()
{
	// PB8 for SCL
	GPIOB_MODER &= ~(3U << 16);
	GPIOB_MODER |=  (2U << 16);//alternate function
	GPIOB_AFRH &= ~(0xFU << 0);
	GPIOB_AFRH |= (4U << 0);//AF4
	GPIOB_OTYPER |= (1U << 8);
	GPIOB_OSPEEDR &= ~(3U << 16);
	GPIOB_OSPEEDR |=  (2U << 16);//Fast speed mode

	// PB9 for SDA
	GPIOB_MODER &= ~(3U << 18);
	GPIOB_MODER |=  (2U << 18);// alternate function mode
	GPIOB_AFRH &= ~(0xFU << 4);// PB9 AFRL bits
	GPIOB_AFRH |=  (4U << 4);
	GPIOB_OTYPER |= (1U << 9);
	GPIOB_OSPEEDR &= ~(3U << 18);
	GPIOB_OSPEEDR |=  (2U << 18);

	I2C1_CR2 &= ~(0x3FU << 0);
	I2C1_CR2 |= (0x10U << 0);//microcontroller is cuurently running at 16MHz

	I2C1_CCR &= ~(0xFFFU << 0);
	I2C1_CCR |= (0x50U << 0);//SCl frequency is 100kHz

	I2C1_TRISE &= ~(0x3FU << 0);
	I2C1_TRISE |= (0x11U << 0);

	I2C1_CR1 |= (1U << 0);
}


void wait_us(int us)
{
    SYST_RVR = 16 - 1; // 1 microsecond at 16 MHz
    SYST_CVR = 0;
    SYST_CSR = 5;// enable SysTick, use processor clock

    for (int i = 0; i < us; i++)
    {
        while ((SYST_CSR & (1U << 16)) == 0);
    }

    SYST_CSR = 0;
}

uint8_t i2c_start()
{
    uint32_t timeout = 0;

    // Make sure previous STOP has completed
    while (I2C1_CR1 & (1U << 9))
    {
        timeout++;

        if (timeout >= 1000000)
        {
            uart2_send_string("STOP TIMEOUT\n");

            // Check I2C error flags before resetting
            uint32_t sr1 = I2C1_SR1;

            if (sr1 & (1U << 8))
            {
                uart2_send_string("BERR\n");
            }

            if (sr1 & (1U << 9))
            {
                uart2_send_string("ARLO\n");
            }

            if (sr1 & (1U << 10))
            {
                uart2_send_string("AF\n");
            }

            if (sr1 & (1U << 11))
            {
                uart2_send_string("OVR\n");
            }

            if (sr1 & (1U << 14))
            {
                uart2_send_string("TIMEOUT FLAG\n");
            }

            // Reset I2C peripheral
            I2C1_CR1 |= (1U << 15);
            I2C1_CR1 &= ~(1U << 15);

            i2c_init();

            uart2_send_string("I2C RESET\n");

            return 0;
        }
    }

    // Generate START
    I2C1_CR1 |= (1U << 8);

    timeout = 0;

    // Wait for SB
    while ((I2C1_SR1 & (1U << 0)) == 0)
    {
        timeout++;

        if (timeout >= 1000000)
        {
            uart2_send_string("START TIMEOUT\n");

            I2C1_CR1 |= (1U << 15);
            I2C1_CR1 &= ~(1U << 15);

            i2c_init();

            uart2_send_string("I2C RESET\n");

            return 0;
        }
    }

    return 1;
}
void i2c_stop(void)
{
    I2C1_CR1 |= (1U << 9);
}

void i2c_write(uint8_t device_addr, uint8_t *data, uint8_t n)
{
	uint8_t i = 0;

	(void)I2C1_SR1;
	I2C1_DR = (device_addr << 1) | 0;

	//uart2_send_string("ADDR\n");


	uint32_t timeout = 0;

	while ((I2C1_SR1 & (1U << 1)) == 0)
	{


	}
//	uart2_send_string("TXE\n");

	(void)I2C1_SR1;
	(void)I2C1_SR2;

	while (i < n)
	{
	//	uart2_send_string("WAIT TXE\n");

		while ((I2C1_SR1 & (1U << 7)) == 0)
		{
		}

	//	uart2_send_string("TXE DONE\n");
		I2C1_DR = data[i];
		i++;
	}

	//uart2_send_string("WAIT BTF\n");

	while ((I2C1_SR1 & (1U << 2)) == 0)
	{
	}

	//uart2_send_string("BTF DONE\n");
}

void i2c_read(uint8_t device_addr, uint8_t *data, uint8_t n)
{
	uint8_t i = 0;

	(void)I2C1_SR1;
	I2C1_DR = (device_addr << 1) | 1;

	while ((I2C1_SR1 & (1U << 1)) == 0)
	{
	}

	if (n == 1)
	{
		I2C1_CR1 &= ~(1U << 10);

		(void)I2C1_SR1;
		(void)I2C1_SR2;

		i2c_stop();

		while ((I2C1_SR1 & (1U << 6)) == 0)
		{
		}

		data[0] = I2C1_DR;
	}

	else if (n == 2)
	{
		I2C1_CR1 &= ~(1U << 10);
		I2C1_CR1 |= (1U << 11);

		(void)I2C1_SR1;
		(void)I2C1_SR2;

		while ((I2C1_SR1 & (1U << 2)) == 0)
		{
		}

		i2c_stop();

		data[0] = I2C1_DR;
		data[1] = I2C1_DR;

		I2C1_CR1 &= ~(1U << 11);
	}

	else
	{
		I2C1_CR1 |= (1U << 10);

		(void)I2C1_SR1;
		(void)I2C1_SR2;

		while (n > 3)
		{
			while ((I2C1_SR1 & (1U << 6)) == 0)
			{
			}

			data[i++] = I2C1_DR;
			n--;
		}

		while ((I2C1_SR1 & (1U << 2)) == 0)
		{
		}

		I2C1_CR1 &= ~(1U << 10);
		data[i++] = I2C1_DR;

		while ((I2C1_SR1 & (1U << 2)) == 0)
		{
		}

		i2c_stop();

		data[i++] = I2C1_DR;
		data[i] = I2C1_DR;

		uint32_t stop_timeout = 0;

		while (I2C1_CR1 & (1U << 9))
		{
		    stop_timeout++;

		    if (stop_timeout >= 1000000)
		    {
		        uart2_send_string("READ STOP FAILED\n");
		        break;
		    }
		}
	}

	I2C1_CR1 |= (1U << 10);
}

void wait_ms(int ms)

{
    SYST_RVR = 16000 - 1;  // 1 ms
    SYST_CVR = 0;
    SYST_CSR = 5;

    for (int i = 0; i < ms; i++) {

       while ((SYST_CSR & (1U << 16)) == 0);

    }
    SYST_CSR = 0;

}

void bmp280_init(void)
{
    uint8_t wake_data[2] = {0xF4, 0x57};
    uint8_t conf_data[2] = {0xF5, 0x10};
    i2c_start();
    i2c_write(0x76, conf_data, 2);
    i2c_stop();

    i2c_start();
    i2c_write(0x76, wake_data, 2);
    i2c_stop();
}

void bmp280_calibration(void)
{

	i2c_start();
	i2c_write(0x76, calib_reg, 1);

	i2c_start();
	i2c_read(0x76, calibration_data, 24);

	dig_T1 = (calibration_data[1] << 8) | calibration_data[0];
	dig_T2 = (calibration_data[3] << 8) | calibration_data[2];
	dig_T3 = (calibration_data[5] << 8) | calibration_data[4];
	dig_P1 = (calibration_data[7] << 8) | calibration_data[6];
	dig_P2 = (calibration_data[9] << 8) | calibration_data[8];
	dig_P3 = (calibration_data[11] << 8) | calibration_data[10];
	dig_P4 = (calibration_data[13] << 8) | calibration_data[12];
	dig_P5 = (calibration_data[15] << 8) | calibration_data[14];
	dig_P6 = (calibration_data[17] << 8) | calibration_data[16];
	dig_P7 = (calibration_data[19] << 8) | calibration_data[18];
	dig_P8 = (calibration_data[21] << 8) | calibration_data[20];
	dig_P9 = (calibration_data[23] << 8) | calibration_data[22];
}

int32_t bmp280_compensate_T_int32(int32_t adc_T)
{
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

uint32_t bmp280_compensate_P_int64(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5) << 17);
	var2 = var2 + (((int64_t)dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
	return (uint32_t)p;
}

void pressure_ref(void)
{
	uint32_t PR;
	int32_t adc_TR;
	int32_t adc_PR;
	reference_pressure = 0;

	for (int i = 0; i < 100; i++)

	{
		i2c_start();
		i2c_write(0x76, data_reg, 1);

		i2c_start();
		i2c_read(0x76, sensor_data_bmp280, 6);

		adc_TR = ((int32_t)sensor_data_bmp280[3] << 12)
			 | ((int32_t)sensor_data_bmp280[4] << 4)
			 | ((int32_t)sensor_data_bmp280[5] >> 4);

		adc_PR = ((int32_t)sensor_data_bmp280[0] << 12)
				| ((int32_t)sensor_data_bmp280[1] << 4)
				| ((int32_t)sensor_data_bmp280[2] >> 4);

		bmp280_compensate_T_int32(adc_TR);
		PR = bmp280_compensate_P_int64(adc_PR);
		reference_pressure += PR / 256.0f;
		wait_ms(10);
	}

	reference_pressure = reference_pressure/100;
}

float altitude_filter(float new_altitude)
{
    float alpha = 0.90f;

    filtered_altitude = alpha * filtered_altitude
                      + (1.0f - alpha) * new_altitude;

    return filtered_altitude;
}

void mpu6050_wake(void)
{
    uint8_t wake_data[2] = {0x6B, 0x00};

    i2c_start();
    i2c_write(0x68, wake_data, 2);
    i2c_stop();
}

void calibrate_mpu6050()
{
	int32_t ax_sum = 0;
	int32_t ay_sum = 0;
	int32_t az_sum = 0;

	int32_t gx_sum = 0;
	int32_t gy_sum = 0;
	int32_t gz_sum = 0;

	for (int i = 0; i < 100; i++)

	{
	    i2c_start();
	    i2c_write(0x68, sensor_reg, 1);
	    i2c_start();
	    i2c_read(0x68, sensor_data_mpu, 14);

	    accel_x = (sensor_data_mpu[0] << 8) | sensor_data_mpu[1];
	    accel_y = (sensor_data_mpu[2] << 8) | sensor_data_mpu[3];
	    accel_z = (sensor_data_mpu[4] << 8) | sensor_data_mpu[5];

	    gyro_x = (sensor_data_mpu[8] << 8) | sensor_data_mpu[9];
	    gyro_y = (sensor_data_mpu[10] << 8) | sensor_data_mpu[11];
	    gyro_z = (sensor_data_mpu[12] << 8) | sensor_data_mpu[13];

	    ax_sum += accel_x;
	    ay_sum += accel_y;
	    az_sum += accel_z;

	    gx_sum += gyro_x;
	    gy_sum += gyro_y;
	    gz_sum += gyro_z;

	    wait_ms(2);

	}
	accel_x_bias = ax_sum/100;
	accel_y_bias = ay_sum/100;
	accel_z_bias = (az_sum/100) - 16384;

	gyro_x_bias = gx_sum/100;
	gyro_y_bias = gy_sum/100;
	gyro_z_bias = gz_sum/100;


}

void complementary_filter(float ax_g,
						float ay_g,
						float az_g,
						float gx_dps,
						float gy_dps,
						float dt)
{
	float accel_roll = atan2(ay_g, az_g);
	float accel_pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g));

	accel_roll *= (180.0f / PI);
	accel_pitch *= (180.0f / PI);

	roll = roll + gx_dps * dt;
	pitch = pitch + gy_dps *dt;

	roll = alpha * roll + (1 - alpha) * accel_roll;
	pitch = alpha * pitch+ (1 - alpha) * accel_pitch;
}

void flight_control_update(void)
{
	int base_throttle = 1000;
	int pitch_correction = 0;
	int roll_correction = 0;
	int yaw_correction = 0;
    // calculate base throttle
	base_throttle += throttle_percent * 10;
    // calculate pitch correction
	pitch_correction += pitch_percent * 2;
    // calculate roll correction
	roll_correction +=  roll_percent * 2;
    // calculate yaw correction
	yaw_correction += yaw_percent * 2;
    // calculate each motor value

    // update CCR1
	TIM2_CCR1 = base_throttle - roll_correction - pitch_correction - yaw_correction;//motor front right
    // update CCR2
	TIM2_CCR2 = base_throttle - roll_correction + pitch_correction + yaw_correction;//motor back right
    // update CCR3
	TIM2_CCR3 = base_throttle + roll_correction - pitch_correction + yaw_correction;//motor front left
    // update CCR4
	TIM2_CCR4 = base_throttle + roll_correction + pitch_correction - yaw_correction;//motor back left
}


int main(void)
{
	SCB_CPACR |= (0xFU << 20);
	char buffer[100];
	clock_init();
	USART2_init();
	i2c_init();
	bmp280_init();
	bmp280_calibration();
	pressure_ref();
	mpu6050_wake();
	calibrate_mpu6050();

	int32_t adc_T;
	int32_t adc_P;
	uint32_t P;
	float current_pressure;
	float altitude;

	ESC_init();
	usart1_init();
	pwm_mode();

	// All motors at minimum throttle

	TIM2_CCR1 = 1000;

	TIM2_CCR2 = 1000;

	TIM2_CCR3 = 1000;

	TIM2_CCR4 = 1000;

	wait_ms(3000);

    	while (1)
    	{
    		if (joystick_packet_ready)
    		{
    		    if (sscanf(uart_rx_buffer, "J,%d,%d,%d,%d",
    		                	       &throttle_percent,
    		                	       &pitch_percent,
    		                	       &roll_percent,
    		                	       &yaw_percent) == 4)
    		    {

    		    	flight_control_update();
    		    }
    		    char debug[30];

    		        sprintf(debug, "THROTTLE:%d\n", throttle_percent);

    		       // uart2_send_string(debug);
    		    joystick_packet_ready = 0;

    		}

    		if (i2c_start() == 1)

    		{

    		    i2c_write(0x76, data_reg, 1);

    		    if (i2c_start() == 1)

    		    {

    		        i2c_read(0x76, sensor_data_bmp280, 6);

    		        uint32_t stop_test = 0;

    		        while (I2C1_CR1 & (1U << 9))

    		        {

    		            stop_test++;

    		            if (stop_test >= 1000000)

    		            {

    		                uart2_send_string("BMP READ STOP STUCK\n");

    		                break;

    		            }

    		        }

    		    }

    		}

    	//wait_ms(10);
/*
    	    adc_T = ((int32_t)sensor_data_bmp280[3] << 12)
    	          | ((int32_t)sensor_data_bmp280[4] << 4)
    	          | ((int32_t)sensor_data_bmp280[5] >> 4);

    	    adc_P = ((int32_t)sensor_data_bmp280[0] << 12)
  	        	  | ((int32_t)sensor_data_bmp280[1] << 4)
    	          | ((int32_t)sensor_data_bmp280[2] >> 4);

    	    bmp280_compensate_T_int32(adc_T);
    	    P = bmp280_compensate_P_int64(adc_P);

    	    current_pressure = P / 256.0f;
    	    altitude = 44330.0f * (1.0f - powf(current_pressure / reference_pressure, 0.1903f));
    	    filtered_altitude = altitude_filter(altitude);
*/


/*
    	    i2c_start();

            // Tell MPU6050 to start reading at register 0x3B
    	    i2c_write(0x68, sensor_reg, 1);

            // Repeated START
    	    i2c_start();

    	    // Read all 14 bytes
    	    i2c_read(0x68, sensor_data_mpu, 14);

            // Accelerometer
            accel_x = (sensor_data_mpu[0] << 8) | sensor_data_mpu[1];
            accel_y = (sensor_data_mpu[2] << 8) | sensor_data_mpu[3];
            accel_z = (sensor_data_mpu[4] << 8) | sensor_data_mpu[5];

            // Temperature
            temperature = (sensor_data_mpu[6] << 8) | sensor_data_mpu[7];

            // Gyroscope
            gyro_x = (sensor_data_mpu[8] << 8) | sensor_data_mpu[9];
            gyro_y = (sensor_data_mpu[10] << 8) | sensor_data_mpu[11];
            gyro_z = (sensor_data_mpu[12] << 8) | sensor_data_mpu[13];

            accel_x -= accel_x_bias;
            accel_y -= accel_y_bias;
    	    accel_z -= accel_z_bias;

            gyro_x -= gyro_x_bias;
    	    gyro_y -= gyro_y_bias;
    	    gyro_z -= gyro_z_bias;

    	    float ax_g = accel_x / 16384.0f;
    	    float ay_g = accel_y / 16384.0f;
    	    float az_g = accel_z / 16384.0f;

    	    float gx_dps = gyro_x / 131.0f;
    	    float gy_dps = gyro_y / 131.0f;
    	    float gz_dps = gyro_z / 131.0f;

    	    complementary_filter(ax_g, ay_g, az_g,gx_dps, gy_dps, dt);
*/
    	  //  sprintf(buffer, "ROLL:%.2f PITCH:%.2f ALT:%.2f\n",
    	   //         roll, pitch, filtered_altitude);

    	  //  uart1_send_string(buffer);

    	  //  wait_ms(500);
    	}
    }
