#include "baseLight.h"

//------------------------------------
//LFlash
class LFlash : public baseLight
{
  public:
    LFlash(eMode mode, int len)
      : baseLight(mode, len)
      , _flashT(1000)
      , _timer(0)
    {
      memset(_needFlash, 1, 4);
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

    virtual void update(CRGB* ledData, long delta)
    {
      if (!_isPlaying || delta <= 0)
      {
        return;
      }

      _timer -= delta;
      if (_timer <= 0)
      {
        _timer = _flashT;
        setLight(ledData);
        if (_playType == ePlayOnce)
        {
          _isPlaying = false;
        }
      }
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_flashT, source + 2, sizeof(int));
      memcpy(_needFlash, source + 4, sizeof(boolean));
      memcpy(_needFlash + 1, source + 5, sizeof(boolean));
      memcpy(_needFlash + 2, source + 6, sizeof(boolean));
      memcpy(_needFlash + 3, source + 7, sizeof(boolean));

      _timer = 0;
    }

  private:
    void setLight(CRGB* ledData)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        int ledID = floor(i / (float)LED_EACH_SIDE);
        if (_needFlash[ledID])
        {
          ledData[i] = gColor;
        }
      }
    }
  private:
    int _timer;
    int _flashT;
    bool _updateFlash;
    boolean _needFlash[4];
};

