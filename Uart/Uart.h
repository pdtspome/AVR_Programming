

#define START_CHARACTER '^'
#define END_CHARACTER '/'
#define MAX_BUFFER 50

void Uart_init(void);
void Uart_Txchar(char c);
void Uart_Txstr(char* str);
char Uart_Rx(char *Rx_Str);