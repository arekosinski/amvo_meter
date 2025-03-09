#include <Arduino.h>
#include "avarager.h"


Avareager t1(5);


void print_values() {
    Serial.print("T1 max size: ");
    Serial.println(t1.getMaxSize());

    Serial.print("T1 current size: ");
    Serial.println(t1.getCurrentSize());

    Serial.print("T1 avg: ");
    Serial.println(t1.getAvarage());

    Serial.print("T1 Sum: ");
    Serial.println(t1.getSum());

    Serial.print("T1 Max: ");
    Serial.println(t1.getMaxValue());

    Serial.print("T1 Min: ");
    Serial.println(t1.getMinValue());

}

void setup() {

	asm(".global _printf_float"); // enable floating numbers handling

    Serial.begin(SERIAL_BAUDS);

    Serial.println("hello");

    t1.push(1);
    Serial.println("After adding 1 value");
    print_values();


    t1.push(1);
    t1.push(1);

    Serial.println("After adding 3 values");
    print_values();

    t1.push(1);
    t1.push(1);
    Serial.println("After adding 5 values");
    print_values();

    t1.push(1);
    Serial.println("After adding 6 values");
    print_values();
}



void loop() {
    
}

