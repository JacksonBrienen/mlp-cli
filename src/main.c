#include <mlp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>

typedef enum {
    NONE = 0,
    MLP = 1,
    CSV = 2
} MODE;

char *modes[] = {
    "none",
    "csv",
    "mlp"
};

typedef struct {
    const char *command;
    void(*action)(char*);
} command_t;

char *none_commands[] = {"mlp", "csv", "cd", "pwd", "ls", "exit", NULL};
char *mlp_commands[] = {"cd", "pwd", "ls", "exit", "new", "save", "read", "train", "predict", NULL};
char *csv_commands[] = {"cd", "pwd", "ls", "exit", "read", "compile", NULL};

char **commands[3] = {
    none_commands,
    mlp_commands,
    csv_commands
};

// thx JRL https://stackoverflow.com/a/1431206
char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s)); 
}

char *prompt(MODE mode) {
    static const char *sep = "$ ";

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    char *current_dir = cwd + strlen(cwd) - 1;
    while((*current_dir) != '/' && current_dir > cwd) current_dir--;
    current_dir++;
    char * prompt;
    switch(mode) {
        case NONE:
            prompt = smalloc(strlen(current_dir) + 3);
            sprintf(prompt, "[%s]%s", current_dir, sep);
        break;

        case MLP:

        break;

        case CSV:

        break;
    }
    return prompt;
}
MODE mode = NONE;

char *command_completion(const char *text, int state) {
    static int cmd_index, cmd_len;
    static 
    char *cmd;

    if(state == 0) {
        cmd_index = 0;
        cmd_len = strlen(text);
    }
    while ((cmd = commands[mode][cmd_index++])) {
        if(strncmp(cmd, text, cmd_len) == 0) {
            return strdup(cmd);
        }
    }

    return NULL;
}

char **completion(const char *init, int start, int end) {
    rl_attempted_completion_over = 1;
    char *buf = strdup(rl_line_buffer);
    if (!strlen(trim(buf)) || start == 0) {
        return rl_completion_matches(init, command_completion);
    }
    return rl_completion_matches(init, rl_filename_completion_function);
}

void perform(char *line) {
    char *command;
    int i = 0;
    while(!whitespace(line[i])) i++;
    line[i] = 0;
    command = line;
    line = trim(line + i + 1);
    printf("Command is '%s' on '%s'\n", command, line);
}

int main() {
    rl_attempted_completion_function = completion;
    // rl_completion_entry_function = completion_function;
    rl_bind_key('\t', rl_complete);

    using_history();

    // printf("%s", prompt(mode));

    int running = 1;

    char *line;
    char *prmt = prompt(mode);
    MODE last_mode = mode;
    while(running) {
        if(mode != last_mode) {
            free(prmt);
            prmt = prompt(mode);
        }
        line = readline(prmt);
        char *linestart = line;
        if(!line) {
            printf("STDIN closed by user\n");
            break;
        }
        line = trim(line);
        perform(line);
        if(strlen(line) > 0) {
            add_history(line);
        }
        free(linestart);
    }
    return 0;

}


// int main() {
//     srandf(clock());

//     size_t num_layers = 3;
//     size_t layers[3] = {2, 5, 1};
//     activation_t activations[] = {sig, sig};

//     mlp_t *m = mlp_init(num_layers, layers, activations);

//     int num_inputs = 1000;
//     vector_t * inputs[num_inputs];
//     vector_t * targets[num_inputs];
//     for(int i = 0; i < num_inputs; i++) {
//         float f1 = (float)rand() / (RAND_MAX * 2L);
//         float f2 = (float)rand() / (RAND_MAX * 2L);
//         inputs[i] = vector(2, (float[2]){f1, f2});
//         targets[i] = vector(1, (float[1]){f1 + f2});
//     }

//     dataset_t dataset = {num_inputs, inputs, targets};
    
//     train(m, &dataset, 50000, 0.1f);

//     printf("Testing with %f + %f: ", 0.3, 0.4);
//     vector_t *in = vector(2, (float[2]){0.3, 0.4});
//     vector_t *result = forward(m, in, 0);
//     printf("%f\n", result->vector[0]);

//     $vector(in);
//     $vector(result);
//     for(int i = 0; i < num_inputs; i++) {
//         $vector(inputs[i]);
//         $vector(targets[i]);
//     }
    
//     mlp_free(m);

//     return 0;
// }