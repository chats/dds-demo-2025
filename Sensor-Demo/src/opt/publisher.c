#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_TEMP 15.0
#define MAX_TEMP 45.0
#define MAX_TEMP_CHANGE 0.5
#define MAX_SENSORS 10
#define DEVICE_ID_LEN 20

typedef struct {
    char id[DEVICE_ID_LEN];
    double current_temp;
} Sensor;

static Sensor sensors[MAX_SENSORS];
static size_t sensor_count = 0;
static const double HUMIDITY_FACTOR = 0.7;

static inline double get_next_temperature(const double current_temp) {
    const double change = (((double)rand() / RAND_MAX) * 2 - 1) * MAX_TEMP_CHANGE;
    double new_temp = current_temp + change;
    return new_temp < MIN_TEMP ? MIN_TEMP : (new_temp > MAX_TEMP ? MAX_TEMP : new_temp);
}

static inline double calculate_humidity(const double temperature) {
    return (temperature / HUMIDITY_FACTOR) + (((double)rand() / RAND_MAX) * 4.0 - 2.0);
}

static void print_usage(const char *program) {
    printf("Usage: %s [-h] [-v] [-s device_id ...]\n", program);
}

int main(int argc, char *argv[]) {
    bool verbose = false;
    dds_entity_t participant, topic, writer;
    dds_return_t rc;
    SensorData_Reading msg = {0};
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        switch (argv[i][1]) {
            case 'h': print_usage(argv[0]); return 0;
            case 'v': verbose = true; break;
            case 's': 
                if (++i < argc && sensor_count < MAX_SENSORS) {
                    strncpy(sensors[sensor_count].id, argv[i], DEVICE_ID_LEN-1);
                    sensors[sensor_count].id[DEVICE_ID_LEN-1] = '\0';
                    sensors[sensor_count++].current_temp = 25.0;
                }
                break;
        }
    }

    if (!sensor_count) {
        strcpy(sensors[0].id, "sensor-001");
        sensors[0].current_temp = 25.0;
        sensor_count = 1;
    }

    srand(time(NULL));

    if ((participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &SensorData_Reading_desc, "SensorTopic", NULL, NULL)) < 0 ||
        (writer = dds_create_writer(participant, topic, NULL, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-writer));
        return 1;
    }

    while (1) {
        for (size_t i = 0; i < sensor_count; i++) {
            sensors[i].current_temp = get_next_temperature(sensors[i].current_temp);
            msg.device_id = sensors[i].id;
            msg.temperature = sensors[i].current_temp;
            msg.humidity = calculate_humidity(sensors[i].current_temp);
            msg.timestamp = time(NULL);

            if ((rc = dds_write(writer, &msg)) != DDS_RETCODE_OK) {
                fprintf(stderr, "Error: %s\n", dds_strretcode(-rc));
            } else if (verbose) {
                printf("Published: %s, T=%.1f, H=%.1f\n",
                       msg.device_id, msg.temperature, msg.humidity);
            }
        }
        dds_sleepfor(DDS_SECS(1));
    }

    dds_delete(participant);
    return 0;
}