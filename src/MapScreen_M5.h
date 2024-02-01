#ifndef MAPSCREEN_M5_H
#define MAPSCREEN_M5_H

#include <MapScreen_ex.h>

#include "navigation_waypoints.h"

class M5StickCPlus;

class MapScreen_M5 : public MapScreen_ex
{
    static constexpr int s_registrationPixelsSize = 16;           // MBJ REFACTOR - HOW TO GET RID OF regpixelssize HERE? c.f. C array x[]
    static const std::array<MapScreen_ex::pixel, s_registrationPixelsSize> s_registrationPixels;

// These are screen dimensions plus half dimensions - must be the landscape t3
    static const int16_t mX_t3 = 135,  hX_t3 = 67;
    static const int16_t mY_t3 = 240,  hY_t3 = 120;
    static const int16_t o = 10;

    M5StickCPlus& _m5;

    virtual int getFirstDetailMapIndex() override { return _northMapIndex;}
    virtual int getEndDetailMaps() override { return _allLakeMapIndex; }
    virtual int getAllMapIndex() override { return _allLakeMapIndex; }
    virtual const geo_map* getMaps() override { return s_maps; }
    
    virtual const geo_map* getNextMapByPixelLocation(MapScreen_ex::pixel loc, const geo_map* thisMap) override;

    static const geo_map s_maps[];

    static constexpr const geo_map* _northMap=s_maps;         static const uint8_t _northMapIndex = 0;
    static constexpr const geo_map* _cafeJettyMap=s_maps+1;   static const uint8_t _cafeJettyMapIndex = 1;
    static constexpr const geo_map* _midJettyMap=s_maps+2;    static const uint8_t _midJettyMapIndex = 2;
    static constexpr const geo_map* _southMap=s_maps+3;       static const uint8_t _southMapIndex = 3;
    static constexpr const geo_map* _allLakeMap=s_maps+4;     static const uint8_t _allLakeMapIndex = 4;
    static constexpr const geo_map* _canoeZoneMap=s_maps+5;   static const uint8_t _canoeZoneMapIndex = 5;
    static constexpr const geo_map* _subZoneMap=s_maps+6;     static const uint8_t _subZoneMapIndex = 6;


    static const std::array<MapScreen_ex::MapScreen_ex::BoundingBox, 1> boundingBoxesCanoe;
    static const std::array<MapScreen_ex::MapScreen_ex::BoundingBox, 2> boundingBoxesSub;

    static const int maxFeatures = 255;
    std::array<geoRef, maxFeatures>    _featureToMaps;

    public:
        MapScreen_M5(TFT_eSPI& tft, M5StickCPlus& m5);

        virtual MapScreen_ex::pixel getRegistrationMarkLocation(int index) override;

        virtual int getRegistrationMarkLocationsSize() override { return s_registrationPixelsSize; }

        virtual int16_t getTFTWidth() const override {return 135;}
        virtual int16_t getTFTHeight() const override {return 240; }

        virtual void fillScreen(int colour) override;
        virtual void copyFullScreenSpriteToDisplay(TFT_eSprite& sprite) override;
        virtual void writeMapTitleToSprite(TFT_eSprite& sprite, const geo_map& map) override;

        virtual bool isPixelInCanoeZone(const MapScreen_ex::pixel loc, const geo_map& thisMap) const override;
        virtual bool isPixelInSubZone(const MapScreen_ex::pixel loc, const geo_map& thisMap) const override;
};
#endif

