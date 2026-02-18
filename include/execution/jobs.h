#ifndef JOBS_H
#define JOBS_H

void handle_background(char **args);
void reap_background_jobs(void);
int is_background(char **args);

#endif
