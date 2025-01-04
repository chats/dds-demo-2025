#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *program) {
    printf("Usage: %s [options]\n", program);
    printf("Options:\n");
    printf("  -h                Show this help message\n");
    printf("  -s lat,lon        Set start point (default: 13.913762006418807,100.6045283064237)\n");
    printf("  -e lat,lon        Set end point (default: 16.465591672108264,102.78304626323816)\n");
    printf("  -n num_points     Set number of points (default: 10)\n");
}

int main(int argc, char *argv[]) {
    double start_lat = 13.913762006418807;
    double start_lon = 100.6045283064237;
    double end_lat = 16.465591672108264;
    double end_lon = 102.78304626323816;
    int num_points = 10;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_points = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            sscanf(argv[++i], "%lf,%lf", &start_lat, &start_lon);
        }
        else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            sscanf(argv[++i], "%lf,%lf", &end_lat, &end_lon);
        }
    }

    printf("#ifndef GPS_DATA_2_H\n");
    printf("#define GPS_DATA_2_H\n\n");
    printf("typedef struct {\n");
    printf("    double latitude;\n");
    printf("    double longitude;\n");
    printf("} GPSPoint;\n\n");
    printf("static const GPSPoint gps_waypoints[] = {\n");

    for (int i = 0; i < num_points; i++) {
        double t = i / (double)(num_points - 1);
        double lat = start_lat + (end_lat - start_lat) * t;
        double lon = start_lon + (end_lon - start_lon) * t;
        printf("    {%.12f, %.12f},\n", lat, lon);
    }

    printf("};\n\n");
    printf("#define NUM_WAYPOINTS %d\n\n", num_points);
    printf("#endif // GPS_DATA_2_H\n");

    return 0;
}