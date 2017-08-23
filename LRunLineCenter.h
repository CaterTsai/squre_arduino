#include "baseLight.h"

//------------------------------------
//LRunLineCenter
class LRunLineCenter : public baseLight
{
  public:
    LRunLineCenter(eMode mode, int len)
      : baseLight(mode, len)
      , _runT(1000)
      , _timer(0)
      , _isFull(false)
    {
      _animV = 1.0f / (_runT / 1000.0);
    }

    virtual void update(CRGB* ledData, long delta)
    {
      if (!_isPlaying)
      {
        return;
      }
      updateRunLine(ledData, delta);
      _timer -= delta;
      if (_timer <= 0)
      {
        if (_playType == ePlayRepeat)
        {
          addRunLine();
        }
        _timer = _runT;
      }
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_runT, source + 2, sizeof(int));
      memcpy(&_isFull, source + 4, sizeof(bool));
      _timer = _runT;
      _animV = 1.0f / (_runT / 1000.0);
    }

    virtual void play(ePlayType t)
    {
      _isPlaying = true;
      _playType = t;
      addRunLine();
    }

    virtual void stop()
    {
      _isPlaying = false;
    }

  private:
    void addRunLine()
    {
      _animP = 0.0;
      _pos = 0.0f;
    }

    void updateRunLine(CRGB* ledData, long delta)
    {
      float detlaS = delta / 1000.0;
      _animP += _animV * detlaS;
      float nextPos = easeInOutQuad(_animP);

      int len = static_cast<int>(LED_EACH_SIDE * nextPos * 0.5 + 0.5);
      int s = _isFull ? 0 : len;
      int center = LED_EACH_SIDE * 0.5;
      for (int i = s; i <= len; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          ledData[center + i + j * LED_EACH_SIDE] = gColor;
          ledData[center - i + j * LED_EACH_SIDE] = gColor;
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

    inline float easeInOutQuad(float p)
    {
      float t = p;
      return t < 0.5 ? 8 * t * t * t * t : 1 - 8 * (--t) * t * t * t;
    }

  private:
    int _timer;
    int _runT;
    float _animP, _animV;
    float _pos;
    bool _isFull;
};

