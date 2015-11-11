extern int flashenv_cmd(char *arg1, char *arg2);
extern char * flashenv_get(char *name);
extern int flashenv_set(char *name, char *value);
extern int flashenv_erase(void);
extern void flashenv_boot(void);
