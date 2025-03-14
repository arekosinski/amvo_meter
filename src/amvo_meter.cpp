#include <Arduino.h>
#include "Wire.h"
#include "INA226.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dataseries.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

GFXcanvas1 canvas_text(64,32); // 16-bit, 120x30 pixels
GFXcanvas1 canvas_graph(64,32); // 16-bit, 120x30 pixels

INA226 INA(0x40);

DataSeries data_series_current(60);
DataSeries data_series_voltage_bus(60);


void setup() {

	asm(".global _printf_float"); // enable floating numbers handling

    Serial.begin(SERIAL_BAUDS);

    Wire.begin();
    if (!INA.begin() )
    {
        Serial.println("could not connect. Fix and Reboot");
    };

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

    delay(1000); // this time is needed to establish startup time after power up - escpecially if initialization would be in first lines of setup procedure 
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    } else {
      Serial.println("Screen found");
    };
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.display();

}

void set_max_value(float reference, float *current_max) {
    if ( *current_max < reference ) {
        *current_max = reference;
    };
    return;
}

int get_autoscale_max_value_graph(int current) {

    if ( current < 1 ) {
        return 1;
    } else if ( current < 10 ) {
        return 10; 
    } else if (current < 30 ) {
        return 30;
    } else if ( current < 50 ) {
        return 50;
    } else if ( current < 100 ) {
        return 100;
    };

    return 300;
}

int graph_max_miliamps_scale = 30;

void loop() {
    
    char line[300] = {0};
    float voltage_bus = INA.getBusVoltage();
    float voltage_shunt = INA.getShuntVoltage_mV();
    float current_ma = INA.getCurrent_mA();
    float current_ua = INA.getCurrent_uA();
    float power_mw = INA.getPower_mW();

    int current_index_in_graph = data_series_current.push(current_ma);

    snprintf(line,299,"Bus: %2.3f V | Shunt: % 2.3f V  | Current: % 3.3f mA | Power: %03.2f mW | M_Bus: %2.3f V | M_Curernt: %3.2f mA",voltage_bus,voltage_shunt,current_ma,power_mw,data_series_voltage_bus.getMax(),data_series_current.getMax());
    Serial.println(line);

  
    // prepare text to show in display
    canvas_text.fillScreen(SSD1306_BLACK);
    canvas_text.setTextSize(1);
    canvas_text.setTextColor(SSD1306_WHITE);

    // prepare line to print about current
    line[0] = '\0';
    if ( current_ma < 0 ) {
        snprintf(line, 9, "A: -0 m");
    } else if ( current_ma > 1 ) {
        snprintf(line, 12, "A:%03.3fm", current_ma);
    } else {
        snprintf(line, 12, "A:%03.3fu", current_ua);
    };
    canvas_text.setCursor(0,0);
    canvas_text.print(line);
    
    // prepare line to print about max current
    line[0] = '\0';
    snprintf(line, 12, "MA:%03.1fm", data_series_current.getMax());
    canvas_text.setCursor(0,8);
    canvas_text.print(line);


    // prepare line to print about avg current
    line[0] = '\0';
    snprintf(line, 12, "AA:%03.1fm", data_series_current.getAvarage());
    canvas_text.setCursor(0,16);
    canvas_text.print(line);

    // prepare line to print about voltage bus
    line[0] = '\0';
    snprintf(line, 12, "Vb:%-2.2fV", voltage_bus);
    canvas_text.setCursor(0,24);
    canvas_text.print(line);

    // prepare graph data to show in display
    // simplified autoscale of the graph
    graph_max_miliamps_scale = get_autoscale_max_value_graph(data_series_current.getMax());

    canvas_graph.drawFastVLine(current_index_in_graph,0,SCREEN_HEIGHT,SSD1306_BLACK);
    int scale_ma_into_pixel_position_y = SCREEN_HEIGHT - ceil(current_ma/graph_max_miliamps_scale*SCREEN_HEIGHT);
    if (scale_ma_into_pixel_position_y < 0 ) { // why? because we assume that we can measure more than we're scaling the graph
        scale_ma_into_pixel_position_y = 0; 
    };
    canvas_graph.drawPixel(current_index_in_graph, scale_ma_into_pixel_position_y, SSD1306_WHITE);

    // rewrite buffers to screen
    display.drawBitmap(64, 0, canvas_text.getBuffer(), canvas_text.width(),canvas_text.height(), SSD1306_WHITE, SSD1306_BLACK);
    display.drawBitmap(0, 0, canvas_graph.getBuffer(), canvas_graph.width(),canvas_graph.height(), SSD1306_WHITE, SSD1306_BLACK);
    display.display();
    
    delay(200);
}

