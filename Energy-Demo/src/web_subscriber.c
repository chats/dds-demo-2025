#include "dds/dds.h"
#include "PowerPlantData.h"
#include <libwebsockets.h>
#include <jansson.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"

#define WEBSOCKET_PORT 8080

static struct lws_context *context;
static volatile bool running = true;
static PowerPlant_EnergyData dashboard_data[MAX_SAMPLES];
static size_t valid_count = 0;

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

static char* create_json_array(PowerPlant_EnergyData *data, size_t count) {
    json_t *array = json_array();
    
    for (size_t i = 0; i < count; i++) {
        json_t *plant = json_object();
        json_object_set_new(plant, "plant_id", json_string(data[i].plant_id));
        json_object_set_new(plant, "type", 
            json_string(data[i].type == PowerPlant_WIND ? "WIND" : "SOLAR"));
        json_object_set_new(plant, "location", json_string(data[i].location.name));
        json_object_set_new(plant, "power", json_real(data[i].power_output));
        json_object_set_new(plant, "energy", json_real(data[i].daily_energy));
        json_array_append_new(array, plant);
    }
    
    char *json_str = json_dumps(array, JSON_COMPACT);
    json_decref(array);
    return json_str;
}

static int callback_energy_data(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_SERVER_WRITEABLE:
            if (valid_count > 0) {
                char *json_str = create_json_array(dashboard_data, valid_count);
                if (json_str) {
                    unsigned char buf[LWS_PRE + 16384];  // Increased buffer size
                    size_t json_len = strlen(json_str);
                    memcpy(&buf[LWS_PRE], json_str, json_len);
                    lws_write(wsi, &buf[LWS_PRE], json_len, LWS_WRITE_TEXT);
                    free(json_str);
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "energy-data",
        callback_energy_data,
        0,
        16384,  // Increased rx buffer size
    },
    { NULL, NULL, 0, 0 }
};

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    
    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t reader;
    void *samples[MAX_SAMPLES];
    dds_sample_info_t infos[MAX_SAMPLES];
    PowerPlant_EnergyData *msg;
    
    participant = dds_create_participant(DOMAIN_ID, NULL, NULL);
    topic = dds_create_topic(participant, &PowerPlant_EnergyData_desc, 
                           TOPIC_NAME, NULL, NULL);
    reader = dds_create_reader(participant, topic, NULL, NULL);

    for (int i = 0; i < MAX_SAMPLES; i++) {
        samples[i] = PowerPlant_EnergyData__alloc();
    }

    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    info.port = WEBSOCKET_PORT;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    context = lws_create_context(&info);
    if (!context) {
        printf("WebSocket context creation failed\n");
        return 1;
    }

    printf("WebSocket server started on port %d\n", WEBSOCKET_PORT);

    while (running) {
        dds_return_t rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc > 0) {
            valid_count = 0;
            for (int i = 0; i < rc && valid_count < MAX_SAMPLES; i++) {
                if (infos[i].valid_data) {
                    msg = samples[i];
                    memcpy(&dashboard_data[valid_count++], msg, 
                           sizeof(PowerPlant_EnergyData));
                }
            }
            lws_callback_on_writable_all_protocol(context, &protocols[0]);
        }
        
        lws_service(context, 50);
        dds_sleepfor(DDS_MSECS(100));
    }

    // Cleanup
    for (int i = 0; i < MAX_SAMPLES; i++) {
        PowerPlant_EnergyData_free(samples[i], DDS_FREE_ALL);
    }
    
    lws_context_destroy(context);
    dds_delete(participant);
    return 0;
}