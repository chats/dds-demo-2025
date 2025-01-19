#include "dds/dds.h"
#include "PowerPlantData.h"
#include "plant_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"

static volatile bool running = true;
static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

double simulate_power_output(const PlantInfo *plant, time_t current_time) {
    struct tm *time_info = localtime(&current_time);
    int hour = time_info->tm_hour;
    double base_output = 0.0;
    
    if (plant->type == PowerPlant_SOLAR) {
        if (hour >= 6 && hour <= 18) {
            double solar_factor = sin(M_PI * (hour - 6) / 12);
            base_output = plant->capacity * solar_factor;
        }
        base_output *= (0.9 + (rand() % 200) / 1000.0);
    } else {
        base_output = plant->capacity * (0.3 + (rand() % 700) / 1000.0);
    }
    
    return base_output;
}

double calculate_daily_energy(double power_output, int hour) {
    return power_output * hour;
}

void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -v            Enable verbose output\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -t topic      Set topic name (default: PowerPlantTopic)\n");
}

int main(int argc, char *argv[]) {
    bool verbose = false;
    dds_domainid_t domain_id = DOMAIN_ID;
    const char *topic_name = TOPIC_NAME;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            domain_id = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            topic_name = argv[++i];
        }
    }

    signal(SIGINT, signal_handler);
    srand(time(NULL));

    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t writer;
    dds_return_t rc;
    PowerPlant_EnergyData msg;

    participant = dds_create_participant(domain_id, NULL, NULL);
    if (participant < 0) {
        printf("Error creating participant: %s\n", dds_strretcode(-participant));
        return 1;
    }

    topic = dds_create_topic(participant, &PowerPlant_EnergyData_desc, 
                           topic_name, NULL, NULL);
    if (topic < 0) {
        printf("Error creating topic: %s\n", dds_strretcode(-topic));
        return 1;
    }

    writer = dds_create_writer(participant, topic, NULL, NULL);
    if (writer < 0) {
        printf("Error creating writer: %s\n", dds_strretcode(-writer));
        return 1;
    }

    printf("Publishing data for %zu plants...\n", NUM_PLANTS);
    
    while (running) {
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        
        for (size_t i = 0; i < NUM_PLANTS; i++) {
            //msg.plant_id = power_plants[i].plant_id;
            strncpy(msg.plant_id, power_plants[i].plant_id, strlen(power_plants[i].plant_id));
            msg.plant_id[strlen(power_plants[i].plant_id)] = '\0'; // Ensure null termination

            msg.type = power_plants[i].type;
            //msg.location.name = power_plants[i].location;
            strncpy(msg.location.name, power_plants[i].location, strlen(power_plants[i].location));
            msg.location.name[strlen(power_plants[i].location)] = '\0'; // Ensure null termination
            msg.location.latitude = power_plants[i].latitude;
            msg.location.longitude = power_plants[i].longitude;
            msg.power_output = simulate_power_output(&power_plants[i], current_time);
            msg.daily_energy = calculate_daily_energy(msg.power_output, time_info->tm_hour);
            msg.temperature = 25.0 + (rand() % 200) / 10.0;
            msg.timestamp = current_time;

            rc = dds_write(writer, &msg);
            if (rc != DDS_RETCODE_OK) {
                printf("Error writing: %s\n", dds_strretcode(-rc));
            } else if (verbose) {
                printf("Published: Plant=%s, Type=%s, Power=%.2f kW, Energy=%.2f kWh\n",
                       msg.plant_id, 
                       msg.type == PowerPlant_WIND ? "WIND" : "SOLAR",
                       msg.power_output, 
                       msg.daily_energy);
            }
        }
        
        dds_sleepfor(DDS_SECS(10));
    }

    printf("Cleaning up...\n");
    dds_delete(participant);
    return 0;
}