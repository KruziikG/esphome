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
