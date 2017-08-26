#include "baseLight.h"

//------------------------------------
//LBreathe
class LBreathe : public baseLight
{
  public:
    LBreathe(eMode mode, int len)
      : baseLight(mode, len)
      , _breatheT(1000)
      , _timer(0)
    {
      _animV = 1.0f / (_breatheT / 1000.0);
    }

    virtual void play(ePlayType t)
    {
      _isPlaying = true;
      _animP = 0.0;
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

      updateLight(ledData, delta);

 
    }

    virtual void holdData(unsigned char* source, int len)
    {
      if (len != _dataLength)
      {
        return;
      }
      memcpy(&_breatheT, source + 2, sizeof(int));
      _timer = _breatheT;
      _animV = 1.0f / (_breatheT / 1000.0);
    }

  private:
    void updateLight(CRGB* ledData, long delta)
    {
      CHSV color = gColor;
      float detlaS = delta / 1000.0;
      _animP += _animV * detlaS;

     if(_animP >= 1.0)
      {
        _animP = 1.0;
        _animV = -_animV;
      }
      else if(_animP < 0.0)
      {
        _animP = 0.0;
        _animV = -_animV;
      }
      
      color.val = (int)(easeInOutQuad(_animP) * 255.0);
      for (int i = 0; i < NUM_LEDS; i++)
      {
        ledData[i] = color;
      }
    }

    inline float easeInOutQuad(float p)
    {
      float t = p;
      return t < 0.5 ? 8 * t * t * t * t : 1 - 8 * (--t) * t * t * t;
    }
  private:
    int _timer;
    float _animP, _animV;
    int _breatheT;
};

