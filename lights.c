#include "config.h"
#include "lights.h"

void set_status(int8 s1, int8 s2, int8 s3) {
    if (s1 >= 0) {
        STATUS_I_LAT = s1;
    }
    if (s2 >= 0) {
        STATUS_J_LAT = s2;
    }    
    if (s3 >= 0) {
        STATUS_K_LAT = s3;
    }
}

void toggle_lights() {
    static uint8 status = 0;
    
    status ^= 1;
    
    STATUS_I_LAT = status & 0b1;
    STATUS_J_LAT = !(status & 0b1);
}

void flash_id() {
    static uint8 id_status = 0;
    
    id_status ^= 1;
    
    if (id_status) {
//        set_status(MODULE_ID & 0b01, (MODULE_ID & 0b10) >> 1);   
    } else {
        set_status(0, 0, 0);
    }   
}

