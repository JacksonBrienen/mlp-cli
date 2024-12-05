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

void __cd(char **args) {
    if(args[1] == NULL) {
        printf("cd error: expected argument\n");
    } else if(args[2] != NULL) {
        printf("cd error: too many arguments\n");
    } else if(!strcmp(args[1], "?")) {
        printf("usage: \n\tcd <directory>\n\tChanges the current directory to <directory>\n");
    } else if(chdir(args[1]) != 0) {
        printf("cd error: '%s' is not a directory\n", args[1]);
    }
}

void __ls(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tls\n\tLists the files in the current directory\n\n\tls <directory>\n\tLists the files in <directory>\n");
    } else if(args[1] != NULL ) {
        char buf[4 + strlen(args[1])];
        sprintf(buf, "%s %s", "ls", args[1]);
        system(buf);
    } else if(args[2] != NULL) {
        printf("ls error: too many arguments\n");
    } else {
        system("ls");
    }
}

void __pwd(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tpwd\n\tPrints the current working directory\n");
    } else if(args[1] != NULL ) {
        printf("usage error: too many arguments");
    } else {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
}

typedef void(*cmd_action_t)(char**);

typedef struct {
    char *command;
    void(*action)(char**);
} command_t;

command_t none_commands[] = {
    (command_t){"mlp", NULL}, 
    (command_t){"csv", NULL},
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd}, 
    (command_t){"ls", __ls}, 
    (command_t){"exit", NULL}, 
    NULL
};
command_t mlp_commands[] = {
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd}, 
    (command_t){"ls", __ls},
    (command_t){"exit", NULL},
    (command_t){"new", NULL}, 
    (command_t){"save", NULL}, 
    (command_t){"read", NULL}, 
    (command_t){"train", NULL}, 
    (command_t){"predict", NULL}, 
    NULL
};
command_t csv_commands[] = {
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd},
    (command_t){"ls", __ls},
    (command_t){"exit", NULL}, 
    (command_t){"read", NULL}, 
    (command_t){"compile", NULL},
    NULL
};

command_t *commands[3] = {
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

cmd_action_t is_command(char *cmd) {
    command_t* cmds = commands[mode];
    for(int i = 0; cmds[i].command != NULL; i++) {
        if(!strcmp(cmd, cmds[i].command)) {
            return cmds[i].action;
        }
    }
    return NULL;
}

char *command_completion(const char *text, int state) {
    static int cmd_index, cmd_len;
    static char *cmd;

    if(state == 0) {
        cmd_index = 0;
        cmd_len = strlen(text);
    }
    while ((cmd = commands[mode][cmd_index++].command)) {
        if(strncmp(cmd, text, cmd_len) == 0) {
            return strdup(cmd);
        }
    }

    return NULL;
}

char **completion(const char *init, int start, int end) {
    char *buf = strdup(rl_line_buffer);
    if (!strlen(trim(buf)) || start == 0) {
        rl_attempted_completion_over = 1;
        return rl_completion_matches(init, command_completion);
    }
    free(buf);

    // char *buf = strdup(rl_line_buffer);
    // char *cmd = ltrim(buf);
    // {
    //     int i = 0;
    //     while(!whitespace(cmd)) i++;
    //     cmd[i] = '\0';
    // }
    // if(is_command(cmd)) {
    //     free(buf);
    //     char *tmp = strdup(rl_line_buffer);
    //     buf = ltrim(buf);
    //     int pos = 0;
    //     int flag = 0;
    //     for(int i = 0; i < end; i++) {
    //         if(whitespace(buf[i]) && !flag) {
    //             pos++;
    //             flag = 1;
    //         } else if(!whitespace(buf[i]) && flag) {
    //             flag = 0;
    //         }
    //     }
    //     int cap = 8;
    //     char **arr = szalloc(sizeof(char *) * 8);
    //     strtok(arr, " \t");
    //     for(int i = 1; (buf = strtok(NULL, " \t")); i++) {
    //         if(i + 1 >= cap) {
    //             cap *= 1.5;
    //             char *narr = szalloc(sizeof(char *) * cap);
    //             memcpy(narr, arr, sizeof(char *) * 8);
    //             free(arr);
    //             arr = narr;
    //         }
    //         arr[i] = buf;
    //     }

    //     // do stf here

    //     free(tmp);
    //     free(arr);
    // 
    // }
    rl_filename_quoting_desired = 1;
    return NULL;
}

#define __parse_add() do {\
    start[i] = '\0';\
    if(pos + 1 >= cap) {\
        size_t oldcap = cap;\
        cap *= 1.5;\
        char **narr = szalloc(sizeof(char *) * cap);\
        memcpy(narr, arr, sizeof(char *) * oldcap);\
        free(arr);\
        arr = narr;\
    }\
    arr[pos++] = start;\
    start = ltrim(start + i + 1);\
    i = -1;\
} while(0)

char **parse(char *line) {
    line = trim(line);
    size_t cap = 8;
    char **arr = szalloc(sizeof(char *) * sizeof(cap));
    
    char *start = line;

    int pos = 0;
    int quote_flag = 0;

    for(int i = 0; start[i] != '\0'; i++) {
        if((start[i] == '\'' || start[i] == '"') && i == 0) {
            quote_flag = start[i] ;
            start++;
            i--;
        } else if(start[i] == quote_flag) {
            __parse_add();
            quote_flag = 0;
        }
        else if(whitespace(start[i]) && !quote_flag) {
            __parse_add();
        }
    }

    if(quote_flag || start[0] != '\0') {
        if(pos + 1 >= cap) {
            size_t oldcap = cap;
            cap += 1;
            char **narr = szalloc(sizeof(char *) * cap);
            memcpy(narr, arr, sizeof(char *) * oldcap);
            free(arr);
            arr = narr;
        }
        arr[pos++] = start;
    }

    return arr;
}

char *quoting(char *text, int m_t, char *qp) {
    if(rl_completion_quote_character != '\0') {
        return strdup(text);
    }

    size_t len = strlen(text);
    int ws_flag = 0;
    for(int i = 0; i < len; i++) {
        if(whitespace(text[i])) {
            ws_flag = 1;
            break;
        }
    }

    if(!ws_flag) {
        return strdup(text);
    }

    char *quoted = szalloc(len + 3);
    strcpy(quoted + 1, text);
    quoted[0] = '\'';
    quoted[len + 1] = '\'';
    return quoted;
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
    rl_completer_quote_characters = "'\"";
    rl_filename_quote_characters = " `'=[]{}()<>|&\\\t";
    rl_filename_quoting_function = quoting;
    rl_attempted_completion_function = completion;
    rl_bind_key('\t', rl_complete);
    
    using_history();

    // printf("%s", prompt(mode));

    int running = 1;

    char *line;
    char *prmt = prompt(mode);
    while(running) {
        line = readline(prmt);
        char *linestart = line;
        if(!line) {
            printf("STDIN closed by user\n");
            break;
        }
        char **arr = parse(line);
        if(arr[0] != NULL) {
            cmd_action_t action;
            if((action = is_command(arr[0]))) {
                action(arr);
            } else {
                printf("error: unrecoginized command '%s'\n", arr[0]);
            }
        }
        free(arr);
        // line = trim(line);
        // perform(line);
        if(strlen(line) > 0) {
            add_history(line);
        }
        free(linestart);
        free(prmt);
        prmt = prompt(mode);
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