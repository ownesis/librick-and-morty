#include <stdio.h>
#include <stdlib.h>
#include "episode.h"

int main(void) {

    uint64_t id_array[3] = {1, 8, 42};

    rm_episode_arr_t *entity = rm_episode_get_list(id_array, 3);

    if (!entity) {
        puts("entity is null");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < entity->len; i++) {
        printf("\n\n[ ~~~~~~~~~~~~~~~~~~~  #%lu ~~~~~~~~~~~~~~~~~~ ]\n", 
                entity->da_episode[i]->id);
        printf("Episode Name: %s\n", entity->da_episode[i]->name);
        printf("Episode Code: %s\n", entity->da_episode[i]->episode);
        printf("Episode Date: %s\n", entity->da_episode[i]->air_date);
        printf("Characters in episode : %ld\n", entity->da_episode[i]->characters_len);

        for (size_t x = 0; x < entity->da_episode[i]->characters_len; x++)
            printf(" - %s\n", entity->da_episode[i]->characters[x]);
    }

    rm_episode_da_clear(entity);

    return 0;
}
