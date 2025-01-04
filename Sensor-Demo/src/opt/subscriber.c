#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <string.h>

#define MAX_SAMPLES 1
#define DEVICE_ID_LEN 20

static void print_usage(const char *program) {
    printf("Usage: %s [-h] [-s device_id]\n", program);
}

int main(int argc, char *argv[]) {
    dds_entity_t participant, topic, reader;
    dds_return_t rc;
    SensorData_Reading *msg;
    void *samples[MAX_SAMPLES] = {NULL};
    dds_sample_info_t infos[MAX_SAMPLES];
    char filter_device_id[DEVICE_ID_LEN] = {0};
    bool filtering = false;
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        switch (argv[i][1]) {
            case 'h': print_usage(argv[0]); return 0;
            case 's': 
                if (++i < argc) {
                    strncpy(filter_device_id, argv[i], DEVICE_ID_LEN-1);
                    filter_device_id[DEVICE_ID_LEN-1] = '\0';
                    filtering = true;
                }
                break;
        }
    }

    if ((participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &SensorData_Reading_desc, "SensorTopic", NULL, NULL)) < 0 ||
        (reader = dds_create_reader(participant, topic, NULL, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-reader));
        return 1;
    }

    samples[0] = SensorData_Reading__alloc();

    if (filtering) {
        printf("Filtering for device: %s\n", filter_device_id);
    }

    while (1) {
        rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc > 0 && infos[0].valid_data) {
            msg = samples[0];
            if (!filtering || strcmp(msg->device_id, filter_device_id) == 0) {
                printf("Received: %s, T=%.1f, H=%.1f\n",
                       msg->device_id, msg->temperature, msg->humidity);
            }
        }
        dds_sleepfor(DDS_MSECS(100));
    }

    SensorData_Reading_free(samples[0], DDS_FREE_ALL);
    dds_delete(participant);
    return 0;
}