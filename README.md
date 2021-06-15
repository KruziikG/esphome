# Forked from [ESPHome](https://github.com/esphome) 
# DDSU6606 for ESPHOME

此项目增加了对DDSU6606电能表的支持。按照正常操作，DDSU6606的支持应该作为External Components来弄的，但是ESPHOME说明文档对于这个功能没有详细说明，碰到了bug，就没有继续弄，简单起见，直接把对DDSU6606的支持增加到了ESPHOME内置的传感器中了。


## 1. Installation

Clone this repository,build and install esphome.

```bash
git clone https://github.com/KruziikG/esphome.git
cd esphome
./setup.py build && sudo ./setup.py install
```

## 2. build DDSU6606 Components

### 2.1 YAML Example

```yaml
wifi:
  ...

uart:
  rx_pin: D7
  tx_pin: D8
  parity: EVEN
  baud_rate: 2400

sensor:
  - platform: ddsu6606
    current:
      name: "DDSU6606 Current"
    voltage:
      name: "DDSU6606 Voltage"
    energy:
      name: "DDSU6606 Energy"
    power:
      name: "DDSU6606 Power"
    frequency:
      name: "DDSU6606 Frequency"
    power_factor:
      name: "DDSU6606 Power Factor"
    update_interval: 10s
```

### 2.2 Build Firmware
   
```bash
esphome xxx.yaml run
```

## 3. DDSU6606 信息
### 3.1 RS485 接口
DDSU6606 具有一个RS485接口以及一个脉冲输出口。这里我们使用RS485接口。RS485接口默认参数为：2400波特率，偶校验，8数据位，1停止位，Modbus-RTU协议，地址码默认为1。采用03号命令读出数据。
### 3.2 Modbus寄存器

|  地址  | 数据格式  |单位|读/写|数据长度|说明|
|  ----  | ----  | ----  | ----  | ----  | ----  |
| 0000H  | long | 0.01kWh | R   |2  | 当前组合总有功电能 |
| 000AH  | long | 0.01kWh | R   |2  | 当前组合正向有功电能 |
| 0014H  | long | 0.01kWh | R   |2  | 当前组合反向有功电能 |
| 0300H  | int  | 0.1V    | R   |1  | A相电压            |
| 0303H  | int  | 0.01A   | R   |1  | A相电流            |
| 0306H  | int  | 0.01kW  | R   |1  | 瞬时A相有功功率      |
| 0312H  | int  | 0.001   | R   |1  | A相功率因素         |
| 0316H  | int  | 0.01Hz  | R   |1  | 频率               |
| 0404H  | int  |         | R/W |1  | 波特率<br>0-1200<br>1-2400<br>2-4800<br>3-9600|
| 0406H  | int  |         | R/W |1  | 通讯地址<br>1-247可设 |
| 0407H  | int  |         | R   |1  | 电表常数           |
| FE00H  | int  |         | R   |1  | 数据等于5555H时，进入编程状态<br>数据等于0000H时，退出编程状态 |
