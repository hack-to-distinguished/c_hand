#include <time.h>

# define USER_ID_SIZE 256
# define MSG_STORE_SIZE 1000

typedef struct
{
    int    ID; // Increment with each new item
    char   sender_id[USER_ID_SIZE];
    char   recipient_id[USER_ID_SIZE];
    char*  message;
    size_t msg_len;
    time_t send_time;
    time_t recv_time;
    char   msg_type;
    size_t send_status; // 0=Failed 1=Sent 2=Pending
    size_t recv_status; // 1=Received 2=Read
} flat_message_store;
extern flat_message_store fms[MSG_STORE_SIZE];

typedef struct {
    int total_len;
    char* messages_by_user;
} msg_buffer;


typedef struct
{
    int    ID; // Increment with each new item
    char*  message;
    time_t connected_at;
    time_t disconnected_at;
    time_t last_message_send_time;
} chand_users;
extern chand_users c_users[MSG_STORE_SIZE];

extern int* end_of_db_idx;

int ms_point_to_last_entry(flat_message_store* fms);
void ms_view_all_entries(flat_message_store* fms, int* end_of_db_idx, int limit);
void ms_resize_store();
void ms_add_message(char* recipient_id, char* user_message,
                    flat_message_store* fms, int *end_of_db_idx);
void ms_stream_messages_desc(flat_message_store* fms, int* end_of_db_idx);
void ms_stream_user_messages_desc(flat_message_store* fms, int* end_of_db_idx,
                                  char* sender_id);
void ms_show_latest_msg();
void free_memory(flat_message_store* fms);
msg_buffer ms_get_all_messages(flat_message_store* fms);
msg_buffer ms_get_all_messages_desc(flat_message_store* fms, int* end_of_db_idx);

// users updates
void ms_register_or_update_user();
