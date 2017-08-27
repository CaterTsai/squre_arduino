#include<FastLED.h>
static CHSV gColor(0, 0, 128);

#include "constParameter.h"
#include "lightMode.h"

//---------------------
unsigned long timer;

//---------------------
//LED
CRGB _leds[NUM_LEDS];

void clearLED()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB::Black;
  }

}

//---------------------
//Serial
int _serialCount = 0;
int _targetLength = 0;
unsigned char _serialInput[MAX_SERIAL_LENGTH];
enum eSerialState
{
  eIdle = 0
  , eReadLength
  , eReadData
  , eWaitEnd
} _eState;

//---------------------
//Mode
baseLight* _modeList[10];

void initMode()
{
  _modeList[0] = new LFlash(eModeFlash, 8);
  _modeList[1] = new LPRandom(eModePRandom, 4);
  _modeList[2] = new LRunLine(eModeRunLine, 5);
  _modeList[3] = new LOpen(eModeOpen, 4);
  _modeList[4] = new LRunLineCenter(eModeRunLineC, 5);
  _modeList[5] = new LBreathe(eModeBreathe, 4);
}

void updateMode(unsigned long delta)
{
  for (int i = 0; i < eModeNum; i++)
  {
    _modeList[i]->update(_leds, delta);
  }
}

void processSerialData()
{
  int modeId = _serialInput[0];
  if (modeId < 0)
  {
    return;
  }
  //Serial.println(modeId);
  if (modeId == eModeColor && _serialInput[1] == 'c')
  {
    gColor.hue = _serialInput[2];
    gColor.sat = _serialInput[3];
    gColor.val = _serialInput[4];
  }
  else if(modeId == eModeStop && _serialInput[1] == 's')
  {
    for(int i = 0; i < eModeNum; i++)
    {
      _modeList[i]->stop();
    }
  }
  else
  {
    _modeList[modeId]->holdData(_serialInput, _targetLength);

    switch (_serialInput[1])
    {
      case 't':
        {
          _modeList[modeId]->play(ePlayOnce);
          break;
        }
      case 'p':
        {
          _modeList[modeId]->play(ePlayRepeat);
          break;
        }
      case 's':
        {
          _modeList[modeId]->stop();
          break;
        }
    }
  }
}

//------------------------------------------------
//Basic Function
void setup()
{
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(_leds, NUM_LEDS);
  FastLED.setBrightness(96);

  initMode();
  _eState = eIdle;
  testLight();
  Serial.begin(115200);
  //_modeList[5]->play(ePlayRepeat);
}

//------------------------------------------------
void loop()
{
  unsigned long delta = 15;
  
  clearLED();
  updateMode(delta);

  FastLED.show();
  delay(10);
}

//------------------------------------------------
void serialEvent() {

  int count = 0;
  _targetLength = 0;
  _serialCount = 0;
  _eState = eIdle;
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    count++;

    if (_eState == eIdle)
    {
      if (inChar == START_TAX)
      {
        _serialCount = 0;
        _eState = eReadLength;
      }
    }
    else if (_eState == eReadLength)
    {
      _targetLength = (unsigned int)inChar;
      _eState = eReadData;
    }
    else if (_eState == eReadData)
    {
      _serialInput[_serialCount] = inChar;
      _serialCount++;

      if (_serialCount == _targetLength)
      {
        _eState = eWaitEnd;
      }
    }
    else if (_eState == eWaitEnd)
    {
      if (inChar == END_TAX)
      {
        //Serial.println("END");
        processSerialData();
      }
      _targetLength = 0;
      _serialCount = 0;
      _eState = eIdle;
    }
  }
  //Serial.println(count);
  Serial.flush();
}

//-------------------------------
void testLight()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(255, 255, 0);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(255, 0, 255);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    _leds[i] = CRGB(0, 255, 255);
  }
  FastLED.show();
  delay(200);
}

