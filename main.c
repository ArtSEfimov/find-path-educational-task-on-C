#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

void reverse_path(STATION **path, const int *count_st) {
    STATION *tmp = NULL;
    for (int i = 0, j = *count_st - 1; i < j; i++, j--) {
        tmp = path[i];
        path[i] = path[j];
        path[j] = tmp;
    }
}

static STATION *queue[max_path_station];

static void reset_queue(void) {
    for (STATION **p = queue; p < queue + max_path_station; ++p) {
        if (*p) {
            (*p)->fl_reserved = 0;
            (*p)->previous_tag_station = NULL;
        } else {
            break;
        }
    }

    memset(queue, 0, sizeof(queue));
}


void find_path(STATION *from, STATION *to, STATION *path[], int *count_st) {
    reset_queue();

    // init head and tail pointers
    STATION **head = queue;
    STATION **tail = queue + 1;

    // add first element
    *head = from;
    (*head)->fl_reserved = 1;
    (*head)->previous_tag_station = NULL;

    while (*head != *tail) {
        for (int i = 0; i < (*head)->count_links; i++) {
            if ((*head)->links[i]->fl_reserved == 1) {
                continue;
            }
            (*head)->links[i]->fl_reserved = 1;
            (*head)->links[i]->previous_tag_station = *head;
            *tail = (*head)->links[i];
            if ((*head)->links[i]->name == to->name) {
                path[0] = to;
                *count_st = *count_st + 1;
                get_path(to->previous_tag_station, path, count_st);
                reverse_path(path, count_st);
                return;
            }
            tail++;
        }
        head++;
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

    // __ASSERT_TESTS__ // макроопределение для тестирования (не убирать и должно идти непосредственно перед return 0)


    return 0;
}
