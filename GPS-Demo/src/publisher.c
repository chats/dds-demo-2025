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

static double start_lat = 13.913762006418807;
static double start_lon = 100.6045283064237;
static double end_lat = 16.465591672108264;
static double end_lon = 102.78304626323816;
static int num_points = 80;

static bool verbose = false;
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
    printf("  -v            Enable verbose output\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: SensorTopic)\n");
    printf("  -i vehicle_id Set vehicle ID (default: vehicle-001)\n");
    printf("  -s lat,lon        Set start point (default: 13.913762006418807,100.6045283064237)\n");
    printf("  -e lat,lon        Set end point (default: 16.465591672108264,102.78304626323816)\n");
    printf("  -n num_points     Set number of points (default: 10)\n");
}

void config(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        switch (argv[i][1]) {
            case 'h': print_usage(argv[0]); exit(0);
            case 'v': verbose = true; break;
            case 'd': 
                if (i < argc ) { domain_id = atoi(argv[++i]);  }
                break;
            case 't': 
                if (i < argc ) { topic_name = argv[++i]; }
                break;      
            case 'i': 
                if (i < argc ) { vehicle_id = argv[++i]; }
                break;
            case 's': 
                if (i < argc ) { sscanf(argv[++i], "%lf,%lf", &start_lat, &start_lon); }
                break;              
            case 'e': 
                if (i < argc ) { sscanf(argv[++i], "%lf,%lf", &end_lat, &end_lon); }
                break;      
            case 'n': 
                if (i < argc ) { num_points = atoi(argv[++i]); }
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

GPSPoint* waypoint_generator() {
    GPSPoint *points = (GPSPoint *)malloc(num_points * sizeof(GPSPoint));

    for (int i = 0; i < num_points; i++) {
        double t = i / (double)(num_points - 1);
        points[i].latitude = start_lat + (end_lat - start_lat) * t;
        points[i].longitude = start_lon + (end_lon - start_lon) * t;
    }

    return points;
}


int main(int argc, char *argv[]) {
    
    config(argc, argv);
    
    signal(SIGINT, signal_handler);

    if (setup() != 0) {
        return 1;
    }

    GPSPoint *waypoints = waypoint_generator();

    printf("Publisher running, Domain: %d, Topic=%s, press Ctrl-C to stop...\n", domain_id, topic_name);

    size_t current_point = 0;
    bool return_to_start = false;

    while (running) {
        msg.vehicle_id = vehicle_id;
        //msg.latitude = gps_waypoints[current_point].latitude;
        msg.latitude = waypoints[current_point].latitude;
        //msg.longitude = gps_waypoints[current_point].longitude;
        msg.longitude = waypoints[current_point].longitude;
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
        //current_point = (current_point + 1) % NUM_WAYPOINTS;
        if (current_point + 1 >= num_points) {
            return_to_start = true;
            printf("Reached end of path, resting...\n");
            dds_sleepfor(DDS_SECS(10));
        }
        if (current_point == 0) {
            printf("Reached start of path, resting...\n");
            dds_sleepfor(DDS_SECS(10));
            return_to_start = false;
        }

        if (return_to_start) {
            current_point--;
        } else {
            current_point++;
        }
        

        //if (current_point + 1 >= NUM_WAYPOINTS || current_point == 0) {
        //    printf("Reached end of path, resting...\n");
        //    dds_sleepfor(DDS_SECS(30));
        //}


        dds_sleepfor(DDS_SECS(2));
    }

    printf("Cleaning up DDS entities...\n");
    free(waypoints);  // Free memory allocated for waypoints
    dds_delete(participant);
    return 0;
}