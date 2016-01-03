#define PROFIL_NAME    "APF51"

/* IO TYPE */
#define IO_LED      0
#define IO_BUTTON   1

/* Exchange type */
#define EXCHANGE_GET 1
#define EXCHANGE_SET 0

typedef struct
{
    long type;
    long index;
} str_param;

int straton_ios_init(void);

void straton_ios_close(void);

char * straton_ios_get_profile_name(void);

int straton_ios_exchange(str_param *pParams, int index,
                         long value_in, long *value_out);
