#include <Arduino.h>
#include <FastLED.h>
#include <map>

#define BIG_ONE

#define LED_PIN     26
#define CHIPSET     NEOPIXEL

#define BRIGHTNESS  64
#define FRAMES_PER_SECOND 30
const float DimRate = 0.9;


enum Orientation
{
  ORIENT_HORIZ = 0,
  ORIENT_UP,
  ORIENT_DOWN 
};

struct Segment
{
  CRGB* m_LEDs;
  CRGB* m_Trails;
  signed char m_Neighbors[4]; //left up, left down, right up, right down
  signed char m_Direction;
  Orientation m_Orientation;
};

#ifdef LITTLE_ONE
#define NUM_TRAILS 3
#define NUM_SEGMENTS 15
#define LEDS_PER_SEGMENT 3

#define NUM_LEDS    (NUM_SEGMENTS * LEDS_PER_SEGMENT)
CRGB leds[NUM_LEDS];

//     +------+
//    /   1    \
//   /2        0\
//  +            +------+
//   \          /   11   \
//    \3     10/        12\
//     +------+            +
//    /   9    \          /
//   /4        8\      13/
//  +            +------+
//   \          /   14
//    \5      7/
//     +------+
//        6


Segment Segments[NUM_SEGMENTS] =
{
  { leds +  0, {-1,  1, 11, 10}, -1, ORIENT_DOWN},
  { leds +  3, {-1,  2, -1,  0}, -1, ORIENT_HORIZ},
  { leds +  6, {-1,  3, -1,  1}, -1, ORIENT_UP},
  { leds +  9, { 2, -1,  9,  4},  1, ORIENT_DOWN},

  { leds + 12, {-1,  5,  3,  9}, -1, ORIENT_UP},
  { leds + 15, { 4, -1,  6, -1},  1, ORIENT_DOWN},
  { leds + 18, { 5, -1,  7, -1},  1, ORIENT_HORIZ},
  { leds + 21, { 6, -1,  8, 14},  1, ORIENT_UP},

  { leds + 24, {10,  9, 14,  7}, -1, ORIENT_DOWN},
  { leds + 27, { 3,  4, 10,  8}, -1, ORIENT_HORIZ},
  { leds + 30, { 9,  8,  0, 11},  1, ORIENT_UP},
  { leds + 33, { 0, 10, -1, 12},  1, ORIENT_HORIZ},

  { leds + 36, {-1, 11, -1, 13},  1, ORIENT_DOWN},
  { leds + 39, {14, -1, 12, -1}, -1, ORIENT_UP},
  { leds + 42, { 8,  7, 13, -1}, -1, ORIENT_HORIZ}
};
#endif

#ifdef BIG_ONE
#define NUM_TRAILS 20
#define NUM_SEGMENTS 54
#define LEDS_PER_SEGMENT 8

#define NUM_LEDS    (NUM_SEGMENTS * LEDS_PER_SEGMENT)
CRGB leds[NUM_LEDS];
CRGB TrailBuffer[NUM_LEDS];

//     +------+            +------+            +------+
//    /   43   \          /   24   \          /    8   \
//   /44      42\        /25      23\        /9        7\
//  +            +------+            +------+            +
//   \45        /   41   \26        /   22   \10       6/
//    \   46   /40        \   27   /21        \   11   /
//     +------+            +------+            +------+
//    /        \39        /        \20        /        \
//   /47        \        /28        \   19   /12       5\
//  +            +------+            +------+            +
//   \48        /   38   \29        /        \13       4/
//    \   49   /37        \   30   /18        \   14   /
//     +------+            +------+            +------+
//    /        \36        /        \17        /        \
//   /50        \   35   /34        \   16   /15       3\
//  +            +------+            +------+            +
//   \51      53/        \33      31/        \0        2/
//    \   52   /          \   32   /          \    1   /
//     +------+            +------+            +------+

Segment Segments[NUM_SEGMENTS] =
{
  { leds + ( 0 * LEDS_PER_SEGMENT), TrailBuffer + ( 0 * LEDS_PER_SEGMENT), {15, 16,  1, -1},  1, ORIENT_DOWN},
  { leds + ( 1 * LEDS_PER_SEGMENT), TrailBuffer + ( 1 * LEDS_PER_SEGMENT), { 0, -1,  2, -1},  1, ORIENT_HORIZ},
  { leds + ( 2 * LEDS_PER_SEGMENT), TrailBuffer + ( 2 * LEDS_PER_SEGMENT), { 1, -1,  3, -1},  1, ORIENT_UP},
  { leds + ( 3 * LEDS_PER_SEGMENT), TrailBuffer + ( 3 * LEDS_PER_SEGMENT), { 4, 14, -1,  2}, -1, ORIENT_DOWN},
  { leds + ( 4 * LEDS_PER_SEGMENT), TrailBuffer + ( 4 * LEDS_PER_SEGMENT), {14,  3,  5, -1},  1, ORIENT_UP},
  { leds + ( 5 * LEDS_PER_SEGMENT), TrailBuffer + ( 5 * LEDS_PER_SEGMENT), { 6, 11, -1,  4}, -1, ORIENT_DOWN},
  { leds + ( 6 * LEDS_PER_SEGMENT), TrailBuffer + ( 6 * LEDS_PER_SEGMENT), {11,  5,  7, -1},  1, ORIENT_UP},
  { leds + ( 7 * LEDS_PER_SEGMENT), TrailBuffer + ( 7 * LEDS_PER_SEGMENT), {-1,  8, -1,  6}, -1, ORIENT_DOWN},
  { leds + ( 8 * LEDS_PER_SEGMENT), TrailBuffer + ( 8 * LEDS_PER_SEGMENT), {-1,  9, -1,  7}, -1, ORIENT_HORIZ},
  { leds + ( 9 * LEDS_PER_SEGMENT), TrailBuffer + ( 9 * LEDS_PER_SEGMENT), {22, 10, -1,  8}, -1, ORIENT_UP},
  { leds + (10 * LEDS_PER_SEGMENT), TrailBuffer + (10 * LEDS_PER_SEGMENT), { 9, 22, 11, 12},  1, ORIENT_DOWN},
  { leds + (11 * LEDS_PER_SEGMENT), TrailBuffer + (11 * LEDS_PER_SEGMENT), {10, 12,  6,  5},  1, ORIENT_HORIZ},
  { leds + (12 * LEDS_PER_SEGMENT), TrailBuffer + (12 * LEDS_PER_SEGMENT), {19, 13, 10, 11}, -1, ORIENT_UP},
  { leds + (13 * LEDS_PER_SEGMENT), TrailBuffer + (13 * LEDS_PER_SEGMENT), {12, 19, 14, 15},  1, ORIENT_DOWN},
  { leds + (14 * LEDS_PER_SEGMENT), TrailBuffer + (14 * LEDS_PER_SEGMENT), {13, 15,  4,  3},  1, ORIENT_HORIZ},
  { leds + (15 * LEDS_PER_SEGMENT), TrailBuffer + (15 * LEDS_PER_SEGMENT), {16,  0, 13, 14}, -1, ORIENT_UP},
  { leds + (16 * LEDS_PER_SEGMENT), TrailBuffer + (16 * LEDS_PER_SEGMENT), {17, 31, 15,  0}, -1, ORIENT_HORIZ},
  { leds + (17 * LEDS_PER_SEGMENT), TrailBuffer + (17 * LEDS_PER_SEGMENT), {18, 30, 16, 31}, -1, ORIENT_DOWN},
  { leds + (18 * LEDS_PER_SEGMENT), TrailBuffer + (18 * LEDS_PER_SEGMENT), {30, 17, 20, 19},  1, ORIENT_UP},
  { leds + (19 * LEDS_PER_SEGMENT), TrailBuffer + (19 * LEDS_PER_SEGMENT), {20, 18, 12, 13},  1, ORIENT_HORIZ},
  { leds + (20 * LEDS_PER_SEGMENT), TrailBuffer + (20 * LEDS_PER_SEGMENT), {21, 27, 19, 18}, -1, ORIENT_DOWN},
  { leds + (21 * LEDS_PER_SEGMENT), TrailBuffer + (21 * LEDS_PER_SEGMENT), {27, 20, 23, 22},  1, ORIENT_UP},
  { leds + (22 * LEDS_PER_SEGMENT), TrailBuffer + (22 * LEDS_PER_SEGMENT), {23, 21,  9, 10},  1, ORIENT_HORIZ},
  { leds + (23 * LEDS_PER_SEGMENT), TrailBuffer + (23 * LEDS_PER_SEGMENT), {-1, 24, 22, 21}, -1, ORIENT_DOWN},
  { leds + (24 * LEDS_PER_SEGMENT), TrailBuffer + (24 * LEDS_PER_SEGMENT), {-1, 25, -1, 23}, -1, ORIENT_HORIZ},
  { leds + (25 * LEDS_PER_SEGMENT), TrailBuffer + (25 * LEDS_PER_SEGMENT), {41, 26, -1, 24}, -1, ORIENT_UP},
  { leds + (26 * LEDS_PER_SEGMENT), TrailBuffer + (26 * LEDS_PER_SEGMENT), {25, 41, 27, 28},  1, ORIENT_DOWN},
  { leds + (27 * LEDS_PER_SEGMENT), TrailBuffer + (27 * LEDS_PER_SEGMENT), {26, 28, 21, 20},  1, ORIENT_HORIZ},
  { leds + (28 * LEDS_PER_SEGMENT), TrailBuffer + (28 * LEDS_PER_SEGMENT), {38, 29, 26, 27}, -1, ORIENT_UP},
  { leds + (29 * LEDS_PER_SEGMENT), TrailBuffer + (29 * LEDS_PER_SEGMENT), {28, 38, 30, 34},  1, ORIENT_DOWN},
  { leds + (30 * LEDS_PER_SEGMENT), TrailBuffer + (30 * LEDS_PER_SEGMENT), {29, 34, 18, 17},  1, ORIENT_HORIZ},
  { leds + (31 * LEDS_PER_SEGMENT), TrailBuffer + (31 * LEDS_PER_SEGMENT), {32, -1, 17, 16}, -1, ORIENT_UP},
  { leds + (32 * LEDS_PER_SEGMENT), TrailBuffer + (32 * LEDS_PER_SEGMENT), {33, -1, 31, -1}, -1, ORIENT_HORIZ},
  { leds + (33 * LEDS_PER_SEGMENT), TrailBuffer + (33 * LEDS_PER_SEGMENT), {34, 35, 32, -1}, -1, ORIENT_DOWN},
  { leds + (34 * LEDS_PER_SEGMENT), TrailBuffer + (34 * LEDS_PER_SEGMENT), {35, 33, 29, 30},  1, ORIENT_UP},
  { leds + (35 * LEDS_PER_SEGMENT), TrailBuffer + (35 * LEDS_PER_SEGMENT), {36, 53, 34, 33}, -1, ORIENT_HORIZ},
  { leds + (36 * LEDS_PER_SEGMENT), TrailBuffer + (36 * LEDS_PER_SEGMENT), {37, 49, 35, 53}, -1, ORIENT_DOWN},
  { leds + (37 * LEDS_PER_SEGMENT), TrailBuffer + (37 * LEDS_PER_SEGMENT), {49, 36, 39, 38},  1, ORIENT_UP},
  { leds + (38 * LEDS_PER_SEGMENT), TrailBuffer + (38 * LEDS_PER_SEGMENT), {39, 37, 28, 29},  1, ORIENT_HORIZ},
  { leds + (39 * LEDS_PER_SEGMENT), TrailBuffer + (39 * LEDS_PER_SEGMENT), {40, 46, 38, 37}, -1, ORIENT_DOWN},
  { leds + (40 * LEDS_PER_SEGMENT), TrailBuffer + (40 * LEDS_PER_SEGMENT), {46, 39, 42, 41},  1, ORIENT_UP},
  { leds + (41 * LEDS_PER_SEGMENT), TrailBuffer + (41 * LEDS_PER_SEGMENT), {42, 40, 25, 26},  1, ORIENT_HORIZ},
  { leds + (42 * LEDS_PER_SEGMENT), TrailBuffer + (42 * LEDS_PER_SEGMENT), {-1, 43, 41, 40}, -1, ORIENT_DOWN},
  { leds + (43 * LEDS_PER_SEGMENT), TrailBuffer + (43 * LEDS_PER_SEGMENT), {-1, 44, -1, 42}, -1, ORIENT_HORIZ},
  { leds + (44 * LEDS_PER_SEGMENT), TrailBuffer + (44 * LEDS_PER_SEGMENT), {-1, 45, -1, 43}, -1, ORIENT_UP},
  { leds + (45 * LEDS_PER_SEGMENT), TrailBuffer + (45 * LEDS_PER_SEGMENT), {44, -1, 46, 47},  1, ORIENT_DOWN},
  { leds + (46 * LEDS_PER_SEGMENT), TrailBuffer + (46 * LEDS_PER_SEGMENT), {45, 47, 40, 39},  1, ORIENT_HORIZ},
  { leds + (47 * LEDS_PER_SEGMENT), TrailBuffer + (47 * LEDS_PER_SEGMENT), {-1, 48, 45, 46}, -1, ORIENT_UP},
  { leds + (48 * LEDS_PER_SEGMENT), TrailBuffer + (48 * LEDS_PER_SEGMENT), {47, -1, 49, 50},  1, ORIENT_DOWN},
  { leds + (49 * LEDS_PER_SEGMENT), TrailBuffer + (49 * LEDS_PER_SEGMENT), {48, 50, 37, 36},  1, ORIENT_HORIZ},
  { leds + (50 * LEDS_PER_SEGMENT), TrailBuffer + (50 * LEDS_PER_SEGMENT), {-1, 51, 48, 49}, -1, ORIENT_UP},
  { leds + (51 * LEDS_PER_SEGMENT), TrailBuffer + (51 * LEDS_PER_SEGMENT), {50, -1, 52, -1},  1, ORIENT_DOWN},
  { leds + (52 * LEDS_PER_SEGMENT), TrailBuffer + (52 * LEDS_PER_SEGMENT), {51, -1, 53, -1},  1, ORIENT_HORIZ},
  { leds + (53 * LEDS_PER_SEGMENT), TrailBuffer + (53 * LEDS_PER_SEGMENT), {52, -1, 36, 35},  1, ORIENT_UP}
};
//     +--<---+            +--<---+            +---<--+
//    /   43   ^          /   24   ^          /    8   ^
//   v44      42\        v25      23\        v9        7\
//  +            +----> +            +----> +            +
//   \45        ^   41   \26        ^   22   \10       6^
//    v   46   /40        v   27   /21        v   11   /
//     +----> +            +----> +            +----> +
//    /        ^39        /        ^20        /        ^
//   v47        \        v28        \   19   v12       5\
//  +            +----> +            +----> +            +
//   \48        ^   38   \29        ^        \13       4^
//    v   49   /37        v   30   /18        v   14   /
//     +----> +            +--->v +            +----> +
//    /        ^36              |  ^17        /        ^
//   v50        \   35   ^34     |  \   16   v15       3\
//  +            +---<--+         |  +--<---+            +
//   \51      53         ^33      31        |\0        2^
//    v   52   ^ |        \   32   v        | v    1   /
//     +--->--+  v         +---<--+         ^  +--->--+
#endif

double PixelToDistance(int segment, int pixel)
{
  
}


unsigned long LastUpdateTime = 0;
unsigned long MillisPerFrame = 1000 / FRAMES_PER_SECOND;



unsigned int Angle = 0;
unsigned char AngleStep = 2;

struct Trail
{
  void Init()
  {
    m_CurrSegment = random(NUM_SEGMENTS);
    m_CurrPos = random(LEDS_PER_SEGMENT);
    m_LastSegment = m_CurrSegment;
    m_LastPos = m_CurrPos;
    m_InterpolatePos = 0;
    m_CurrDirection = random(2) ? -1 : 1;
    m_ColorTheta = random(256);
    m_CurrSpeed = random(50, 256) / 15.0;
  }

  void Step()
  {
    m_InterpolatePos += m_CurrSpeed / FRAMES_PER_SECOND;
    m_ColorTheta += m_CurrSpeed / FRAMES_PER_SECOND;
    if (m_ColorTheta > 256)
      m_ColorTheta -= 256;

    if (m_InterpolatePos > 1)
    {
      m_LastPos = m_CurrPos;
      m_LastSegment = m_CurrSegment;

      m_CurrPos += m_CurrDirection * Segments[m_CurrSegment].m_Direction;
      m_InterpolatePos -= 1;
    }

    if (m_CurrPos < 0 || m_CurrPos >= LEDS_PER_SEGMENT)
    {
      //move to next segment
      int whichWay = random(2);
      int nextSegment = -1;
      if (m_CurrDirection < 0)
      {
        //move to next segment on left
        nextSegment = Segments[m_CurrSegment].m_Neighbors[0 + whichWay];
        if (nextSegment == -1)
          nextSegment = Segments[m_CurrSegment].m_Neighbors[0 + !whichWay];
      }
      else
      {
        //move to next segment on right
        nextSegment = Segments[m_CurrSegment].m_Neighbors[2 + whichWay];
        if (nextSegment == -1)
          nextSegment = Segments[m_CurrSegment].m_Neighbors[2 + !whichWay];
      }

      //did we change direction? (only happens when going from up to down or down to up - horiz always keeps the same direction)      
      if ((Segments[m_CurrSegment].m_Orientation == ORIENT_UP && Segments[nextSegment].m_Orientation == ORIENT_DOWN) ||
          (Segments[m_CurrSegment].m_Orientation == ORIENT_DOWN && Segments[nextSegment].m_Orientation == ORIENT_UP))
        m_CurrDirection = -m_CurrDirection;

      //what is the new position?
      if (m_CurrDirection * Segments[nextSegment].m_Direction > 0)
        m_CurrPos = 0;
      else
        m_CurrPos = LEDS_PER_SEGMENT - 1;
      m_CurrSegment = nextSegment;
      //Serial.println("NEW Segment: " + String(CurrSegment) + ", Pos: " + String(CurrPos) + ", Dir: " + String(CurrDirection) + ", SegDir: " + String(Segments[CurrSegment].m_Direction));
    }
    //else
    //{
    //  Serial.println("OLD Segment: " + String(CurrSegment) + ", Pos: " + String(CurrPos) + ", Dir: " + String(CurrDirection) + ", SegDir: " + String(Segments[CurrSegment].m_Direction));
    //}

    unsigned char colorTheta = (unsigned char)m_ColorTheta;
    //use integer overflow to cleanly wrap around 256 (does mod-256 for us)
    unsigned char r = sin8(colorTheta);
    unsigned char g = sin8(colorTheta + 85);
    unsigned char b = sin8(colorTheta + 171);

    unsigned char r1 = lerp8by8(0, r, (uint8_t)(m_InterpolatePos * 255));
    unsigned char r2 = lerp8by8(0, r, (uint8_t)((1 - m_InterpolatePos) * 255));
    unsigned char g1 = lerp8by8(0, g, (uint8_t)(m_InterpolatePos * 255));
    unsigned char g2 = lerp8by8(0, g, (uint8_t)((1 - m_InterpolatePos) * 255));
    unsigned char b1 = lerp8by8(0, b, (uint8_t)(m_InterpolatePos * 255));
    unsigned char b2 = lerp8by8(0, b, (uint8_t)((1 - m_InterpolatePos) * 255));

    if (Segments[m_CurrSegment].m_LEDs[m_CurrPos].r < r1)
      Segments[m_CurrSegment].m_LEDs[m_CurrPos].r = r1;
    if (Segments[m_CurrSegment].m_LEDs[m_CurrPos].g < g1)
      Segments[m_CurrSegment].m_LEDs[m_CurrPos].g = g1;
    if (Segments[m_CurrSegment].m_LEDs[m_CurrPos].b < b1)
      Segments[m_CurrSegment].m_LEDs[m_CurrPos].b = b1;

    if (Segments[m_LastSegment].m_LEDs[m_LastPos].r < r2)
      Segments[m_LastSegment].m_LEDs[m_LastPos].r = r2;
    if (Segments[m_LastSegment].m_LEDs[m_LastPos].g < g2)
      Segments[m_LastSegment].m_LEDs[m_LastPos].g = g2;
    if (Segments[m_LastSegment].m_LEDs[m_LastPos].b < b2)
      Segments[m_LastSegment].m_LEDs[m_LastPos].b = b2;
  }

  char m_CurrSegment;
  char m_LastSegment;
  char m_CurrPos;
  char m_LastPos;
  double m_InterpolatePos;
  signed char m_CurrDirection;
  double m_CurrSpeed;
  double m_ColorTheta;
};

Trail Trails[NUM_TRAILS];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //create random seed by analogRead-ing some floating pins
  randomSeed(analogRead(39) + analogRead(34));
  for (int i = 0; i < NUM_TRAILS; i++)
    Trails[i].Init();
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); 
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  LastUpdateTime = millis() - MillisPerFrame;
}

int LastFrames = 0;
int LastCountMS = 0;
int SleepTime = 0;
void loop()
{
  unsigned long now = millis();
  if (now - LastUpdateTime < MillisPerFrame)
  {
    //Serial.println("Sleeping for " + String(MillisPerFrame - (now - LastUpdateTime)) + " of " + String(MillisPerFrame));
    SleepTime += MillisPerFrame - (now - LastUpdateTime);
    delay(MillisPerFrame - (now - LastUpdateTime));
    return;
  }
  LastUpdateTime += MillisPerFrame;
  Angle += AngleStep;

  //dim all LEDs
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].r = leds[i].r * DimRate;
    leds[i].g = leds[i].g * DimRate;
    leds[i].b = leds[i].b * DimRate;
  }

  for (int i = 0; i < NUM_TRAILS; i++)
    Trails[i].Step();

  FastLED.show();
  LastFrames++;
  if (now - LastCountMS > 1000)
  {
    Serial.println("FPS = " + String(LastFrames) + ", Sleep time = " + String(SleepTime) + " of 1000 = " + String(100 * SleepTime / 1000) + "% idle");
    LastCountMS += 1000;
    LastFrames = 0;
    SleepTime = 0;
  }
}

// unsigned int Angle = 0;
// unsigned char AngleStep = 10;

// void loop() {
//   // put your main code here, to run repeatedly:
//   unsigned long now = millis();
//   if (now - LastUpdateTime < MillisPerFrame)
//   {
//     delay(MillisPerFrame - (now - LastUpdateTime));
//     return;
//   }

//   //while loop to account for possible missed frames
//   while (LastUpdateTime < now)
//   {
//     LastUpdateTime += MillisPerFrame;
//     Angle += AngleStep;
//   }

//   for (int i = 0; i < NUM_LEDS; i++)
//   {
//     unsigned char attenuationAngle = ((int)((NUM_LEDS - i) * 10 + Angle * 0.2)) % 256;
//     float attenuation = lerp8by8(20, 255, sin8(attenuationAngle)) / 255.0;
//     leds[i].r = sin8((i * 10) + Angle) * attenuation;
//     leds[i].g = sin8((i * 10) + Angle + 85) * attenuation;
//     leds[i].b = sin8((i * 10) + Angle + 171) * attenuation;
//   }
//   FastLED.show();
// }
