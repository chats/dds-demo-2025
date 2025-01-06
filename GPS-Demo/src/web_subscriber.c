#include "dds/dds.h"
#include "VehicleGPS.h"
#include <libwebsockets.h>
#include <jansson.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "config.h"

#define WEBSOCKET_PORT 8081

static struct lws_context *context;
static volatile bool running = true;
static dds_entity_t participant;
static dds_entity_t topic;
static dds_entity_t reader;
static void *samples[MAX_SAMPLES];
static dds_sample_info_t infos[MAX_SAMPLES];

static void signal_handler(int sig) {
    if (sig == SIGINT) {
        running = false;
        printf("\nShutting down...\n");
    }
}

static char* create_json_message(VehicleGPS_Location *msg) {
    json_t *root = json_object();
    json_object_set_new(root, "vehicle_id", json_string(msg->vehicle_id));
    json_object_set_new(root, "latitude", json_real(msg->latitude));
    json_object_set_new(root, "longitude", json_real(msg->longitude));
    char *json_str = json_dumps(root, JSON_COMPACT);
    json_decref(root);
    return json_str;
}

static int callback_gps(struct lws *wsi, enum lws_callback_reasons reason,
                       void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            samples[0] = VehicleGPS_Location__alloc();
            lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            dds_return_t rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
            if (rc > 0 && infos[0].valid_data) {
                VehicleGPS_Location *msg = samples[0];
                char *json_str = create_json_message(msg);
                if (json_str) {
                    unsigned char buf[LWS_PRE + 1024];
                    size_t json_len = strlen(json_str);
                    memcpy(&buf[LWS_PRE], json_str, json_len);
                    lws_write(wsi, &buf[LWS_PRE], json_len, LWS_WRITE_TEXT);
                    free(json_str);
                }
            }
            lws_callback_on_writable(wsi);
            break;
        }

        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "gps-protocol",
        callback_gps,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 }
};

int main(void) {
    signal(SIGINT, signal_handler);

    participant = dds_create_participant(DOMAIN_ID, NULL, NULL);
    topic = dds_create_topic(participant, &VehicleGPS_Location_desc, TOPIC_NAME, NULL, NULL);
    reader = dds_create_reader(participant, topic, NULL, NULL);

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
        lws_service(context, 50);
    }

    VehicleGPS_Location_free(samples[0], DDS_FREE_ALL);
    lws_context_destroy(context);
    dds_delete(participant);
    return 0;
}