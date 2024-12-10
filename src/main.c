#include <mlp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include "trim.h"
#include "csv.h"


typedef enum {
    NONE = 0,
    MLP = 1,
    CSV = 2
} MODE;

// globals
MODE mode = NONE;
char *csv_file;
mlp_t* active_mlp = NULL;
char *mlp_file;
dataset_t active_dataset;


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
        printf("pwd error: too many arguments\n");
    } else {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
}

void __csv(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tcsv\n\tEnters CSV Mode\n");
    } else if(args[1] != NULL ) {
        printf("csv error: too many arguments\n");
    } else {
        mode = CSV;
    }
}

void __csv_exit(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\texit\n\tExits CSV Mode\n");
    } else if(args[1] != NULL ) {
        printf("exit error: too many arguments\n");
    } else {
        mode = NONE;
    }
}

void __exit(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\texit\n\tExits with code 0\n");
    } else if(args[1] != NULL ) {
        printf("exit error: too many arguments\n");
    } else {
        exit(0);
    }
}

void __csv_read(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tread <csv file>\n\tReads <csv file>\n");
    } else if(args[1] == NULL ) {
        printf("read error: no file provided\n");
    } else {
        if(strcmp(csv_file, "*")) {
            for(size_t i = 0; i < active_dataset.len; i++) {
                $vector(active_dataset.inputs[i]);
                $vector(active_dataset.targets[i]);
            }
            free(active_dataset.inputs);
            free(active_dataset.targets);
        }

        if(parsecsv(args[1], &active_dataset) != 0) {
            printf("read error: could not parse '%s'\n", args[1]);
            free(csv_file);
            csv_file = strdup("*");
        } else {
            free(csv_file);
            csv_file = strdup(args[1]);
        }
    }
}

void __csv_print(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tprint\n\tPrints the currently selected dataset\n");
    } else if(args[1] != NULL ) {
        printf("print error: unexpected argument\n");
    } else if(!strcmp(csv_file, "*")) {
        printf("print error: no dataset currently selected\n");
    } else {
        if(active_dataset.len == 0) {
            printf("EMPTY DATASET\n");
        } else {
            for(size_t n = 0; n < active_dataset.inputs[0]->length; n++) {
                printf("%-11s", "input");
            }
            for(size_t n = 0; n < active_dataset.targets[0]->length; n++) {
                printf("%-11s", "output");
            }
            printf("\n");
            for(size_t i = 0; i < active_dataset.len; i++) {
                for(size_t n = 0; n < active_dataset.inputs[i]->length; n++) {
                    printf("%*f", -11, active_dataset.inputs[i]->vector[n]);
                }
                for(size_t n = 0; n < active_dataset.targets[i]->length; n++) {
                    printf("%*f", -11, active_dataset.targets[i]->vector[n]);
                }
                printf("\n");
            }
        }
    }
}

void __mlp(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tmlp\n\tEnters MLP Mode\n");
    } else if(args[1] != NULL ) {
        printf("mlp error: too many arguments\n");
    } else {
        mode = MLP;
    }
}

void __mlp_exit(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\texit\n\tExits MLP Mode\n");
    } else if(args[1] != NULL ) {
        printf("exit error: too many arguments\n");
    } else {
        mode = NONE;
    }
}

void __mlp_new(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tnew\n\tGenerates new MLP\n");
    } else if(args[1] != NULL ) {
        printf("new error: too many arguments\n");
    } else {
        int num_inputs = -1;
        int res = 0;
        while(res != 1 || num_inputs < 1) {
            printf("Number of input nodes: ");
            res = scanf("%i", &num_inputs);
            while (getchar() != '\n');
        }

        int num_outputs = -1;
        res = 0;
        while(res != 1 || num_outputs < 1) {
            printf("Number of output neurons: ");
            res = scanf("%i", &num_outputs);
            while (getchar() != '\n');
        }

        int num_hidden = -1;
        res = 0;
        while(res != 1 || num_hidden < 0) {
            printf("Number of hidden layers: ");
            res = scanf("%i", &num_hidden);
            while (getchar() != '\n');
        }

        size_t layers[num_hidden + 2];
        layers[0] = num_inputs;
        layers[num_hidden + 1] = num_outputs;

        activation_t activations[num_hidden + 1];

        for(int i = 0; i < num_hidden; i++) {
            res = 0;
            while(res != 1 || layers[i + 1] < 1 || layers[i + 1] > 1000) {
                printf("Number of neurons in hidden layer %i: ", i);
                res = scanf("%lu", &layers[i + 1]);
                while (getchar() != '\n');
            }

            int in = 0;
            res = 0;
            while(res != 1 || in < 1 || in > 3) {
                printf("Activation for neurons in hidden layer %i\n", i);
                printf("(1) Is Sigmoid\n");
                printf("(2) Is Linear\n");
                printf("(3) Is ReLU\n");
                printf("> ");
                res = scanf("%i", &in);
                while (getchar() != '\n');
            }
            switch(in) {
                case 1: activations[i] = sig; break;
                case 2: activations[i] = linear; break;
                case 3: activations[i] = ReLU; break;
            }
        }

        int in = 0;
        res = 0;
        while(res != 1 || in < 1 || in > 3) {
            printf("Activation for output layer\n");
            printf("(1) Is Sigmoid\n");
            printf("(2) Is Linear\n");
            printf("(3) Is ReLU\n");
            printf("> ");
            res = scanf("%i", &in);
            while (getchar() != '\n');
        }
        switch(in) {
            case 1: activations[num_hidden] = sig; break;
            case 2: activations[num_hidden] = linear; break;
            case 3: activations[num_hidden] = ReLU; break;
        }
        if(active_mlp != NULL) {
            mlp_free(active_mlp);
        }
        active_mlp = mlp_init(num_hidden + 2, layers, activations);
        printf("MLP Successfully Created\n");
        free(mlp_file);
        mlp_file = strdup("unnamed.mlp");
    }
}

void __mlp_train(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\ttrain <learning_rate> <epochs>\n\tTrains the selected MLP on the selected Dataset with the given <learning_rate> and <epochs>\n");
    } else if(args[1] == NULL || args[2] == NULL ) {
        printf("train error: missing arguments\n");
    } else if(!strcmp(csv_file, "*")) {
        printf("train error: no dataset selected\n");
    } else if(!strcmp(mlp_file, "*")) {
        printf("train error: no MLP selected\n");
    } else {
        char *end;
        size_t epochs = strtoul(args[2], &end, 10);
        if(*end != '\0' || args[2] == end) {
            printf("train error: could not parse epochs integer '%s'\n", args[2]);
            return;
        }

        *end;
        float lr = strtof(args[1], &end);
        if(*end != '\0' || args[1] == end) {
            printf("train error: could not parse learning rate epochs '%s'\n", args[1]);
            return;
        }

        train(active_mlp, &active_dataset, epochs, lr);
    }
}

void __mlp_predict(char **args) {
    if(args[1] != NULL && !strcmp(args[1], "?")) {
        printf("usage:\n\tpredict\n\tFinds MSE of Dataset without performing backpropogation\n\tpredict i ...\n\tGets a predicted value using ... as the float inputs\n");
    } else if(args[1] != NULL && strcmp(args[1], "i")) {
        printf("predict error: unexpected argument '%s'\n", args[1]);
    } else if(!strcmp(mlp_file, "*")) {
        printf("predict error: no MLP selected\n");
    }else if(args[1] != NULL){
        int inputs = 0;
        for(int i = 2; args[i] != NULL; i++) {
            inputs++;
        }
        if(inputs != active_mlp->layers[0]) {
            printf("predict error: expected %lu inputs but got %i\n", active_mlp->layers[0], inputs);
        } else {
            float in[inputs];
            for(int i = 0; i < inputs; i++) {
                char *end;
                in[i] = strtof(args[2 + i], &end);
                if(*end != '\0' || args[2 + i] == end) {
                    printf("predict error: could not parse input float '%s'\n", args[2 + i]);
                    return;
                }
            }
            vector_t *ins = vector(inputs, in);
            vector_t *outs = forward(active_mlp, ins, 0);
            printf("The model predicted: ");
            for(size_t i = 0; i < outs->length; i++) {
                printf("%f ", outs->vector[i]);
            }
            printf("\n");
            $vector(outs);
            $vector(ins);
        }
    } else if(!strcmp(csv_file, "*")) {
        printf("predict error: no dataset selected\n");
    }   else {
        predict(active_mlp, &active_dataset);
    }
}

typedef void(*cmd_action_t)(char**);

typedef struct {
    char *command;
    void(*action)(char**);
} command_t;

command_t none_commands[] = {
    (command_t){"mlp", __mlp}, 
    (command_t){"csv", __csv},
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd}, 
    (command_t){"ls", __ls}, 
    (command_t){"exit", __exit}, 
    NULL
};
command_t mlp_commands[] = {
    (command_t){"csv", __csv},
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd}, 
    (command_t){"ls", __ls},
    (command_t){"exit", __mlp_exit},
    (command_t){"new", __mlp_new}, 
    (command_t){"save", NULL}, 
    (command_t){"read", NULL}, 
    (command_t){"train", __mlp_train}, 
    (command_t){"predict", __mlp_predict}, 
    NULL
};
command_t csv_commands[] = {
    (command_t){"mlp", __mlp},
    (command_t){"cd", __cd}, 
    (command_t){"pwd", __pwd},
    (command_t){"ls", __ls},
    (command_t){"exit", __csv_exit},
    (command_t){"read", __csv_read}, 
    (command_t){"print", __csv_print}, 
    (command_t){"compile", NULL},
    NULL
};

command_t *commands[3] = {
    none_commands,
    mlp_commands,
    csv_commands
};

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
            prompt = smalloc(strlen(current_dir) + strlen(mlp_file) + 24);
            sprintf(prompt, "\033[1mMLP\033[0m (%s)[%s]%s", mlp_file, current_dir, sep);
        break;

        case CSV:
            prompt = smalloc(strlen(current_dir) + strlen(csv_file) +  24);
            sprintf(prompt, "\033[1mCSV\033[0m (%s)[%s]%s", csv_file, current_dir, sep);
        break;
    }
    return prompt;
}

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

    csv_file = strdup("*");
    mlp_file = strdup("*");

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
        line = trim(line);
        if(strlen(line) > 0) {
            add_history(line);
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
        free(linestart);
        free(prmt);
        prmt = prompt(mode);
    }
    return 0;

}