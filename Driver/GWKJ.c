/*
 * Copyright (c) 2023 感为智能科技(济南)
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#include "GWKJ.h"
#include "board.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#define ACK 0x0 // acknowledge (SDA LOW)
#define NACK 0x1 // not acknowledge (SDA HIGH)

#define LOW 0x0
#define HIGH 0x1

#define I2C_READ 0x1
#define I2C_WRITE 0x0

static void sw_i2c_hal_start(sw_i2c_interface_t *i2c_interface);
static void sw_i2c_hal_stop(sw_i2c_interface_t *i2c_interface);

static void sw_i2c_hal_write_bit(sw_i2c_interface_t *i2c_interface, uint8_t bit);
static uint8_t sw_i2c_hal_read_bit(sw_i2c_interface_t *i2c_interface);

static uint8_t sw_i2c_hal_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t byte);
static uint8_t sw_i2c_hal_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t ack);


int8_t sw_i2c_read(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t i;
	uint8_t ack_bit;

	/* 起始位 */
	sw_i2c_hal_start(i2c_interface);

	/* 地址+读写位 */
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_READ);
	if (ack_bit) {
		/* 从设备没有回复ACK,直接退出 */
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}

	/* 连续读取N-1个数据 给ACK */
	for (i = 0; i < data_length - 1; ++i) {
		data[i] = sw_i2c_hal_read_byte(i2c_interface, ACK);
	}

	/* 最后一个数据给 NACK */
	data[i] = sw_i2c_hal_read_byte(i2c_interface, NACK);

	/* 停止位 */
	sw_i2c_hal_stop(i2c_interface);
	return 0;
}


int8_t sw_i2c_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t *data, uint8_t data_length)
{
	uint8_t i;
	uint8_t ack_bit;

	/* 起始位 */
	sw_i2c_hal_start(i2c_interface);

	/* 地址+读写位 */
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit) {
		/* 从设备没有回复ACK,直接退出 */
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}

	/* 连续写入N个数据, 每次读取1 bit的 ACK */
	for (i = 0; i < data_length; ++i) {
		 ack_bit = sw_i2c_hal_write_byte(i2c_interface, data[i]);
	}

	/* 停止位 */
	sw_i2c_hal_stop(i2c_interface);
	return 0;
}


int8_t sw_i2c_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t *data)
{
	return sw_i2c_read(i2c_interface, dev_addr, data, 1);
}


int8_t sw_i2c_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, const uint8_t data)
{
	return sw_i2c_write(i2c_interface, dev_addr, &data, 1);
}


int8_t sw_i2c_mem_read(sw_i2c_interface_t *i2c_interface,
					   uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t ack_bit;
	sw_i2c_hal_start(i2c_interface);
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit) {
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, mem_addr);

	return sw_i2c_read(i2c_interface, dev_addr, data, data_length);
}


int8_t sw_i2c_mem_write(sw_i2c_interface_t *i2c_interface, uint8_t dev_addr, uint8_t mem_addr, const uint8_t *data,
                        uint8_t data_length)
{
	uint8_t ack_bit;
	sw_i2c_hal_start(i2c_interface);
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, dev_addr | I2C_WRITE);
	if (ack_bit) {
		sw_i2c_hal_stop(i2c_interface);
		return 1;
	}
	ack_bit = sw_i2c_hal_write_byte(i2c_interface, mem_addr);

	return sw_i2c_write(i2c_interface, dev_addr, data, data_length);
}


/***************************
 * 基礎操作抽象层
 **************************/

/**
 * @brief send start bit by driving sda and scl LOW
 * @param i2c_interface
 */
static void sw_i2c_hal_start(sw_i2c_interface_t *i2c_interface)
{
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->sda_out(LOW, i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
}

/**
 * @brief send stop bit
 * @param i2c_interface
 */
static void sw_i2c_hal_stop(sw_i2c_interface_t *i2c_interface)
{
	i2c_interface->sda_out(LOW, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
}

/**
 * @brief 输出 sda 电平,然后 scl 输出一个时钟
 * @param i2c_interface
 * @param bit bit level to send, 0:LOW, 1:HIGH
 */
static void sw_i2c_hal_write_bit(sw_i2c_interface_t *i2c_interface, uint8_t bit)
{
	i2c_interface->sda_out(bit, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
}

/**
 * @brief 读 sda 电平值,然后 scl 输出一个时钟
 * @param i2c_interface
 * @return 返回 SDA 电平值, 0:LOW, 1:HIGH
 */
static uint8_t sw_i2c_hal_read_bit(sw_i2c_interface_t *i2c_interface)
{
	uint8_t bit;
	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	i2c_interface->scl_out(HIGH, i2c_interface->user_data);
	bit = i2c_interface->sda_in(i2c_interface->user_data);
	i2c_interface->scl_out(LOW, i2c_interface->user_data);
	return bit;
}

/**
 * @brief 向IIC输出一个字节
 * @param i2c_interface
 * @param byte
 * @return 从设备反馈的 ACK 电平值
 */
static uint8_t sw_i2c_hal_write_byte(sw_i2c_interface_t *i2c_interface, uint8_t byte)
{
	uint8_t i;
	uint8_t ack;

	for (i = 0; i < 8; ++i) {
		sw_i2c_hal_write_bit(i2c_interface, byte & (0x80 >> i));
	}

	ack = sw_i2c_hal_read_bit(i2c_interface);
	return ack;
}

/**
 * @brief 从IIC总线上读取一个字节
 * @param i2c_interface
 * @param ack 向从设备反馈 ACK 或者 NACK
 * @return 读取到的字节
 */
static uint8_t sw_i2c_hal_read_byte(sw_i2c_interface_t *i2c_interface, uint8_t ack)
{
	uint8_t byte = 0;
	uint8_t i;

	i2c_interface->sda_out(HIGH, i2c_interface->user_data);
	for (i = 0; i < 8; ++i) {
		i2c_interface->scl_out(HIGH, i2c_interface->user_data);
		byte <<= 1;
		byte |= i2c_interface->sda_in(i2c_interface->user_data);
		i2c_interface->scl_out(LOW, i2c_interface->user_data);
	}

	sw_i2c_hal_write_bit(i2c_interface, ack);
	return byte;
}

/**
 * 初始化i2c
 */
void sw_i2c_init(void)
{
    
}

/* 定义sda输出函数 bit=0为低电平 bit=1为高电平 */
void sda_out(uint8_t bit, void *user_data)
{
	DL_GPIO_writePinsVal(GPIO_GW_PORT,GPIO_GW_GPIO_SDA_PIN,bit);
	/* IIC软件延迟 */
	delay_us(10);
}

/* 定义sda读取函数 bit 为返回的电平值 */
uint8_t sda_in(void *user_data)
{
	uint8_t bit;
	bit = DL_GPIO_readPins(GPIO_GW_PORT,GPIO_GW_GPIO_SDA_PIN);
	/* IIC软件延迟 */
	delay_us(10);
	return bit;
}

/* 定义scl时钟输出函数 bit=0为低电平 bit=1为高电平 */
void scl_out(uint8_t bit, void *user_data)
{
	DL_GPIO_writePinsVal(GPIO_GW_PORT,GPIO_GW_GPIO_SCL_PIN,bit);
	/* IIC软件延迟 */
	delay_us(10);
}

/**
 * i2c地址扫描
 * @param scan_addr 扫描出来的地址存放,数值不为0的为扫描到的地址，扫到的地址会挨个放在数组的最前面
 * @return 返回扫描到的设备数量, 0为无设备发现
 */
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr)
{
	int i;
	uint8_t count = 0;
	uint8_t data;
	int8_t ret;
	
	for (i = 1; i < 127; ++i) {
		ret = sw_i2c_read(i2c_interface, i << 1, &data, 1);
		if (ret == 0) {
			scan_addr[count] = i;
			++count;
		}
	}
	
	return count;
}

/* 设置软件IIC驱动 */
sw_i2c_interface_t i2c_interface = 
{
	.sda_in = sda_in,
	.scl_out = scl_out,
	.sda_out = sda_out,
	.user_data = 0, //用户数据，可在输入输出函数里得到
};

void GW_digital_Init(void)
{
	/* 存放扫描到的地址 */
	uint8_t ping_response;
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	/* 初始化IIC */
	sw_i2c_init();
	/* ps: 软件IIC初始化的时候会出发一次IIC start，会导致第一次IIC通讯会失败 */
	
	/* 第一次IIC通讯会失败（因为软件IIC触发了start），手动发个stop也能消除 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	/* 后面IIC通讯是正常的 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	
	/* 扫描开始 */
	count = i2c_scan(&i2c_interface, scan_addr);
	
	/* 打开开关量数据模式 */
	sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
}

void GW_digital_Read(uint8_t *digital_data)
{
	/* 读取开关量数据 */
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, digital_data); // digital_data 有1~8号探头开关数据
}

uint8_t GW_digital_Read_Bit(uint8_t Num)
{
	uint8_t digital_data;
	GW_digital_Read(&digital_data);
	// 从字节中提取第二个 比特
	return GET_NTH_BIT(digital_data, Num);
}

void GW_digital_dis_to_eight(uint8_t digital_data, uint8_t *gray_sensor)
{
	/* 把字节里的8个开关量存到八个变量里，这里为gray_sensor[0] ~ gray_sensor[7], 
		 * 也可以是变量val1 ~ val8, 因为是宏定义 */
		SEP_ALL_BIT8(digital_data, 
			gray_sensor[0], //探头1
			gray_sensor[1], //探头2
			gray_sensor[2], //探头3
			gray_sensor[3], //探头4
			gray_sensor[4], //探头5
			gray_sensor[5], //探头6
			gray_sensor[6], //探头7
			gray_sensor[7]  //探头8
		);
}

void GW_analog_Init(void)
{
	/* 存放扫描到的地址 */
	uint8_t ping_response;
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	/* 初始化IIC */
	sw_i2c_init();
	/* ps: 软件IIC初始化的时候会出发一次IIC start，会导致第一次IIC通讯会失败 */
	
	/* 第一次IIC通讯会失败（因为软件IIC触发了start），手动发个stop也能消除 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	/* 后面IIC通讯是正常的 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	
	/* 扫描开始 */
	count = i2c_scan(&i2c_interface, scan_addr);
	
	/* 读取模拟量数据 */
	uint8_t analog_data[8]; // analog_data[0:7] 有1~8号探头模拟数据
	
	/* 打开模拟值模式, 并且读取模拟数值, 后面可以直接读取 */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_ANALOG_MODE, analog_data, 8);
}

void GW_analog_Read(uint8_t *analog_data)
{
	/* 直接读取 */
	sw_i2c_read(&i2c_interface, 0x4C << 1, analog_data, 8);
}
