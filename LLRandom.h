#include "baseLight.h"

//------------------------------------
//LLRandom
class LLRandom : public baseLight
{
  public:
    LLRandom(eMode mode, int len)
      : baseLight(mode, len)
      , _randomT(1000)
      , _timer(0)
    {
      memset(_ledID, -1, RANDOM_LINE_NUM);
    }

    virtual void update(CRGB* ledData, long delta)
    {
      if (!_isPlaying)
      {
        return;
      }

      setColor(ledData);
      _timer -= delta;
      if (_timer <= 0)
      {
        _timer = _randomT;
        if (_playType == ePlayRepeat)
        {
          setRandom();
        }
      }
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_randomT, source + 2, sizeof(int));
      _timer = _randomT;
    }

    virtual void play(ePlayType t)
    {
      _isPlaying = true;
      _playType = t;
      setRandom();
    }

    virtual void stop()
    {
      _isPlaying = false;
    }

  private:
    void setColor(CRGB* ledData)
    {
      for (int i = 0; i < RANDOM_LINE_NUM; i++)
      {
        if (_ledID[i] == -1)
        {
          break;
        }
        int count = 0;
        for (int j = _ledID[i]; j < NUM_LEDS && count <= RANDOM_LINE_LENGTH; j++, count++)
        {
          ledData[j] = _color;
        }
      }
    }
    void setRandom()
    {
      memset(_ledID, -1, RANDOM_LINE_NUM);
      int id = 0;
      for (int i = 0; i < RANDOM_LINE_NUM; i++)
      {
        if (random(10) < 5 && _ledID[id] == -1)
        {
          _ledID[id] = random(0, NUM_LEDS);
          id++;
        }
      }
      _color = gColor;
      _color.val = 255;
    }
  private:
    int _timer;
    int _randomT;
    int _ledID[RANDOM_LINE_NUM];
    float _colorV;
    CHSV _color;
};

