typedef struct
{
    uint32_t syscall_num;
    uint32_t args[6];
} syscall_args_t;

void syscall_handler(syscall_args_t* args);
