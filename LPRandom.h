#include "baseLight.h"

//------------------------------------
//LPRandom
class LPRandom : public baseLight
{
  public:
    LPRandom(eMode mode, int len)
      : baseLight(mode, len)
      , _randomT(1000)
      , _timer(0)
    {
      memset(_ledID, -1, RANDOM_LIGHT_NUM);
      _colorV = -255.0 / (_randomT / 1000.0);
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
        if (_playType == ePlayRepeat)
        {
          setRandom();
        }
        _timer = _randomT;
      }


    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_randomT, source + 2, sizeof(int));
      _colorV = -255.0 / (_randomT / 1000.0);
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
      for (int i = 0; i < RANDOM_LIGHT_NUM; i++)
      {
        if (_ledID[i] == -1)
        {
          break;
        }
        ledData[_ledID[i]] = _color;
      }
    }

    void setRandom()
    {
      memset(_ledID, -1, RANDOM_LIGHT_NUM);
      int id = 0;
      for (int i = 0; i < RANDOM_LIGHT_NUM; i++)
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
    float _colorV;
    int _ledID[RANDOM_LIGHT_NUM];
    CHSV _color;
};

