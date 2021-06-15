#include "ddsu6606.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ddsu6606 {

static const char *TAG = "ddsu6606";

static const uint8_t DDSU_CMD_READ_REGISTERS = 0x03;


static const uint16_t DDSU_ENERGY_REGISTER_COUNT = 2;     // 2x 16-bit registers
static const uint16_t DDSU_ELEC_PARA_REGISTER_COUNT = 5; // 5x 16-bit registers


static const uint16_t DDSU_CMD_EMERGY_ADDR = 0x00;
static const uint16_t DDSU_CMD_ELEC_PARA_ADDR = 0x0300;

static const uint16_t InfoAddr[]={0x0,0x0,0x0300,0x0303,0x0306,0x0312,0x0316};




 #define Info_IDLE    0
 #define Info_Energy  1
 #define Info_Voltage 2
 #define Info_Current 3
 #define Info_Power   4
 #define Info_PF      5
 #define Info_Freq    6
 #define Info_MAX     7

static int step =Info_IDLE;

void DDSU6606::on_modbus_data(const std::vector<uint8_t> &data) {

  uint32_t buffer32=0;
  float bufferf=0.0f;

  auto ddsu_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };
  auto ddsu_get_32bit = [&](size_t i) -> uint32_t {
    return (uint32_t(ddsu_get_16bit(i + 2)) << 16) | (uint32_t(ddsu_get_16bit(i + 0)) << 0);
  };

  if ((data.size() < 2 && step > Info_Energy) || (data.size() < 4 && step == Info_Energy)) {
      ESP_LOGW(TAG, "Invalid reply size for DDSU6606,len %d @ step %d",data.size(),step);
      //return;
  }
  if(step == Info_Energy) {
    buffer32 = ddsu_get_16bit(0)<<16;
    buffer32 |= ddsu_get_16bit(2);
  }
  else {
    buffer32 = ddsu_get_16bit(0);
  }
  //ESP_LOGI(TAG, "ON_MODBUS_RECV@step%d",step);
  switch(step) { 
    case Info_Energy: 
      bufferf = buffer32 / 100.0f;  // 0.01kWh
      if (this->energy_sensor_ != nullptr)
        this->energy_sensor_->publish_state(bufferf);
      step++;
      this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    case Info_Voltage:
      bufferf = buffer32 / 10.0f;  // max 6553.5 V
      if (this->voltage_sensor_ != nullptr)
       this->voltage_sensor_->publish_state(bufferf);
      step++;
      this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    case Info_Current:
      bufferf = buffer32 / 100.0f;  // max 655.35 A
      if (this->current_sensor_ != nullptr)
        this->current_sensor_->publish_state(bufferf);
      step++;
      this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    case Info_Power:
      bufferf = buffer32 /100.0f;  // max 655.360kW
      if (this->power_sensor_ != nullptr)
        this->power_sensor_->publish_state(bufferf);
      step++;
      this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    case Info_PF:
      bufferf = buffer32 / 10.0f; //0.001 -> 0.1%
      if (this->power_factor_sensor_ != nullptr)
        this->power_factor_sensor_->publish_state(bufferf);
      step++;
      this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    case Info_Freq:
      bufferf = buffer32 / 100.0f;        //0.01Hz
      if (this->frequency_sensor_ != nullptr)
        this->frequency_sensor_->publish_state(bufferf);
      step = Info_IDLE;
      //this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 1); 
      break;
    default:
      step = Info_IDLE;
    }
}

void DDSU6606::update() { step = Info_Energy; this->send(DDSU_CMD_READ_REGISTERS, InfoAddr[step], 2); }
void DDSU6606::dump_config() {
  ESP_LOGCONFIG(TAG, "DDSU6606:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  LOG_SENSOR("", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Energy", this->energy_sensor_);
  LOG_SENSOR("", "Frequency", this->frequency_sensor_);
  LOG_SENSOR("", "Power Factor", this->power_factor_sensor_);
}

}  // namespace ddsu6606
}  // namespace esphome
