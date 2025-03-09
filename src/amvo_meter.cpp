#include <Arduino.h>
#include "Wire.h"
#include "INA226.h"
#include "avarager.h"


INA226 INA(0x40);

void setup() {

	asm(".global _printf_float"); // enable floating numbers handling

    Serial.begin(SERIAL_BAUDS);

    Wire.begin();
    if (!INA.begin() )
    {
        Serial.println("could not connect. Fix and Reboot");
    }

    float ina_shunt = 0.1;                      /* shunt (Shunt Resistance in Ohms). Lower shunt gives higher accuracy but lower current measurement range. Recommended value 0.020 Ohm. Min 0.001 Ohm */
    float ina_current_LSB_mA = 0.025;              /* current_LSB_mA (Current Least Significant Bit in milli Amperes). Recommended values: 0.050, 0.100, 0.250, 0.500, 1, 2, 2.5 (in milli Ampere units) */
    float ina_current_zero_offset_mA = -0.025 ;         /* current_zero_offset_mA (Current Zero Offset in milli Amperes, default = 0) */
    uint16_t ina_bus_V_scaling_e4 = 10004;        /* bus_V_scaling_e4 (Bus Voltage Scaling Factor, default = 10000) */
  
    INA.configure(ina_shunt, ina_current_LSB_mA, ina_current_zero_offset_mA, ina_bus_V_scaling_e4);
    INA.setMaxCurrentShunt(1, ina_shunt);
    INA.setAverage(INA226_4_SAMPLES);
    INA.setBusVoltageConversionTime(INA226_1100_us);
    INA.setShuntVoltageConversionTime(INA226_1100_us);

    Serial.print("\nMax Measurable Current:\t");
    Serial.print(INA.getMaxCurrent(), 3);
    Serial.println(" A");

    Serial.println("BUS\tSHUNT\tCURRENT\tPOWER\tPOWER2\tDELTA\tMAX_BUS_V\tMAX_SHT_V\tMAX_CURRENT\tMAX_POWER");

}

void set_max_value(float reference, float *current_max) {
    if ( *current_max < reference ) {
        *current_max = reference;
    };
    return;
}

float max_voltage_bus = 0; // in V
float max_voltage_shunt = 0; // in mV
float max_current = 0; // in mA
float max_power = 0; // in mW


void loop() {
    
    char line[300];
    float voltage_bus = INA.getBusVoltage();
    float voltage_shunt = INA.getShuntVoltage_mV();
    float current_ma = INA.getCurrent_mA();
    float power_mw = INA.getPower_mW();
  
    set_max_value(voltage_shunt,&max_voltage_shunt);
    set_max_value(current_ma,&max_current);
    set_max_value(power_mw,&max_power);
    set_max_value(voltage_bus,&max_voltage_bus);

    snprintf(line,299,"Bus: %2.3f V | Shunt: % 2.3f V  | Current: % 3.3f mA | Power: %03.2f mW | M_Bus: %2.3f V | M_Shunt: %2.3fV | M_Curernt: %3.2f mA",voltage_bus,voltage_shunt,current_ma,power_mw,max_voltage_bus,max_voltage_shunt,max_current);
    Serial.println(line);

    // Serial.print(voltage_bus, 3);
    // Serial.print("\t");
    // Serial.print(voltage_shunt, 3);
    // Serial.print("\t");
    // Serial.print(current_ma, 3);
    // Serial.print("\t");
    // Serial.print(power_mw, 3);
    // Serial.print("\t");
    // Serial.print(voltage_bus * current_ma, 3);
    // Serial.print("\t");
    // Serial.print((power_mw - voltage_bus * current_ma), 3);
    // Serial.print("\t");
    // Serial.print(max_voltage_bus);
    // Serial.print("\t");
    // Serial.print(max_voltage_shunt);
    // Serial.print("\t");
    // Serial.print(max_current);
    // Serial.print("\t");
    // Serial.print(max_power);
    // Serial.println();
    delay(200);
}

