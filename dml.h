/* dml.h - UPDATED for Stage 2 (4 chars) and Stage 3 (scrolling) */
void display_char(unsigned char ch,unsigned int delay);
void display_4chars(unsigned char c1,unsigned char c2,unsigned char c3,unsigned char c4,unsigned int duration_ms);
void scroll_string(char *msg, unsigned int step_delay_ms);
void display_scroll(unsigned char *c1);