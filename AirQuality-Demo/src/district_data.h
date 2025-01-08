#ifndef DISTRICT_DATA_H
#define DISTRICT_DATA_H

typedef struct {
    const char *id;
    const char *name;
    double latitude;
    double longitude;
} District;

static const District districts[] = {
    // Bangkok Districts
    {"BKK01", "Phra Nakhon", 13.7520, 100.4916},
    {"BKK02", "Dusit", 13.7741, 100.5092},
    {"BKK03", "Nong Chok", 13.8748, 100.8853},
    {"BKK04", "Bang Rak", 13.7246, 100.5285},
    {"BKK05", "Bang Khen", 13.8729, 100.5957},
    {"BKK06", "Bang Kapi", 13.7692, 100.6412},
    {"BKK07", "Pathum Wan", 13.7446, 100.5331},
    {"BKK08", "Pom Prap Sattru Phai", 13.7533, 100.5097},
    {"BKK09", "Phra Khanong", 13.7054, 100.6019},
    {"BKK10", "Min Buri", 13.8163, 100.7377},
    {"BKK11", "Lat Krabang", 13.7276, 100.7824},
    {"BKK12", "Yan Nawa", 13.6873, 100.5350},
    {"BKK13", "Samphanthawong", 13.7374, 100.5139},
    {"BKK14", "Phaya Thai", 13.7649, 100.5389},
    {"BKK15", "Thon Buri", 13.7200, 100.4800},
    {"BKK16", "Bangkok Yai", 13.7316, 100.4760},
    {"BKK17", "Huai Khwang", 13.7777, 100.5771},
    {"BKK18", "Khlong San", 13.7336, 100.5016},
    {"BKK19", "Taling Chan", 13.7789, 100.4347},
    {"BKK20", "Bangkok Noi", 13.7685, 100.4715},
    {"BKK21", "Bang Khun Thian", 13.6374, 100.4339},
    {"BKK22", "Phasi Charoen", 13.7202, 100.4385},
    {"BKK23", "Nong Khaem", 13.7104, 100.3584},
    {"BKK24", "Rat Burana", 13.6768, 100.5070},
    {"BKK25", "Bang Phlat", 13.7794, 100.4850},
    {"BKK26", "Din Daeng", 13.7690, 100.5576},
    {"BKK27", "Bueng Kum", 13.8163, 100.6550},
    {"BKK28", "Sathon", 13.7199, 100.5277},
    {"BKK29", "Bang Sue", 13.8288, 100.5285},
    {"BKK30", "Chatuchak", 13.8283, 100.5735},
    {"BKK31", "Bang Kho Laem", 13.6945, 100.5090},
    {"BKK32", "Prawet", 13.7160, 100.6969},
    {"BKK33", "Khlong Toei", 13.7145, 100.5847},
    {"BKK34", "Suan Luang", 13.7324, 100.6431},
    {"BKK35", "Chom Thong", 13.6827, 100.4660},
    {"BKK36", "Don Mueang", 13.9279, 100.5874},
    {"BKK37", "Ratchathewi", 13.7509, 100.5316},
    {"BKK38", "Lat Phrao", 13.8276, 100.6130},
    {"BKK39", "Watthana", 13.7373, 100.5714},
    {"BKK40", "Bang Khae", 13.7201, 100.3989},
    {"BKK41", "Lak Si", 13.8837, 100.5682},
    {"BKK42", "Sai Mai", 13.9089, 100.6588},
    {"BKK43", "Khan Na Yao", 13.8370, 100.7016},
    {"BKK44", "Saphan Sung", 13.7767, 100.6922},
    {"BKK45", "Wang Thonglang", 13.7800, 100.6082},
    {"BKK46", "Khlong Sam Wa", 13.8870, 100.7437},
    {"BKK47", "Bang Na", 13.6667, 100.6334},
    {"BKK48", "Thawi Watthana", 13.7776, 100.3750},
    {"BKK49", "Thung Khru", 13.6324, 100.5008},
    {"BKK50", "Bang Bon", 13.6824, 100.3882},

    // Nonthaburi
    {"NTB01", "Muang Nonthaburi", 13.8610, 100.4927},
    {"NTB02", "Pak Kret", 13.9132, 100.4977},
    
    // Pathum Thani
    {"PTT01", "Muang Pathum Thani", 14.0169, 100.5347},
    {"PTT02", "Khlong Luang", 14.0664, 100.6453},
    
    // Samut Prakan
    {"SPK01", "Muang Samut Prakan", 13.5991, 100.5998},
    {"SPK02", "Bang Phli", 13.6068, 100.7320}
};

#define NUM_DISTRICTS (sizeof(districts) / sizeof(districts[0]))

// AQI Level definitions
static const float AQI_GOOD_MAX = 50.0;
static const float AQI_MODERATE_MAX = 100.0;
static const float AQI_UNHEALTHY_SENSITIVE_MAX = 150.0;
static const float AQI_UNHEALTHY_MAX = 200.0;
static const float AQI_VERY_UNHEALTHY_MAX = 300.0;

static inline const char* get_aqi_level(float aqi) {
    if (aqi <= AQI_GOOD_MAX) return "Good";
    if (aqi <= AQI_MODERATE_MAX) return "Moderate";
    if (aqi <= AQI_UNHEALTHY_SENSITIVE_MAX) return "Unhealthy for Sensitive Groups";
    if (aqi <= AQI_UNHEALTHY_MAX) return "Unhealthy";
    if (aqi <= AQI_VERY_UNHEALTHY_MAX) return "Very Unhealthy";
    return "Hazardous";
}

#endif // DISTRICT_DATA_H