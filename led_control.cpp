#include "led_control.h"
#include <avr/interrupt.h>



LedControl::LedControl()
{
    pinMode(RING_PIN, OUTPUT);
    pinMode(SEGMENT_PIN, OUTPUT);

    ring_leds.begin();
    ring_leds.setBrightness(40);

    segment_leds.begin();
    segment_leds.setBrightness(20);
}



// Draws segment number.
void LedControl::set_segment_number(uint16_t number)
{
    uint16_t digit;
    Side side;


    // Reset segments before drawing number.
    segment_leds.clear();


    for(uint16_t i = 0; i < 2; i++)
    {
        side = i == 0 ? LEFT : RIGHT;
        digit = i == 0 ? number / 10 : number % 10;

        switch(digit)
        {
            case 0:
                set_digit(6, zero, side);
                break;
            case 1:
                set_digit(2, one, side);
                break;
            case 2:
                set_digit(5, two, side);
                break;
            case 3:
                set_digit(5, three, side);
                break;
            case 4:
                set_digit(4, four, side);
                break;
            case 5:
                set_digit(5, five, side);
                break;
            case 6:
                set_digit(6, six, side);
                break;
            case 7:
                set_digit(3, seven, side);
                break;
            case 8:
                set_digit(7, eight, side);
                break;
            case 9:
                set_digit(6, nine, side);
                break;
        }
    }

    segment_leds.show();
}



// Draws one digit number.
void LedControl::set_digit(uint16_t segment_number, Segment *segment, Side side)
{
    uint16_t red = 250;
    uint16_t blue = 0;
    uint16_t green = 0;

    for(uint16_t i = 0; i < segment_number; i++)
    {
        for(uint16_t j = 0; j < SEGMENT_LENGTH; j++)
        {
            if(side == LEFT)
            {
                segment_leds.setPixelColor(segment[i].left[j], segment_leds.Color(red, blue, green));
            }
            else if(side == RIGHT)
            {
                segment_leds.setPixelColor(segment[i].right[j], segment_leds.Color(red, blue, green));
            }
        }
    }
}





// Updates the ring LEDs.
void LedControl::update_ring(uint16_t minute, uint16_t second)
{
    uint16_t minute_index = uint16_t(((float)RING_LED_NUMBER / 60.0) * (float)minute);

    if(minute_index < 12)
    {
        minute_index = 11 - minute_index;
    }
    else
    {
        minute_index = RING_LED_NUMBER - (minute_index - 11);
    }

    uint16_t second_index = uint16_t(((float)RING_LED_NUMBER / 60.0) * (float)second);

    if(second_index < 12)
    {
        second_index = 11 - second_index;
    }
    else
    {
        second_index = RING_LED_NUMBER - (second_index - 11);
    }


    ring_leds.clear();

    // Color all LEDs with standard color.
    for(uint16_t i = 0; i < RING_LED_NUMBER; i++)
    {
        ring_leds.setPixelColor(i, ring_leds.Color(0, 45, 45));
    }

    // Color second.
    ring_leds.setPixelColor(second_index, ring_leds.Color(250, 250, 0));

    // Color minute.
    ring_leds.setPixelColor(minute_index, ring_leds.Color(0, 250, 0));



    ring_leds.show();
}
