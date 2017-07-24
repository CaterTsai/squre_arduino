#pragma once
#include "constParameter.h"

enum eMode
{
  eModeFlash = 0
  , eModePRandom
  , eModeLRandim
  , eModeRunLine
  , eModeOpen
  , eModeNum
};

enum ePlayType
{
  ePlayOnce = 0
  , ePlayRepeat
};

class baseLight
{
  public:
    baseLight()
      :_isPlaying(false)
      ,_dataLength(0)
    {}
    baseLight(eMode type, int len)
      : _isPlaying(false)
      ,_dataLength(len)
    {}

    virtual void update(CRGB* ledData, long delta) = 0;
    virtual void holdData(unsigned char* source, int len) = 0;
    virtual void play(ePlayType t) = 0;
    virtual void stop() = 0;

  protected:
    boolean _isPlaying;
    ePlayType _playType;
    eMode _lightType;
    int _dataLength;
};
