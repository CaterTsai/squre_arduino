#include "baseLight.h"

//------------------------------------
//LRunLine
class LRunLine : public baseLight
{
  public:
    LRunLine(eMode mode, int len)
      : baseLight(mode, len)
      , _runT(1000)
      , _timer(0)
    {
      _animV = 1.0f / (_runT / 1000.0);
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
        addRunLine();
        _timer = _runT;
      }
      updateRunLine(ledData, delta);
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_runT, source + 2, sizeof(int));
      _timer = 0;
      _animV = 1.0f / (_runT / 1000.0);
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


      int sPos = (LED_EACH_SIDE * _pos);
      int ePos = (LED_EACH_SIDE * nextPos);
      _pos = nextPos;

      for (int i = sPos; i <= ePos; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          ledData[ePos + j * LED_EACH_SIDE] = CRGB::Red;
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
    int _ledID[RANDOM_LIGHT_NUM];
    float _animP, _animV;
    float _pos;

    CHSV _color;
};

