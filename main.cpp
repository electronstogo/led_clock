#include <Arduino.h>
#include <avr/sleep.h>
#include "led_control.h"
#include "rtc.h"



#define BUTTON_LEFT     7
#define BUTTON_RIGHT    8



// External interrupt from RTC 1Hz signal.
ISR(INT1_vect)
{
    // Just wake up controller.
}



void setup()
{
    cli();

    // Enable external interrupt rising edge at INT1 pin
    EICRA |= 0x0C;
    // Activate extern interrupt INT1
    EIMSK = 0x02;
    // Reset both extern interrupt flags.
    EIFR  = 0x03;

    sei();


    // Activate button pull ups.
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);

}





void loop()
{
    RTC rtc;
    LedControl led_control;

    // Timestamp for fast scroll time setting.
    uint32_t timestamp = 0;

    // To remember last hours, and refresh only for new hours value.
    uint16_t last_hours = 99;


    while(true)
    {

        // Time configuration if any button is active.

        if(!digitalRead(BUTTON_LEFT))
        {
            timestamp = timestamp ? timestamp : millis();

            if(millis() - timestamp > 3000)
            {
                rtc.hours = rtc.minutes >= 5 ? rtc.hours : rtc.hours ? rtc.hours - 1 : 23;
                rtc.minutes = rtc.minutes >= 5 ? rtc.minutes - 5 : 59;
            }
            else
            {
                rtc.hours = rtc.minutes ? rtc.hours : rtc.hours ? rtc.hours - 1 : 23;
                rtc.minutes = rtc.minutes ? rtc.minutes - 1 : 59;
            }

            rtc.set_data();
        }
        else if(!digitalRead(BUTTON_RIGHT))
        {
            timestamp = timestamp ? timestamp : millis();

            if(millis() - timestamp > 3000)
            {
                rtc.hours = rtc.minutes < 55 ? rtc.hours : rtc.hours < 23 ? rtc.hours + 1 : 0;
                rtc.minutes = rtc.minutes < 55 ? rtc.minutes + 5 : 0;
            }
            else
            {
                rtc.hours = rtc.minutes < 59 ? rtc.hours : rtc.hours < 23 ? rtc.hours + 1 : 0;
                rtc.minutes = rtc.minutes < 59 ? rtc.minutes + 1 : 0;
            }

            rtc.set_data();
        }
        else
        {
            timestamp = 0;
        }


        // Update RTC time.
        rtc.update();

        // LED time update.

        if(last_hours != rtc.hours)
        {
            led_control.set_segment_number(rtc.hours);
            last_hours = rtc.hours;
        }
        led_control.update_ring(rtc.minutes, rtc.seconds);


        // Time configuration is active.
        if(timestamp)
        {
            delay(100);
            continue;
        }

        // Enter sleep mode, RTC 1Hz trigger wakes up controller.
        SMCR |= (1 << 0);
        SMCR |= (1 << 2);
        sleep_cpu();
    }
}
