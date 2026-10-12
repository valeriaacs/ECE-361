#include <stdio.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

int main(void) {
    int rc = 0;
    /* Device type D: 2 switches, 3 LEDs */
    uint32_t *base = iom361_initialize(2, 3, &rc);
    if (base == NULL || rc != 0) {
        fprintf(stderr, "Failed to initialize iom361 model (rc = %d)\n", rc);
        return 1;
    }

    printf("Cold chain shipping logger\n");
    return 0;
}
