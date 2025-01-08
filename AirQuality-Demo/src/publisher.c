#include "dds/dds.h"
#include "AirQuality.h"
#include "district_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"

#define UPDATE_INTERVAL 5  // 5 seconds

static volatile bool running = true;
static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

static float generate_pm25(void) {
    return 10.0 + (rand() % 600) / 10.0;  // 10-70 μg/m³
}

static float generate_pm10(void) {
    return 20.0 + (rand() % 800) / 10.0;  // 20-100 μg/m³
}

static float calculate_aqi(float pm25, float pm10) {
    // Simplified AQI calculation based on PM2.5
    return pm25 * 2;  // Example calculation
}

int main(void) {
    signal(SIGINT, signal_handler);
    srand(time(NULL));

    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t writer;
    AirQuality_AQIData msg;

    participant = dds_create_participant(DOMAIN_ID, NULL, NULL);

    // Set QoS for reliability, durability, and history
    dds_qos_t *qos = dds_create_qos();
    //dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(1));
    //dds_qset_durability(qos, DDS_DURABILITY_TRANSIENT_LOCAL);
    //dds_qset_history(qos, DDS_HISTORY_KEEP_LAST, 1);


    topic = dds_create_topic(participant, &AirQuality_AQIData_desc, 
                           TOPIC_NAME, qos, NULL);
    writer = dds_create_writer(participant, topic, qos, NULL);

    printf("Publishing AQI data every %d seconds. Press Ctrl-C to stop.\n", 
           UPDATE_INTERVAL);

    while (running) {
        for (size_t i = 0; i < NUM_DISTRICTS; i++) {
            //msg.district_id = districts[i].id;
            strncpy(msg.district_id, districts[i].id, sizeof(msg.district_id));
            //msg.name = districts[i].name;
            strncpy(msg.name, districts[i].name, sizeof(msg.name));
            msg.latitude = districts[i].latitude;
            msg.longitude = districts[i].longitude;
            
            msg.pm25 = generate_pm25();
            msg.pm10 = generate_pm10();
            msg.aqi = calculate_aqi(msg.pm25, msg.pm10);
            //msg.level = get_aqi_level(msg.aqi);
            strncpy(msg.level, get_aqi_level(msg.aqi), sizeof(msg.level));

            dds_write(writer, &msg);
            printf("Published AQI=%.1f (%s) for %s\n", 
                   msg.aqi, msg.level, msg.name);
        }

        dds_sleepfor(DDS_SECS(UPDATE_INTERVAL));
    }

    dds_delete(participant);
    dds_delete_qos(qos);
    return 0;
}