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
      ,_eType(eCounterCirclewiseNoFull)
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
      memcpy(&_eType, source + 4, sizeof(bool));
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

      int sPos = getStartPos();
      int ePos = getEndPos(nextPos);
      if(sPos > ePos)
      {
        int temp = sPos;
        sPos = ePos;
        ePos = temp;
      }
      _pos = nextPos;

      for (int i = sPos; i <= ePos; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          ledData[i + j * LED_EACH_SIDE] = CRGB::Red;
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

    int getStartPos()
    {
      float r;
      switch(_eType)
      {
        case eCirclewiseFull:
        {
          r = 0;
          break;
        }
        case eCirclewiseNoFull:
        {
          r = LED_EACH_SIDE * _pos;
          break;
        }
        case eCounterCirclewiseFull:
        {
          r = LED_EACH_SIDE - 1;
          break;
        }
        case eCounterCirclewiseNoFull:
        default:
        {
          r = LED_EACH_SIDE * (1.0 - _pos);
          break;
        }
      }
      return r;
    }

    int getEndPos(float pos)
    {
      float r;
      switch(_eType)
      {
        case eCirclewiseFull:
        case eCirclewiseNoFull:
        {
          r = LED_EACH_SIDE * pos;
          break;
        }
        case eCounterCirclewiseFull:
        case eCounterCirclewiseNoFull:
        default:
        {
          r = LED_EACH_SIDE * (1.0 - pos);
          break;
        }

        Serial.println(r);
      }
      return r;
    }
  private:
    int _timer;
    int _runT;
    int _ledID[RANDOM_LIGHT_NUM];
    float _animP, _animV;
    float _pos;
    enum eType
    {
      eCirclewiseFull = 0
      ,eCirclewiseNoFull
      ,eCounterCirclewiseFull
      ,eCounterCirclewiseNoFull
    }_eType;

    CHSV _color;
};

