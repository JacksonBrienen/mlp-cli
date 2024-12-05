#include "csv.h"
#include <unistd.h>
#include <string.h>
#include "trim.h"

typedef struct {
    size_t cols;
    char **entries;
} row_t;

void parse_row(char *row, row_t *data) {
    size_t col_count = 0;
    size_t cap = 8;
    char **cols = smalloc(sizeof(char *) * cap);
    cols[col_count++] = strtok(row, ",");

    char *col;
    while((col = strtok(NULL, ",")) != NULL) {
        if(!(col_count < cap)) {
            cap *= 1.5;
            cols = srealloc(cols, sizeof(char *) * cap);
        }
        cols[col_count++] = col;
    }

    for(int i = 0; i < col_count; i++) {
        cols[i] = trim(cols[i]);
    } 
    data->cols = col_count;
    data->entries = cols;
}

int parsecsv(char *fname, dataset_t *dt) {
    FILE *fp = fopen(fname, "r");
    if(fp == NULL) {
        return 1;
    }
    
    char buffer[1024] = {0};
    if(!fgets(buffer, sizeof(buffer), fp)) {
        return 1;
    }

    row_t row;
    parse_row(buffer, &row);
    size_t cols = row.cols;

    size_t num_inputs = 0;
    size_t num_targets = 0;

    int line = 1;

    for(int i = 0; i < cols; i++) {
        if(!strcasecmp(row.entries[i], "input")) {
            if(num_targets != 0) {
                fclose(fp);
                free(row.entries);
                return 1; // error inputs and targets out of order
            }
            num_inputs++;
        } else if(!strcasecmp(row.entries[i], "target")) {
            num_targets++;
        } else {
            fclose(fp);
            free(row.entries);
            return 1; // invalid header name
        }
    }
    free(row.entries);

    size_t cap = 8;
    dt->len = 0;
    dt->inputs = smalloc(sizeof(vector_t *) * cap);
    dt->targets = smalloc(sizeof(vector_t *) * cap);

    while(fgets(buffer, sizeof(buffer), fp)) {
        if(buffer[0] == '\0') continue;
        parse_row(buffer, &row);
        
        if(row.cols != cols) {
            fclose(fp);
            free(row.entries);
            return 1; // error uneven column count
        }

        float in[num_inputs];
        for(int i = 0; i < num_inputs; i++) {
            char *end;
            float f = strtof(row.entries[i], &end);
            if(row.entries[i] == end || *end != '\0') {
                fclose(fp);
                free(row.entries);
                return 1; // float parsing error
            }
            in[i] = f;
        }

        float t[num_targets];
        for(int i = 0; i < num_targets; i++) {
            char *end;
            float f = strtof(row.entries[num_inputs + i], &end);
            if(row.entries[num_inputs + i] == end || *end != '\0') {
                fclose(fp);
                free(row.entries);
                return 1; // float parsing error
            }
            t[i] = f;
        }

        vector_t *inputs = vector(num_inputs, in);
        vector_t *targets = vector(num_targets, t);

        if(!(dt->len < cap)) {
            cap *= 1.5;
            dt->inputs = srealloc(dt->inputs, sizeof(vector_t *) * cap);
            dt->targets = srealloc(dt->targets, sizeof(vector_t *) * cap);
        }
        dt->inputs[dt->len] = inputs;
        dt->targets[dt->len] = targets;
        dt->len++;
        free(row.entries);
    }
    fclose(fp);
    return 0;
}