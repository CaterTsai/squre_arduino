#include<FastLED.h>
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
  _modeList[2] = new LLRandom(eModeLRandim, 4);
  _modeList[3] = new LRunLine(eModeRunLine, 4);
  _modeList[4] = new LOpen(eModeOpen, 4);
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
  if (modeId < 0 || modeId > eModeNum)
  {
    return;
  }
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

//------------------------------------------------
//Basic Function
void setup()
{
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(_leds, NUM_LEDS);

  timer = millis();
  initMode();
  _modeList[4]->play(ePlayRepeat);
  _eState = eIdle;
  Serial.begin(9600);
}

//------------------------------------------------
void loop()
{
  unsigned long delta = millis() - timer;
  timer += delta;
  clearLED();
  updateMode(delta);

  FastLED.show();
}

//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

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
        processSerialData();
      }
      _targetLength = 0;
      _serialCount = 0;
      _eState = eIdle;
    }
  }
}
