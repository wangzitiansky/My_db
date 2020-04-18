#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPAPE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum{
    STATEMENT_INSERT, STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;

InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer -> buffer = NULL;
    input_buffer -> buffer_length = 0;
    input_buffer -> input_length = 0;
    return input_buffer;
}

void print_prompt(){
    printf("db > ");
}

// 从命令行读取输入
void read_input(InputBuffer* input_buffer){
    ssize_t bytes_read = getline(&(input_buffer -> buffer), &(input_buffer -> buffer_length), stdin);
    if (bytes_read < 0)
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    // 忽略换行符
    input_buffer -> input_length = bytes_read - 1;
    input_buffer -> buffer[bytes_read - 1] = 0;
}

// 释放内存
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer -> buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if (strcmp(input_buffer->buffer, ".exit") == 0)
    {
        exit(EXIT_SUCCESS);
    } else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

// 这里先识别是 insert or select 
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    {
        statement->type = STATEMENT_INSERT;
        return PREPAPE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPAPE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){
    switch (statement->type)
    {
    case (STATEMENT_INSERT):
        printf("This is where we would do an insert\n");
        break;
    case (STATEMENT_SELECT):
        printf("This is where we would do a select\n");
        break;
    }
}

int main(int argc, char const *argv[])
{
    // 接受输入
    InputBuffer* input_buffer = new_input_buffer();
    // 事件循环
    while(true){
        print_prompt();
        read_input(input_buffer);
        if (strcmp(input_buffer -> buffer, ".exit") == 0)
        {
            if (input_buffer -> buffer[0] == '.')
            {
                switch (do_meta_command(input_buffer))
                {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s\n", input_buffer -> buffer);
                    continue;
                }
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case (PREPAPE_SUCCESS):
            break;
        
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
            continue;
        }
        execute_statement(&statement);
        printf("Executed.\n");
    }
    return 0;
}
