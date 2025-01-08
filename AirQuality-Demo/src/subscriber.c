#include "dds/dds.h"
#include "AirQuality.h"
#include "district_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"

static volatile bool running = true;
static dds_entity_t participant;
static dds_entity_t topic;
static dds_entity_t reader;
static void *samples[MAX_SAMPLES];
static dds_sample_info_t infos[MAX_SAMPLES];
static char *filter_district = NULL;

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

static void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h               Show this help message\n");
    printf("  -d district_id   Filter by district ID\n");
}

static const char* get_color_code(float aqi) {
    if (aqi <= AQI_GOOD_MAX) return "\033[32m";  // Green
    if (aqi <= AQI_MODERATE_MAX) return "\033[33m";  // Yellow
    if (aqi <= AQI_UNHEALTHY_SENSITIVE_MAX) return "\033[35m";  // Purple
    if (aqi <= AQI_UNHEALTHY_MAX) return "\033[31m";  // Red
    if (aqi <= AQI_VERY_UNHEALTHY_MAX) return "\033[34m";  // Blue
    return "\033[31;1m";  // Bright Red
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            filter_district = argv[++i];
        }
    }

    signal(SIGINT, signal_handler);

    participant = dds_create_participant(DOMAIN_ID, NULL, NULL);

    dds_qos_t *qos = dds_create_qos();
    //dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(1));
    //dds_qset_durability(qos, DDS_DURABILITY_TRANSIENT_LOCAL);
    //dds_qset_history(qos, DDS_HISTORY_KEEP_LAST, 1);

    topic = dds_create_topic(participant, &AirQuality_AQIData_desc, TOPIC_NAME, qos, NULL);
    reader = dds_create_reader(participant, topic, qos, NULL);

    for (int i = 0; i < MAX_SAMPLES; i++) {
        samples[i] = AirQuality_AQIData__alloc();
    }

    printf("AQI Monitor running. Press Ctrl-C to exit.\n");
    if (filter_district) {
        printf("Filtering for district: %s\n", filter_district);
    }

    printf("\033[2J\033[H");  // Clear screen
    while (running) {
        dds_return_t rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc > 0) {
            printf("\033[H");  // Move cursor to top
            printf("District                  AQI     PM2.5   PM10    Level\n");
            printf("--------------------------------------------------------\n");
            
            for (int i = 0; i < rc; i++) {
                if (infos[i].valid_data) {
                    AirQuality_AQIData *msg = samples[i];
                    if (!filter_district || strcmp(msg->district_id, filter_district) == 0) {
                        const char *color = get_color_code(msg->aqi);
                        printf("%-20s %s%6.1f\033[0m  %6.1f  %6.1f  %-20s\n",
                               msg->name,
                               color, msg->aqi,
                               msg->pm25, msg->pm10,
                               msg->level);
                    }
                }
            }
        }
        dds_sleepfor(DDS_MSECS(100));
    }

    for (int i = 0; i < MAX_SAMPLES; i++) {
        AirQuality_AQIData_free(samples[i], DDS_FREE_ALL);
    }
    
    dds_delete(participant);
    dds_delete_qos(qos);
    return 0;
}