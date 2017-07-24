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
      _timer -= delta;
      if (_timer <= 0)
      {
        setRandom();
        _timer = _randomT;
      }

      fadeColor(ledData, delta);
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_randomT, source + 2, sizeof(int));
      _colorV = -255.0 / (_randomT / 1000.0);
      _timer = 0;
    }

    virtual void play(ePlayType t)
    {
      _isPlaying = true;
      _playType = t;
    }

    virtual void stop()
    {
      _isPlaying = false;
    }

  private:
    void fadeColor(CRGB* ledData, long delta)
    {
      float deltaS = delta / 1000.0;
      _color.val += _colorV * deltaS;
      if (_color.val >= 10)
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
      else
      {
        memset(_ledID, -1, RANDOM_LIGHT_NUM);
        if (_playType == ePlayOnce)
        {
          _isPlaying = false;
        }
      }
    }
    void setRandom()
    {
      int id = 0;
      for (int i = 0; i < RANDOM_LIGHT_NUM; i++)
      {
        if (random(10) < 5 && _ledID[id] == -1)
        {
          _ledID[id] = random(0, NUM_LEDS);
          id++;
        }
      }
      _color = CHSV(255, 0, 255);
    }
  private:
    int _timer;
    int _randomT;
    float _colorV;
    int _ledID[RANDOM_LIGHT_NUM];
    CHSV _color;
};

