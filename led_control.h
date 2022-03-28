#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#define RING_LED_NUMBER         31
#define SEGMENT_LED_NUMBER      30

#define RING_PIN                4
#define SEGMENT_PIN             5

// LED number of one segment.
#define SEGMENT_LENGTH          3




typedef enum Side
{
    RIGHT = 1,
    LEFT  = 2

} Side;


// Datatype keeps LED indices of Segment.
typedef struct Segment
{
    const uint16_t left[3];
    const uint16_t right[3];

} Segment;



class LedControl
{
    public:
        LedControl();
        void set_segment_number(uint16_t number);
        void update_ring(uint16_t minute, uint16_t second);


    private:
        Segment a = {{4, 5, 14}, {19, 20, 29}};
        Segment b = {{14, 13, 12}, {29, 28, 27}};
        Segment c = {{10, 11, 12}, {27, 26, 25}};
        Segment d = {{0, 9, 10}, {15, 24, 25}};
        Segment e = {{0, 1, 2}, {15, 16, 17}};
        Segment f = {{2, 3, 4}, {17, 18, 19}};
        Segment g = {{2, 7, 12}, {17, 22, 27}};

        Segment zero[6]  = {a, b, c, d, e, f};
        Segment one[2]   = {b, c};
        Segment two[5]   = {b, a, g, e, d};
        Segment three[5] = {b, a, g, c, d};
        Segment four[4]  = {b, f, g, c};
        Segment five[5]  = {f, a, g, c, d};
        Segment six[6]   = {a, f, g, c, e, d};
        Segment seven[3] = {a, b, c};
        Segment eight[7] = {a, b, c, d, e, f, g};
        Segment nine[6]  = {a, b, c, d, f, g};



        Adafruit_NeoPixel ring_leds = Adafruit_NeoPixel(RING_LED_NUMBER, RING_PIN, NEO_GRB + NEO_KHZ800);
        Adafruit_NeoPixel segment_leds = Adafruit_NeoPixel(SEGMENT_LED_NUMBER, SEGMENT_PIN, NEO_GRB + NEO_KHZ800);

        void set_digit(uint16_t segment_number, Segment *segment, Side side);
};


#endif
