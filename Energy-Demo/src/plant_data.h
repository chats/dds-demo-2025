#ifndef PLANT_DATA_H
#define PLANT_DATA_H

#include "PowerPlantData.h"

typedef struct {
    const char *plant_id;
    enum PowerPlant_PlantType type;
    const char *location;
    double latitude;
    double longitude;
    double capacity;
} PlantInfo;

static const PlantInfo power_plants[] = {
    {"WIND001", PowerPlant_WIND, "Nakhon Ratchasima", 14.8798, 101.8198, 500.0},
    {"WIND002", PowerPlant_WIND, "Chaiyaphum", 15.8108, 102.0308, 450.0},
    {"WIND003", PowerPlant_WIND, "Phetchabun", 16.4192, 101.1589, 400.0},
    
    {"SOLAR001", PowerPlant_SOLAR, "Lopburi", 14.7995, 100.6557, 250.0},
    {"SOLAR002", PowerPlant_SOLAR, "Surin", 14.8827, 103.4942, 200.0},
    {"SOLAR003", PowerPlant_SOLAR, "Kanchanaburi", 14.0227, 99.5328, 300.0},
    {"SOLAR004", PowerPlant_SOLAR, "Ayutthaya", 14.3532, 100.5682, 180.0},
    {"SOLAR005", PowerPlant_SOLAR, "Chonburi", 13.3611, 100.9847, 220.0}
};

#define NUM_PLANTS (sizeof(power_plants) / sizeof(power_plants[0]))

#endif