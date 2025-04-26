#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { name_length = 50, total_links = 50, max_path_station = 100 };

typedef struct tag_station {
    char name[name_length]; // название станции метро
    struct tag_station *links[total_links]; // связи станции метро с другими соседними станциями
    short count_links; // общее количество связей
    char fl_reserved;
    struct tag_station *previous_tag_station;
    // зарезервированная переменная (полезна при реализации алгоритма поиска маршрута)
} STATION;

void set_station_links(STATION *st, int count_links, ...) {
    va_list inner_links;
    va_start(inner_links, count_links);
    st->count_links = (short) count_links;
    for (int i = 0; i < count_links; i++) {
        STATION *next_station = va_arg(inner_links, STATION *);
        st->links[i] = next_station;
    }
    va_end(inner_links);
}

void get_path(STATION *previous_station, STATION *path[], int *count_st) {
    path[*count_st] = previous_station;
    *count_st = *count_st + 1;
    while (previous_station->previous_tag_station != NULL) {
        previous_station = previous_station->previous_tag_station;
        path[*count_st] = previous_station;
        *count_st = *count_st + 1;
    }
}

void find_path(STATION *from, STATION *to, STATION *path[], int *count_st) {
    static STATION queue[max_path_station];
    queue[0] = *from;
    const STATION *queue_input = queue;
    STATION *queue_output = queue;
    queue_output->fl_reserved = 1;
    queue_output->previous_tag_station = NULL;
    queue_input++;

    while (queue_input != queue_output) {
        for (int i = 0; i < queue_output->count_links; i++) {
            STATION next_station = *queue_output->links[i];
            next_station.previous_tag_station = queue_output;
            next_station.fl_reserved = 1;
            if (next_station.name == to->name) {
                path[0] = to;
                *count_st++;
                get_path(to->previous_tag_station, path, count_st);
                return;
            }
            queue_input = &next_station;
            queue_input++;
        }
        queue_output++;
    }
}

int main(void) {
    STATION st[10] = {
        {"St #1", .count_links = 0, .fl_reserved = 0},
        {"St #2", .count_links = 0, .fl_reserved = 0},
        {"St #3", .count_links = 0, .fl_reserved = 0},
        {"St #4", .count_links = 0, .fl_reserved = 0},
        {"St #5", .count_links = 0, .fl_reserved = 0},
        {"St #6", .count_links = 0, .fl_reserved = 0},
        {"St #7", .count_links = 0, .fl_reserved = 0},
        {"St #8", .count_links = 0, .fl_reserved = 0},
        {"St #9", .count_links = 0, .fl_reserved = 0},
        {"St #10", .count_links = 0, .fl_reserved = 0},
    };

    // здесь описывайте связи между станциями метро
    set_station_links(&st[0], 2, &st[1], &st[2]);
    set_station_links(&st[1], 3, &st[0], &st[3], &st[4]);
    set_station_links(&st[2], 2, &st[0], &st[5]);
    set_station_links(&st[3], 2, &st[1], &st[5]);
    set_station_links(&st[4], 2, &st[1], &st[7]);
    set_station_links(&st[5], 4, &st[2], &st[3], &st[6], &st[8]);
    set_station_links(&st[6], 2, &st[5], &st[8]);
    set_station_links(&st[7], 2, &st[4], &st[8]);
    set_station_links(&st[8], 4, &st[5], &st[6], &st[7], &st[9]);
    set_station_links(&st[9], 1, &st[8]);

    STATION *path[max_path_station] = {0};
    int count_st = 0;
    find_path(&st[0], &st[6], path, &count_st);
    for (int i = 0; i < count_st; i++) {
        printf("%s\n", path[i]->name);
    }

    // __ASSERT_TESTS__ // макроопределение для тестирования (не убирать и должно идти непосредственно перед return 0)
    return 0;
}
