

int coprocess(char *command, char *argv[], int	*in, int *out, int *err);
int coroutine(void (*routine)(), void *data);
int shellproc(char *command, int *in, int *out, int *err);
