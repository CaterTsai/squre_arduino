#include "baseLight.h"

//------------------------------------
//LOpen
class LOpen : public baseLight
{
  public:
    LOpen(eMode mode, int len)
      : baseLight(mode, len)
      , _openT(1000)
      , _timer(0)
    {
      _animV = 1.0f / (_openT / 1000.0);
    }

    virtual void play(ePlayType t)
    {
      _isPlaying = true;
      _playType = t;
      open();
    }

    virtual void stop()
    {
      _isPlaying = false;
    }

    virtual void update(CRGB* ledData, long delta)
    {
      
      if (!_isPlaying)
      {
        return;
      }
      updateOpen(ledData, delta);
      
      _timer -= delta;
      if (_timer <= 0)
      {
        _timer = _openT;
        if (_playType == ePlayRepeat)
        {
          open();
        }
      }
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_openT, source + 2, sizeof(int));
      _timer = _openT;
      _animV = 1.0f / (_openT / 1000.0);
    }

  private:
    void open()
    {
      _animP = 0.0;
      _openP1 = random(0, NUM_LEDS_HALF - 1);
    }

    void updateOpen(CRGB* ledData, long delta)
    {
      float detlaS = delta / 1000.0;
      _animP += _animV * detlaS;
      int len = static_cast<int>(LED_EACH_SIDE * _animP + 0.5);

      for (int i = 0; i < NUM_LEDS_HALF; i++)
      {
        int dist = abs(i - _openP1);
        dist = min(dist, NUM_LEDS_HALF - dist);

        if (dist <= len)
        {
          ledData[i] = gColor;
          ledData[i + NUM_LEDS_HALF] = gColor;
        }
      }

      if (_animP >= 1.0)
      {
        if (_playType == ePlayOnce)
        {
          _isPlaying = false;
        }
      }
    }
  private:
    int _timer;
    int _openT;
    float _animP, _animV;
    int _openP1;
};

