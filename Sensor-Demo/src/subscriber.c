#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <string.h>

#define MAX_SAMPLES 1

void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -s device_id  Filter by device ID (default: receive all)\n");
}

int main(int argc, char *argv[]) {
    char *filter_device_id = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            filter_device_id = argv[++i];
        }
    }

    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t reader;
    dds_return_t rc;
    SensorData_Reading *msg;
    void *samples[MAX_SAMPLES];
    dds_sample_info_t infos[MAX_SAMPLES];

    // Create participant
    participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant < 0) {
        printf("Error creating participant: %s\n", dds_strretcode(-participant));
        return 1;
    }

    // Create topic
    topic = dds_create_topic(participant, &SensorData_Reading_desc, 
                           "SensorTopic", NULL, NULL);
    if (topic < 0) {
        printf("Error creating topic: %s\n",  dds_strretcode(-topic));
        return 1;
    }


    if (filter_device_id) {
        printf("**Content filtering topic not implement yet.\n");
    }

    // Create reader
    reader = dds_create_reader(participant, topic, NULL, NULL);
    if (reader < 0) {
        printf("Error creating reader: %s\n", dds_strretcode(-reader));
        return 1;
    }

    samples[0] = SensorData_Reading__alloc();

    while (1) {
        rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc > 0) {
            msg = samples[0];
            if (infos[0].valid_data) {
                printf("Received: Device=%s, Temp=%.1f, Humidity=%.1f, Time=%d\n",
                       msg->device_id, msg->temperature, 
                       msg->humidity, msg->timestamp);
            }
        }
        dds_sleepfor(DDS_MSECS(100));
    }

    SensorData_Reading_free(samples[0], DDS_FREE_ALL);
    dds_delete(participant);
    return 0;
}