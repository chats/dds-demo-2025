#include "dds/dds.h"
#include "AirQuality.h"
#include <libwebsockets.h>
#include <jansson.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"

#define WEBSOCKET_PORT 8082

static struct lws_context *context;
static volatile bool running = true;
static dds_entity_t participant;
static dds_entity_t topic;
static dds_entity_t reader;
static void *samples[MAX_SAMPLES] = { NULL };  // Initialize array to NULL
static dds_sample_info_t infos[MAX_SAMPLES];
static bool samples_allocated = false;  // Flag to track allocation

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

static void cleanup_samples(void) {
    if (samples_allocated) {
        for (int i = 0; i < MAX_SAMPLES; i++) {
            if (samples[i]) {
                AirQuality_AQIData_free(samples[i], DDS_FREE_ALL);
                samples[i] = NULL;
            }
        }
        samples_allocated = false;
    }
}

static char* create_json_message(AirQuality_AQIData *msg) {
    json_t *root = json_object();
    
    if (!msg) return NULL;
    
    json_object_set_new(root, "district_id", json_string(msg->district_id));
    json_object_set_new(root, "name", json_string(msg->name));
    json_object_set_new(root, "latitude", json_real(msg->latitude));
    json_object_set_new(root, "longitude", json_real(msg->longitude));
    json_object_set_new(root, "pm25", json_real(msg->pm25));
    json_object_set_new(root, "pm10", json_real(msg->pm10));
    json_object_set_new(root, "aqi", json_real(msg->aqi));
    json_object_set_new(root, "level", json_string(msg->level));
    json_object_set_new(root, "timestamp", json_integer(msg->timestamp));
    
    char *json_str = json_dumps(root, JSON_COMPACT);
    json_decref(root);
    return json_str;
}

static int callback_aqi(struct lws *wsi, enum lws_callback_reasons reason,
                       void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED: {
            if (!samples_allocated) {
                for (int i = 0; i < MAX_SAMPLES; i++) {
                    samples[i] = AirQuality_AQIData__alloc();
                    if (!samples[i]) {
                        printf("Failed to allocate sample %d\n", i);
                        cleanup_samples();
                        return -1;
                    }
                }
                samples_allocated = true;
            }
            lws_callback_on_writable(wsi);
            break;
        }

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            if (!samples_allocated) break;
            
            dds_return_t rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
            if (rc > 0) {
                for (int i = 0; i < rc; i++) {
                    if (infos[i].valid_data) {
                        AirQuality_AQIData *msg = samples[i];
                        char *json_str = create_json_message(msg);
                        if (json_str) {
                            unsigned char buf[LWS_PRE + 2048];
                            size_t json_len = strlen(json_str);
                            memcpy(&buf[LWS_PRE], json_str, json_len);
                            lws_write(wsi, &buf[LWS_PRE], json_len, LWS_WRITE_TEXT);
                            free(json_str);
                        }
                    }
                }
            }
            lws_callback_on_writable(wsi);
            break;
        }

        case LWS_CALLBACK_CLOSED:
            cleanup_samples();
            break;

        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "aqi-protocol",
        callback_aqi,
        0,
        2048,
    },
    { NULL, NULL, 0, 0 }
};

int main(void) {
    signal(SIGINT, signal_handler);

    participant = dds_create_participant(DOMAIN_ID, NULL, NULL);
    if (participant < 0) {
        printf("Error creating participant: %s\n", dds_strretcode(-participant));
        return 1;
    }

    topic = dds_create_topic(participant, &AirQuality_AQIData_desc, TOPIC_NAME, NULL, NULL);
    if (topic < 0) {
        printf("Error creating topic: %s\n", dds_strretcode(-topic));
        return 1;
    }

    reader = dds_create_reader(participant, topic, NULL, NULL);
    if (reader < 0) {
        printf("Error creating reader: %s\n", dds_strretcode(-reader));
        return 1;
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
    printf("Domain ID=%d, Topic Name=%s\n", DOMAIN_ID, TOPIC_NAME);
    printf("Press Ctrl-C to stop\n");

    while (running) {
        lws_service(context, 50);
        usleep(20000);  // 20ms sleep
    }

    cleanup_samples();
    lws_context_destroy(context);
    dds_delete(participant);
    return 0;
}