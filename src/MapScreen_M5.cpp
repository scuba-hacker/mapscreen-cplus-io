#include <MapScreen_M5.h>

#include <M5StickCPlus.h>

//#include <TFT_eSPI.h>

extern const unsigned short w1_1[];
extern const unsigned short w1_2[];
extern const unsigned short w1_3[];
extern const unsigned short w1_4[];
extern const unsigned short wraysbury_x1[];

const geo_map MapScreen_M5::s_maps[] =
{
  [0] = { .mapData = w1_1, .label="North", .backColour=TFT_BLACK, .backText="", .surveyMap=false, .swapBytes=false, .mapLongitudeLeft = -0.55, .mapLongitudeRight = -0.548, .mapLatitudeBottom = 51.4604},
  [1] = { .mapData = w1_2, .label="Cafe", .backColour=TFT_BLACK, .backText="", .surveyMap=false, .swapBytes=false, .mapLongitudeLeft = -0.5495, .mapLongitudeRight = -0.5475, .mapLatitudeBottom = 51.4593},
  [2] = { .mapData = w1_3, .label="Mid", .backColour=TFT_BLACK, .backText="", .surveyMap=false, .swapBytes=false, .mapLongitudeLeft = -0.5478, .mapLongitudeRight = -0.5458, .mapLatitudeBottom = 51.4588},
  [3] = { .mapData = w1_4, .label="South", .backColour=TFT_BLACK, .backText="", .surveyMap=false, .swapBytes=false, .mapLongitudeLeft = -0.5471, .mapLongitudeRight = -0.5451, .mapLatitudeBottom = 51.4583},
  [4] = { .mapData = wraysbury_x1, .label="All", .backColour=TFT_BLACK, .backText="", .surveyMap=false, .swapBytes=false, .mapLongitudeLeft = -0.5499, .mapLongitudeRight = -0.5452, .mapLatitudeBottom = 51.457350},
  [5] = { .mapData = nullptr, .label="Canoe", .backColour=TFT_CYAN, .backText="Canoe",.surveyMap=true, .swapBytes=false, .mapLongitudeLeft = -0.54910, .mapLongitudeRight = -0.54880, .mapLatitudeBottom = 51.46190}, // Canoe area
  [6] = { .mapData = nullptr, .label="Sub",  .backColour=TFT_CYAN, .backText="Sub",.surveyMap=true, .swapBytes=false, .mapLongitudeLeft = -0.54931, .mapLongitudeRight = -0.54900, .mapLatitudeBottom = 51.4608}, // Sub area
};

MapScreen_M5::MapScreen_M5(TFT_eSPI* tft, M5StickCPlus* m5) : MapScreen_ex(tft), _m5(m5)
{
  initMapScreen();
}

const MapScreen_ex::pixel MapScreen_M5::s_registrationPixels[MapScreen_M5::s_registrationPixelsSize] =
{
  [0] = { .x = o,       .y = o,    .colour = 0xFF00},
  [1] = { .x = hX_t3-o, .y = o,    .colour = 0xFF00},
  [2] = { .x = o,       .y = hY_t3-o, .colour = 0xFF00},
  [3] = { .x = hX_t3-o, .y = hY_t3-o, .colour = 0xFF00},
  
  [4] = { .x = hX_t3+o, .y = o,    .colour = 0xFFFF},
  [5] = { .x = mX_t3-o, .y = o,    .colour = 0xFFFF},
  [6] = { .x = hX_t3+o, .y = hY_t3-o, .colour = 0xFFFF},
  [7] = { .x = mX_t3-o, .y = hY_t3-o, .colour = 0xFFFF},

  [8]  = { .x = o,       .y = hY_t3+o, .colour = 0x00FF},
  [9]  = { .x = hX_t3-o, .y = hY_t3+o, .colour = 0x00FF},
  [10] = { .x = o,       .y = mY_t3-o, .colour = 0x00FF},
  [11] = { .x = hX_t3-o, .y = mY_t3-o, .colour = 0x00FF},

  [12] = { .x = hX_t3+o, .y = hY_t3+o, .colour = 0x0000},
  [13] = { .x = mX_t3-o, .y = hY_t3+o, .colour = 0x0000},
  [14] = { .x = hX_t3+o, .y = mY_t3-o, .colour = 0x0000},
  [15] = { .x = mX_t3-o, .y = mY_t3-o, .colour = 0x0000},
};

void MapScreen_M5::fillScreen(int colour)
{
  _tft->fillScreen(colour);
}

void MapScreen_M5::copyFullScreenSpriteToDisplay(TFT_eSprite* sprite)
{
    sprite->pushSprite(0,0);
}

void MapScreen_M5::writeBackTextToScreen(const geo_map* map)
{
    if (*map->backText)
    {
      _m5->Lcd.setCursor(5,5);
      _m5->Lcd.setTextSize(3);
      _m5->Lcd.setTextColor(TFT_BLACK, map->backColour);
      _m5->Lcd.println(map->backText);
    }
}

const geo_map* MapScreen_M5::getNextMapByPixelLocation(MapScreen_ex::pixel loc, const geo_map* thisMap)
{
  const geo_map* nextMap = thisMap;

  if (thisMap == _allLakeMap)
    return _allLakeMap;
    
  if ((thisMap == _canoeZoneMap || thisMap == _subZoneMap) && isPixelOutsideScreenExtent(loc))
  {
    nextMap = (thisMap == _canoeZoneMap ? _northMap : _cafeJettyMap);   // go back to previous map?
    _zoom = _priorToZoneZoom;
  }
  else if (thisMap == _northMap)   // go right from 0 to 1
  {
    if (isPixelInCanoeZone(loc, thisMap))
    {
      _priorToZoneZoom=_zoom;
      _zoom = 1;
      nextMap = _canoeZoneMap;
    }
    else if (isPixelInSubZone(loc, thisMap))
    {
      _priorToZoneZoom=_zoom;
      _zoom = 1;
      nextMap = _subZoneMap;
    }
    else if ((loc.x >= 116 && loc.y >= 118) || 
           loc.x >= 30 && loc.y >= 215)
    {
      nextMap=_cafeJettyMap;
    }
  }
  else if (thisMap == _cafeJettyMap)
  { 
    if (isPixelInSubZone(loc, thisMap))
    {
      _priorToZoneZoom=_zoom;
      _zoom = 1;
      nextMap = _subZoneMap;                  // reset zoom to 1
    }
    else if (loc.x >= 125 && loc.y >= 55 || 
        loc.x >= 135)      // go right from 1 to 2
    {
      nextMap=_midJettyMap;
    }
    else if (loc.x <=4 && loc.y <= 122 || 
            loc.x <= 83 && loc.y <= 30 )  // go left from 1 to 0
    {
      nextMap=_northMap;
    }
  }
  else if (thisMap == _midJettyMap)
  {
    if (loc.x >= 97 && loc.y >= 48|| 
        loc.y >= 175 && loc.x >= 42) // go right from 2 to 3
      nextMap=_southMap;
    else if (loc.x <= 0)
      nextMap=_cafeJettyMap;          // go left from 2 to 1
  }
  else if (thisMap == _southMap)
  {
    if  (loc.x <= 0 && loc.y <= 193 || 
         loc.x <= 39 && loc.y <= 119) // go left from 3 to 2
      nextMap = _midJettyMap;
  }

//  debugPixelMapOutput(loc, thisMap, nextMap);

  return nextMap;
}

// BOUNDING BOX FOR CANOE M5: TOP-LEFT (62, 51) BOT-RIGHT (79, 71) 
const MapScreen_ex::BoundingBox MapScreen_M5::boundingBoxesCanoe[] = {{{62,51},{79,71},{MapScreen_M5::_northMap}}};

bool MapScreen_M5::isPixelInCanoeZone(const MapScreen_ex::pixel loc, const geo_map* thisMap) const
{
  for (auto& box : boundingBoxesCanoe)
  {
    if (box.withinBox(loc, thisMap))
      return true;
  }

  return false;
}

// BOUNDING BOX FOR SUB M5 North Map: TOP-LEFT (48, 168) BOT-RIGHT (65, 191)
// BOUNDING BOX FOR SUB M5 Cafe Jetty Map: TOP-LEFT (12, 53) BOT-RIGHT (31, 72)
const MapScreen_ex::BoundingBox MapScreen_M5::boundingBoxesSub[] = {
                  {{48,168},{65,191},{MapScreen_M5::_northMap}},
                  {{12,53},{31,72},{MapScreen_M5::_cafeJettyMap}}
                  };

bool MapScreen_M5::isPixelInSubZone(const MapScreen_ex::pixel loc, const geo_map* thisMap) const
{
  for (auto& box : boundingBoxesSub)
  {
    if (box.withinBox(loc, thisMap))
      return true;
  }

  return false;
}