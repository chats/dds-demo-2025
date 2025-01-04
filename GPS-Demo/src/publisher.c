#include "dds/dds.h"
#include "VehicleGPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "config.h"
#include "gps_data.h"

static dds_domainid_t domain_id = DOMAIN_ID;
static const char *topic_name = TOPIC_NAME;
static dds_entity_t participant, topic, writer;
static dds_return_t rc;

static char *vehicle_id = DEFAULT_VEHICLE_ID;
static VehicleGPS_Location msg;

static bool verbose = false;
static volatile bool running = true;

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nReceived SIGINT, shutting down...\n");
    }
}

/*
static inline double get_next_temperature(const double current_temp) {
    const double change = (((double)rand() / RAND_MAX) * 2 - 1) * MAX_TEMP_CHANGE;
    double new_temp = current_temp + change;
    return new_temp < MIN_TEMP ? MIN_TEMP : (new_temp > MAX_TEMP ? MAX_TEMP : new_temp);
}

static inline double calculate_humidity(const double temperature) {
    return (temperature / HUMIDITY_FACTOR) + (((double)rand() / RAND_MAX) * 4.0 - 2.0);
}
*/

static void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -v            Enable verbose output\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: SensorTopic)\n");
    printf("  -s vehicle_id Set vehicle ID (default: vehicle-001)\n");
}

void config(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        switch (argv[i][1]) {
            case 'h': print_usage(argv[0]); exit(0);
            case 'v': verbose = true; break;
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
                if (i < argc ) {
                    vehicle_id = argv[++i];
                }
                break;            
        }
    }
}

int setup() {
//    srand(time(NULL));

    if ((participant = dds_create_participant(domain_id, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &VehicleGPS_Location_desc, topic_name, NULL, NULL)) < 0 ||
        (writer = dds_create_writer(participant, topic, NULL, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-writer));
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    
    config(argc, argv);
    
    signal(SIGINT, signal_handler);

    if (setup() != 0) {
        return 1;
    }

    printf("Publisher running, Domain: %d, Topic=%s, press Ctrl-C to stop...\n", domain_id, topic_name);

    size_t current_point = 0;

    while (running) {
        msg.vehicle_id = vehicle_id;
        msg.latitude = gps_waypoints[current_point].latitude;
        msg.longitude = gps_waypoints[current_point].longitude;
        //msg.location_name = (char *)gps_waypoints[current_point].location;
        msg.timestamp = (long)time(NULL);

        rc = dds_write(writer, &msg);
        if (rc != DDS_RETCODE_OK) {
            printf("Error writing: %s\n", dds_strretcode(-rc));
        } else if (verbose) {
            printf("Published: Vehicle=%s, Location=(%.8f, %.8f)\n",
                   msg.vehicle_id, msg.latitude, msg.longitude);
        }

        // Move to next point, loop back to start if at end
        current_point = (current_point + 1) % NUM_WAYPOINTS;

        if (current_point + 1 >= NUM_WAYPOINTS || current_point == 0) {
            printf("Reached end of path, resting...\n");
            dds_sleepfor(DDS_SECS(30));
        }


        dds_sleepfor(DDS_SECS(1));
    }

    printf("Cleaning up DDS entities...\n");
    dds_delete(participant);
    return 0;
}