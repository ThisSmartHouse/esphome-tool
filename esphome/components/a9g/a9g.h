#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace a9g {

typedef struct A9GCoordinate {
    float latitude;
    float longitude;
} A9GCoordinate;

class A9G : public PollingComponent, public uart::UARTDevice {
  public:
    void set_latitude_sensor(sensor::Sensor *latitude_sensor) { latitude_sensor_ = latitude_sensor; }
    void set_longitude_sensor(sensor::Sensor *longitude_sensor) { longitude_sensor_ = longitude_sensor; }

    float get_setup_priority() const override { return setup_priority::HARDWARE; }

    void set_power_on_pin(GPIOPin *power_on_pin) {
        a9g_power_on_pin_ = power_on_pin;
        a9g_power_on_pin_->pin_mode(gpio::FLAG_OUTPUT);
    }

    void set_power_off_pin(GPIOPin *power_off_pin) {
      a9g_power_off_pin_ = power_off_pin;
      a9g_power_off_pin_->pin_mode(gpio::FLAG_OUTPUT);
    }

    void set_wake_pin(GPIOPin *wake_pin) {
      a9g_wake_pin_ = wake_pin;
      a9g_wake_pin_->pin_mode(gpio::FLAG_INPUT);
    }

    void set_low_power_pin(GPIOPin *low_power_pin) {
      a9g_low_power_pin_ = low_power_pin;
      a9g_low_power_pin_->pin_mode(gpio::FLAG_OUTPUT);
    }

    bool send_command(const char *command, uint8_t *response, size_t length);
    bool send_command(const char *command);
    A9GCoordinate* parse_location_response(uint8_t *response);

    void loop() override;
    void update() override;
    void setup() override;

  protected:
    float latitude_ = -1;
    float longitude_ = -1;

    GPIOPin *a9g_power_on_pin_;
    GPIOPin *a9g_power_off_pin_;
    GPIOPin *a9g_wake_pin_;
    GPIOPin *a9g_low_power_pin_;

    bool a9g_initialized_ = false;

    sensor::Sensor *latitude_sensor_{nullptr};
    sensor::Sensor *longitude_sensor_{nullptr};
};

} // end of namespace a9g
} // end of namespace esphome
