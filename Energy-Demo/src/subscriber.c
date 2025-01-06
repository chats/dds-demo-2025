#include "dds/dds.h"
#include "PowerPlantData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"

static volatile bool running = true;
static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h            Show this help message\n");
    printf("  -t type       Filter by plant type (wind/solar)\n");
    printf("  -s plant_id   Filter by specific plant ID\n");
    printf("  -d domain_id  Set DDS domain ID (default: 0)\n");
    printf("  -n topic      Set topic name (default: PowerPlantTopic)\n");
}

void print_dashboard(PowerPlant_EnergyData *data, size_t count) {
    printf("\033[2J\033[H");
    printf("=== Renewable Energy Dashboard ===\n");
    printf("%-10s %-6s %-15s %-10s %-10s %-8s\n",
           "Plant ID", "Type", "Location", "Power(kW)", "Energy(kWh)", "Temp(°C)");
    printf("----------------------------------------------------------------\n");
    
    double total_power = 0.0;
    double total_energy = 0.0;
    
    for (size_t i = 0; i < count; i++) {
        printf("%-10s %-6s %-15s %10.2f %10.2f %8.1f\n",
               data[i].plant_id,
               data[i].type == PowerPlant_WIND ? "WIND" : "SOLAR",
               data[i].location.name,
               data[i].power_output,
               data[i].daily_energy,
               data[i].temperature);
               
        total_power += data[i].power_output;
        total_energy += data[i].daily_energy;
    }
    
    printf("----------------------------------------------------------------\n");
    printf("Total Power Output: %.2f kW\n", total_power);
    printf("Total Energy Today: %.2f kWh\n", total_energy);
}

int main(int argc, char *argv[]) {
    enum PowerPlant_PlantType filter_type = -1;
    char *filter_plant_id = NULL;
    dds_domainid_t domain_id = DOMAIN_ID;
    const char *topic_name = TOPIC_NAME;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            i++;
            if (strcasecmp(argv[i], "wind") == 0) {
                filter_type = PowerPlant_WIND;
            } else if (strcasecmp(argv[i], "solar") == 0) {
                filter_type = PowerPlant_SOLAR;
            }
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            filter_plant_id = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            domain_id = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            topic_name = argv[++i];
        }
    }

    signal(SIGINT, signal_handler);

    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t reader;
    dds_return_t rc;
    PowerPlant_EnergyData *msg;
    void *samples[MAX_SAMPLES];
    dds_sample_info_t infos[MAX_SAMPLES];

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

    reader = dds_create_reader(participant, topic, NULL, NULL);
    if (reader < 0) {
        printf("Error creating reader: %s\n", dds_strretcode(-reader));
        return 1;
    }

    for (int i = 0; i < MAX_SAMPLES; i++) {
        samples[i] = PowerPlant_EnergyData__alloc();
    }

    PowerPlant_EnergyData dashboard_data[MAX_SAMPLES];
    size_t valid_count = 0;

    printf("Waiting for data...\n");
    if (filter_type != -1) {
        printf("Filtering for %s plants\n", filter_type == PowerPlant_WIND ? "WIND" : "SOLAR");
    }
    
    while (running) {
        rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc > 0) {
            valid_count = 0;
            for (int i = 0; i < rc; i++) {
                if (infos[i].valid_data) {
                    msg = samples[i];
                    bool include = true;
                    
                    if (filter_type != -1 && msg->type != filter_type) {
                        include = false;
                    }
                    if (filter_plant_id && strcmp(msg->plant_id, filter_plant_id) != 0) {
                        include = false;
                    }
                    
                    if (include && valid_count < MAX_SAMPLES) {
                        memcpy(&dashboard_data[valid_count++], msg, sizeof(PowerPlant_EnergyData));
                    }
                }
            }
            if (valid_count > 0) {
                print_dashboard(dashboard_data, valid_count);
            }
        }
        dds_sleepfor(DDS_MSECS(100));
    }

    for (int i = 0; i < MAX_SAMPLES; i++) {
        PowerPlant_EnergyData_free(samples[i], DDS_FREE_ALL);
    }
    
    printf("Cleaning up...\n");
    dds_delete(participant);
    return 0;
}