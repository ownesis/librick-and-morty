#include <stdio.h>
#include <stdlib.h>
#include "location.h"

int main(void) {
    rm_info_t info = {0};

    rm_location_arr_t *entity = rm_location_get_page(&info, 6, NULL); /* NULL is no filter */

    if (!entity) {
        puts("entity is null");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < entity->len; i++) {
        printf("\n[.-================================== (%lu) =================================-.]\n", 
                entity->da_location[i]->id);
        
        printf("\"%s\" | %s\n", 
                entity->da_location[i]->name,
                entity->da_location[i]->type);
        printf(" - Dimension: %s\n", entity->da_location[i]->dimension);
        printf(" - Numbers of residents: %lu\n", entity->da_location[i]->residents_len);
    }

    rm_location_da_clear(entity);
    rm_all_info_clear(&info);

    return 0;
}
