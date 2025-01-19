#include "dds/dds.h"
#include "SensorData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "config.h"

/*
typedef struct {
    char id[DEVICE_ID_LEN];
    double current_temp;
} Sensor;
*/
static dds_domainid_t domain_id = DOMAIN_ID;
static const char *topic_name = TOPIC_NAME;

static SensorData sensors[MAX_SENSORS];
static size_t sensor_count = 0;
static const double HUMIDITY_FACTOR = 0.7;

static bool verbose = false;
static dds_entity_t participant, topic, writer;
static dds_return_t rc;
static SensorData msg = {0};

static volatile bool running = true;

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nReceived SIGINT, shutting down...\n");
    }
}

static inline double get_next_temperature(const double current_temp) {
    const double change = (((double)rand() / RAND_MAX) * 2 - 1) * MAX_TEMP_CHANGE;
    double new_temp = current_temp + change;
    return new_temp < MIN_TEMP ? MIN_TEMP : (new_temp > MAX_TEMP ? MAX_TEMP : new_temp);
}

static inline double calculate_humidity(const double temperature) {
    return (temperature / HUMIDITY_FACTOR) + (((double)rand() / RAND_MAX) * 4.0 - 2.0);
}

static void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -v            Enable verbose output\n");
    printf("  -s device_id  Set device ID (default: sensor-001)\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: SensorTopic)\n");
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
                if (++i < argc && sensor_count < MAX_SENSORS) {
                    strncpy(sensors[sensor_count].device_id, argv[i], DEVICE_ID_LEN-1);
                    sensors[sensor_count].device_id[DEVICE_ID_LEN-1] = '\0';
                    //sensors[sensor_count++].temperature = 25.0;
                }
                break;
        }
    }
}

int setup() {
    if (!sensor_count) {
        strcpy(sensors[0].device_id, "sensor-001");
        sensors[0].temperature = 25.0;
        sensor_count = 1;
    }

    srand(time(NULL));

    // Create and configure QoS
    dds_qos_t *qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(1));
    dds_qset_durability(qos, DDS_DURABILITY_TRANSIENT_LOCAL);
    dds_qset_history(qos, DDS_HISTORY_KEEP_LAST, 1);

    // Create DDS entities with QoS
    if ((participant = dds_create_participant(domain_id, NULL, NULL)) < 0 ||
        (topic = dds_create_topic(participant, &SensorData_desc, topic_name, qos, NULL)) < 0 ||
        (writer = dds_create_writer(participant, topic, qos, NULL)) < 0) {
        fprintf(stderr, "Error initializing DDS: %s\n", dds_strretcode(-writer));
        dds_delete_qos(qos);
        return 1;
    }

    dds_delete_qos(qos);
    return 0;
}

int main(int argc, char *argv[]) {
    
    config(argc, argv);
    
    signal(SIGINT, signal_handler);

    if (setup() != 0) {
        return 1;
    }

    printf("Publisher running, Domain: %d, Topic=%s, press Ctrl-C to stop...\n", domain_id, topic_name);

    dds_publication_matched_status_t status;
    while (running) {
        if (dds_get_publication_matched_status(writer, &status) == DDS_RETCODE_OK) {
            if (status.current_count > 0) {
                for (size_t i = 0; i < sensor_count; i++) {
                    sensors[i].temperature = get_next_temperature(sensors[i].temperature);
                    strcpy(msg.device_id, sensors[i].device_id);
                    msg.temperature = sensors[i].temperature;
                    msg.humidity = calculate_humidity(sensors[i].temperature);
                    msg.timestamp = time(NULL);

                    if ((rc = dds_write(writer, &msg)) != DDS_RETCODE_OK) {
                        fprintf(stderr, "Error: %s\n", dds_strretcode(-rc));
                    } else if (verbose) {
                        printf("Published: %s, T=%.1f, H=%.1f\n",
                            msg.device_id, msg.temperature, msg.humidity);
                    }
                }
            }
        }
        dds_sleepfor(DDS_SECS(1));
    }

    printf("Cleaning up DDS entities...\n");
    dds_delete(participant);
    return 0;
}