#include "dds/dds.h"
#include "VehicleGPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static dds_entity_t participant, topic, reader;
static dds_return_t rc;
static bool filtering = false;
static dds_domainid_t domain_id = DOMAIN_ID;
static const char *topic_name = TOPIC_NAME;

static VehicleGPS_Location *msg;
static void *samples[MAX_SAMPLES];
static dds_sample_info_t infos[MAX_SAMPLES];

static char *filter_vehicle_id = NULL;

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
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: SensorTopic)\n");
    printf("  -s vehicle_id Filter by vehicle ID (default: receive all)\n");
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
                if (i < argc) {
                    filter_vehicle_id = argv[++i];
                }
                break;
        }
    }
}

int setup() {
    if ((participant = dds_create_participant(domain_id, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &VehicleGPS_Location_desc, topic_name, NULL, NULL)) < 0 ||
        (reader = dds_create_reader(participant, topic, NULL, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-reader));
        return 1;
    }

    samples[0] = VehicleGPS_Location__alloc();

    if (filtering) {
        printf("Filtering for vehicle: %s\n", filter_vehicle_id);
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
        if (rc > 0) {
            msg = samples[0];
            if (infos[0].valid_data) {
                if (!filter_vehicle_id || strcmp(msg->vehicle_id, filter_vehicle_id) == 0) {
                    printf("Received: Vehicle=%s, Location=(%.8f, %.8f)\n",
                           msg->vehicle_id, msg->latitude, msg->longitude);
                }
            }
        }
        dds_sleepfor(DDS_MSECS(100));
    }

    printf("Cleaning up DDS entities...\n");
    VehicleGPS_Location_free(samples[0], DDS_FREE_ALL);
    dds_delete(participant);
    
    return 0;
}