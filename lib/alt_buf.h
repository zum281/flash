#ifndef ALT_BUF_H
#define ALT_BUF_H

extern int in_alternate;

void exit_alternate_buffer(void);
void clear_screen(void);
void enter_alternate_buffer(void);

#endif
