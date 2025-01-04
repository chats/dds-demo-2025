#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static dds_entity_t participant, topic, reader;
static dds_return_t rc;
static SensorData_Reading *msg;
static void *samples[MAX_SAMPLES] = {NULL};
static dds_sample_info_t infos[MAX_SAMPLES];
static char filter_device_id[DEVICE_ID_LEN] = {0};
static bool filtering = false;
static dds_domainid_t domain_id = DOMAIN_ID;
static const char *topic_name = TOPIC_NAME;

static volatile bool running = true;

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nReceived SIGINT, shutting down...\n");
    }
}

static void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -s device_id  Filter by device ID (default: receive all)\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: SensorTopic)\n");
}

void config(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        switch (argv[i][1]) {
            case 'h': print_usage(argv[0]); exit(0);
            case 'd': 
                if (i < argc ) {
                    domain_id = atoi(argv[++i]);

                }
                break;
            case 't': 
                if (i < argc ) {
                    topic_name = argv[++i];
                }
                break;            
            case 's': 
                if (++i < argc) {
                    strncpy(filter_device_id, argv[i], DEVICE_ID_LEN-1);
                    filter_device_id[DEVICE_ID_LEN-1] = '\0';
                    filtering = true;
                }
                break;
        }
    }
}

int setup() {
    if ((participant = dds_create_participant(domain_id, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &SensorData_Reading_desc, topic_name, NULL, NULL)) < 0 ||
        (reader = dds_create_reader(participant, topic, NULL, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-reader));
        return 1;
    }

    samples[0] = SensorData_Reading__alloc();

    if (filtering) {
        printf("Filtering for device: %s\n", filter_device_id);
    }
    return 0;
}


int main(int argc, char *argv[]) {

    config(argc, argv);
    
    signal(SIGINT, signal_handler);

    if (setup() != 0) {
        return 1;
    }

    printf("Subscriber running, Domain: %d, Topic=%s, press Ctrl-C to stop...\n", domain_id, topic_name);

    while (running) {
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

    printf("Cleaning up DDS entities...\n");
    SensorData_Reading_free(samples[0], DDS_FREE_ALL);
    dds_delete(participant);
    
    return 0;
}