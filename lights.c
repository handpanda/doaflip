#include "config.h"
#include "lights.h"

void set_status(int8 s1, int8 s2) {
    if (s1 >= 0) {
        STATUS1_LAT = s1;
    }
    if (s2 >= 0) {
        STATUS2_LAT = s2;
    }
}

void toggle_lights() {
    static uint8 status = 0;
    
    status ^= 1;
    
    STATUS1_LAT = status & 0b1;
    STATUS2_LAT = !(status & 0b1);
}

void flash_id() {
    static uint8 id_status = 0;
    
    id_status ^= 1;
    
    if (id_status) {
//        set_status(MODULE_ID & 0b01, (MODULE_ID & 0b10) >> 1);   
    } else {
        set_status(0, 0);
    }   
}
