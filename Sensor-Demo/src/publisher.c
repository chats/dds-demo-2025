#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MIN_TEMP 15.0
#define MAX_TEMP 45.0
#define MAX_TEMP_CHANGE 0.5
#define MAX_SENSORS 10

typedef struct {
    char id[20];
    double current_temp;
} Sensor;

Sensor sensors[MAX_SENSORS];
int sensor_count = 0;

double get_next_temperature(double current_temp) {
    double change = (((double)rand() / RAND_MAX) * 2 - 1) * MAX_TEMP_CHANGE;
    double new_temp = current_temp + change;
    if (new_temp < MIN_TEMP) new_temp = MIN_TEMP;
    if (new_temp > MAX_TEMP) new_temp = MAX_TEMP;
    return new_temp;
}

double calculate_humidity(double temperature) {
    const double avg = 0.7;
    double humidity = temperature / avg;
    humidity += (((double)rand() / RAND_MAX) * 4.0 - 2.0);
    return humidity;
}

void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -v            Enable verbose output\n");
    printf("  -s device_id  Add a sensor (can be used multiple times)\n");
}

int main(int argc, char *argv[]) {
    bool verbose = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            if (sensor_count < MAX_SENSORS) {
                strcpy(sensors[sensor_count].id, argv[++i]);
                sensors[sensor_count].current_temp = 25.0;
                sensor_count++;
            }
        }
    }

    if (sensor_count == 0) {
        strcpy(sensors[0].id, "sensor-001");
        sensors[0].current_temp = 25.0;
        sensor_count = 1;
    }

    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t writer;
    dds_return_t rc;
    SensorData_Reading msg;

    srand(time(NULL));

    participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant < 0) {
        printf("Error creating participant: %s\n", dds_strretcode(-participant));
        return 1;
    }

    topic = dds_create_topic(participant, &SensorData_Reading_desc, 
                           "SensorTopic", NULL, NULL);
    if (topic < 0) {
        printf("Error creating topic: %s\n", dds_strretcode(-topic));
        return 1;
    }

    writer = dds_create_writer(participant, topic, NULL, NULL);
    if (writer < 0) {
        printf("Error creating writer: %s\n", dds_strretcode(-writer));
        return 1;
    }

    printf("Publishing data for %d sensor(s)\n", sensor_count);
    while (1) {
        for (int i = 0; i < sensor_count; i++) {
            sensors[i].current_temp = get_next_temperature(sensors[i].current_temp);

            msg.device_id = sensors[i].id;
            msg.temperature = sensors[i].current_temp;
            msg.humidity = calculate_humidity(sensors[i].current_temp);
            msg.timestamp = (long)time(NULL);

            rc = dds_write(writer, &msg);
            if (rc != DDS_RETCODE_OK) {
                printf("Error writing sample: %s\n", dds_strretcode(-rc));
            } else if (verbose) {
                printf("Published: Device=%s, Temp=%.1f, Humidity=%.1f, Time=%d\n",
                       msg.device_id, msg.temperature, msg.humidity, msg.timestamp);
            }
        }
        dds_sleepfor(DDS_SECS(1));
    }

    dds_delete(participant);
    return 0;
}