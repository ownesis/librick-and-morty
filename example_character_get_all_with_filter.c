#include <stdio.h>
#include <stdlib.h>
#include "character.h"

int main(void) {
    rm_character_t filter = {
        .status = "dead"
    };

    rm_character_arr_t *entity = rm_character_get_all(&filter);

    if (!entity) {
        puts("entity is null");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < entity->len; i++) {
        printf("\n[...................... #%lu .......................]\n", 
                entity->da_character[i]->id);
        printf("Image:................%s\n", entity->da_character[i]->image);
        printf("Person:...............%s...[%s]...(%s)\n", 
                entity->da_character[i]->name,
                entity->da_character[i]->gender,
                entity->da_character[i]->status);

        printf("Race:.................%s %s\n", 
                entity->da_character[i]->species,
                entity->da_character[i]->type);

        printf("Origin:...............%s\n", entity->da_character[i]->origin.name);
        printf("Last know location:...%s\n\n", entity->da_character[i]->location.name);
    }

    rm_character_da_clear(entity);

    return 0;
}
