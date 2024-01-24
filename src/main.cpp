#include <Arduino.h>

#include <M5StickCPlus.h>
#include <MapScreen_M5.h>

std::unique_ptr<MapScreen_M5> mapScreen;

#include <memory.h>
#include "esp_heap_caps.h"

#include "dive_track.h"

extern const location diveTrack[];
extern "C" int getSizeOfDiveTrack();

class geo_location
{
  public:
    double la;
    double lo;
    int heading;
};

geo_location pos;

int trackIndex=0;
int trackLength=getSizeOfDiveTrack();
int testIteration=0;

void cycleTrackIndex();
void TestRotateMaps();

extern const unsigned short w1_1[];
extern const unsigned short w1_2[];
extern const unsigned short w1_3[];
extern const unsigned short w1_4[];
extern const unsigned short wraysbury_x1[];


void setup()
{
  Serial.begin(115200);
  Serial.flush();
  delay(50);

  M5.begin();  
  M5.Lcd.setRotation(0);
  mapScreen.reset(new MapScreen_M5(&M5.Lcd,&M5));

  mapScreen->setDrawAllFeatures(true);

  multi_heap_info_t info;
  heap_caps_get_info(&info, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT); // internal RAM, memory capable to store data or to create new task
  info.total_free_bytes;   // total currently free in all non-continues blocks
  info.minimum_free_bytes;  // minimum free ever
  info.largest_free_block;   // largest continues block to allocate big array

  // Serial.println("Managed to allocate 2 x 8-bit 320x240 = 76800 bytes");
  // Serial.printf("free bytes: %i  largest free block: %i\n",  info.total_free_bytes, info.largest_free_block);
  
  mapScreen->setUseDiverHeading(true);

  pos.la = 51.4601028571429;    // spitfire car
  pos.lo = -0.54883835;

  pos.la = 51.4605855;    // lightning boat
  pos.lo = -0.54890166666666; 

//  TestRotateMaps();

  return;
}

void loop()
{ 
  mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

  if (trackIndex == 0)
  {
    mapScreen->setAllLakeShown(false);
    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty
  }
  else if (trackIndex == 200)
  {
    mapScreen->setDrawAllFeatures(true);
  }
  else if (trackIndex == 400)
  {
    mapScreen->setTargetWaypointByLabel("Canoe"); // Mid Jetty    
  }
  else if (trackIndex == 500)
  {
    mapScreen->setAllLakeShown(true);
  }
  else if (trackIndex == 600)
  {
     mapScreen->setZoom(1);
  }
  else if (trackIndex == 700)
  {
     mapScreen->setZoom(2);
  }
  else if (trackIndex == 800)
  {
     mapScreen->setZoom(3);
  }
  else if (trackIndex == 900)
  {
     mapScreen->setZoom(4);
  }
  else if (trackIndex == 1000)
  {
     mapScreen->setZoom(3);
  }
  else if (trackIndex == 1100)
  {
     mapScreen->setZoom(2);
  }
  else if (trackIndex == 1100)
  {
     mapScreen->setZoom(1);
     mapScreen->setDrawAllFeatures(false);
  }

  M5.update();

  if (M5.BtnA.isPressed() && M5.BtnB.isPressed())
  {
     pos.lo -= 0.0002;
     pos.la += 0.0002;
  }
  else if (M5.BtnA.isPressed())
  {
     mapScreen->toggleDrawAllFeatures();  
     delay(100);
     pos.lo -= 0.0001;
  }
  else if (M5.BtnB.isPressed())
     pos.la += 0.0001;
  else if (M5.Axp.GetBtnPress() == 0x02)
  {
    mapScreen->cycleZoom();
    while(M5.Axp.GetBtnPress() == 0x02);
  }

  bool useTrack = true;
  
  if (useTrack)
  {
    mapScreen->drawDiverOnBestFeaturesMapAtCurrentZoom(diveTrack[trackIndex]._la,diveTrack[trackIndex]._lo,diveTrack[trackIndex]._h);
  }
  else
  {
    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

    mapScreen->drawDiverOnBestFeaturesMapAtCurrentZoom(pos.la,pos.lo,pos.heading);

    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

    const uint16_t headingStep = 5;

    pos.heading+=headingStep;
    pos.heading%=360;
  }
    
  cycleTrackIndex();
  testIteration++;
}

void cycleTrackIndex()
{
  trackIndex = (trackIndex + 1) % trackLength;

  pos.lo += 0.00004;
  pos.la -= 0.00002;
}

void TestRotateMaps()
{
  repeat:
  M5.Lcd.pushImage(0,0,135,240,w1_1);
  delay(1000);
  M5.Lcd.pushImage(0,0,135,240,w1_2);
  delay(1000);
  M5.Lcd.pushImage(0,0,135,240,w1_3);
  delay(1000);
  M5.Lcd.pushImage(0,0,135,240,w1_4);
  delay(1000);
  M5.Lcd.pushImage(0,0,135,240,wraysbury_x1);
  delay(1000);
  goto repeat;
}
