#include "lib_servo.h"
#include <avr/io.h>
#include <util/delay.h>

void servo_init(int pin)
{
  
    if (pin == PB2) {
        DDRB |= (1 << PB2); 
        TCCR1A |= (1 << COM1B1) | (1 << WGM11);  
        TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); 
        ICR1 = 39999;  
    }
    if (pin == PB1) {
        DDRB |= (1 << PB1);
        TCCR1A |= (1 << COM1A1) | (1 << WGM11);  
        TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); 
        ICR1 = 39999;  
    }
}




void set_angle(int angle)
{
     int pulse_width = map(angle, 0, 180, 700, 4800);
     OCR1A = pulse_width;  
     OCR1B = pulse_width; 
}


int map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    float c;
    c = (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    return (int)c;  
}