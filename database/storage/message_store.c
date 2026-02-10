#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "message_store.h"

# define BUFFER_SIZE 2048
# define START_SIZE 32

// fms is extern so it will only be declared here
flat_message_store fms[MSG_STORE_SIZE]; 

void ms_view_all_entries(flat_message_store* fms, int* end_of_db_idx)
{
    int idx = *end_of_db_idx;
    int i = 0;
    if (idx - 10 > i) {
        i = idx;
    }
    int upper_bound = i + 10;
    
    for (i = i; i < upper_bound; i++) {
        printf("\nIteration num: %d\n", i);
        printf("Message Store ID: %d\n", fms[i].ID);
        printf("Message Store sender_id: %s\n", fms[i].sender_id);
        printf("Message Store recipient_id: %s\n", fms[i].recipient_id);
        printf("Message Store msg_len: %zu\n", fms[i].msg_len);
        printf("Message Store message: %s\n", fms[i].message);
        printf("Message Store send_time: %jd\n", fms[i].send_time);
        printf("Message Store recv_time: %jd\n", fms[i].recv_time);
        printf("Message Store msg_type: %c\n", fms[i].msg_type);
        // TODO: Fix the char pointer print above
        printf("Message Store send_status: %zu\n", fms[i].send_status);
        printf("Message Store recv_status: %zu\n", fms[i].recv_status);
        // i++;
    }
    printf("Messages printed: %d\n\n", i);
}

int ms_point_to_last_entry(flat_message_store* fms)
{
    int i = 1;
    while (fms[i].ID > fms[i - 1].ID)
    {
        printf("Loop count: %d\n", i);
        i++;
    }
    printf("Index of last entry: %d\n", i);
    printf("ID at last entry: %d\n", fms[i - 1].ID);
    return i-1;
}

// TODO: Create get latest entry
void ms_stream_messages_desc(flat_message_store* fms, int* end_of_db_idx)
{
    printf("\nStreaming messages\n");
    int index = *end_of_db_idx;
    while (fms[index - 1].ID < fms[index].ID)
    {
        puts(fms[index].message);
        index--;
    }
    printf("\n --- End of stream ---\n\n");
    return;
}

void ms_show_latest_msg();

void ms_stream_user_messages_desc(flat_message_store* fms, int* end_of_db_idx,
                                  char* sender_id)
{
    // INFO: Creating linked lists between a users message would make
    // getting those user's message much faster
    printf("\nGetting %s's messages\n", sender_id);
    int index = *end_of_db_idx;
    while (fms[index - 1].ID < fms[index].ID)
    {
        if (strcmp(fms[index].sender_id, sender_id) == 0)
        {
            puts(fms[index].message);
        }
        index--;
    }
    printf("\n --- End of stream ---\n\n");
    return;
}

msg_buffer ms_get_all_messages_desc(flat_message_store* fms, int* latest_entry_ptr) {
    int index = *latest_entry_ptr;
    printf("\nindex: %d\n", index);
    printf("fms[index].ID: %d\n", fms[index].ID);
    printf("fms->ID: %d\n", fms->ID);
    
    char* msg_by_user = malloc(START_SIZE);
    msg_by_user[0] = '\0';
    size_t mbu_len = 0, mbu_cap = START_SIZE;
    char* msg_construction_buffer = malloc(BUFFER_SIZE);

    
    while (fms[index - 1].ID > -1 && fms[index - 1].ID < fms[index].ID)
    {
        snprintf(
            msg_construction_buffer, BUFFER_SIZE,
            "{'%s': '%s'}", fms[index].sender_id, fms[index].message
        );
        
        int msg_c_b_len = strlen(msg_construction_buffer);
        if (msg_c_b_len + mbu_len + 1 >= mbu_cap) {
            mbu_cap = mbu_cap * 2;
            char *tmp_ptr = realloc(msg_by_user, mbu_cap);
            if (!tmp_ptr){ 
                printf("Failed to reallocate memory for the messages");
            }
            msg_by_user = tmp_ptr;
        }
        strcat(msg_by_user, msg_construction_buffer); 
        mbu_len += msg_c_b_len;
        
        index--;
        printf("CAP: %zu \nLEN: %zu \n ADDING: %s", mbu_cap, mbu_len, msg_construction_buffer);
    }
    msg_by_user[mbu_len] = '\0';
    free(msg_construction_buffer);
    msg_buffer out = {mbu_len, msg_by_user};
    
    printf("\n --- Messages: --- \n%s", msg_by_user);
    return out;
}

void ms_add_message(char* sender_id, char* recipient_id, char* user_message,
                    time_t* sent_time, time_t* recieved_time,
                    flat_message_store* fms, int *end_of_db_idx)
{
    int idx = *end_of_db_idx;
    idx++;
    printf("Inserting data at index %d\n", idx);

    strcpy(fms[idx].sender_id, sender_id);
    strcpy(fms[idx].recipient_id, recipient_id);
    fms[idx].msg_len = strlen(user_message);
    fms[idx].message = malloc(fms[idx].msg_len + 1);
    strcpy(fms[idx].message, user_message);
    fms[idx].send_time   = time(sent_time);
    fms[idx].recv_time   = time(recieved_time);
    fms[idx].send_status = 1;
    fms[idx].recv_status = 1;
    fms[idx].ID          = idx;

    *end_of_db_idx = idx;
    printf("Successfully added %s to index %d\n\n", user_message, idx);
    return;

    // IMPROVEMENT:
    // - Find out how to not rely on having a list end flag
    // - This function needs to work in conflicting cases
}

void ms_resize_store()
{ // TODO: Implement resizing mechanism when the store fills up
    return;
}

void free_memory(flat_message_store* fms)
{
    printf("Freeing memory allocated to messages\n");
    int i = 1;
    free(fms[0].message);
    while (fms[i].ID > fms[i - 1].ID)
    {
        free(fms[i].message);
        i++;
    }
    return;
}


void ms_http_message_store_endpoint(int sock, const char *body) {
    char response[BUFFER_SIZE];
    char escaped[BUFFER_SIZE];

    // JSON escaping
    size_t j = 0;
    for (size_t i = 0; body[i] && j < BUFFER_SIZE - 2; i++) {
        if (body[i] == '"' || body[i] == '\\') {
            escaped[j++] = '\\';
        }
        escaped[j++] = body[i];
    }
    escaped[j] = '\0';

    int body_len = snprintf(NULL, 0, "{\"message\": \"%s\"}", escaped);

    snprintf(response, sizeof(response),
         "HTTP/1.1 200 OK\r\n"
         "Content-Type: application/json\r\n"
         "Access-Control-Allow-Origin: *\r\n"
         "Content-Length: %d\r\n"
         "Connection: close\r\n"
         "\r\n"
         "{\"message\": \"%s\"}",
         body_len, escaped);
}

// int main()
// {
//     // INFO: Code below is for testing purposes
//     flat_message_store fms[MSG_STORE_SIZE];
//     printf("Message store initalized\n\n");
//     int*   end_of_db_idx = &fms[0].ID;
//     time_t now           = time(NULL);
//
//     ms_add_message("Christian", "Juan", "Christian test msg", &now, &now, fms,
//                    &end_of_db_idx);
//
//     ms_add_message("chris", "nj", "chris test msg", &now, &now, fms,
//                    &end_of_db_idx);
//     ms_add_message("Alejandro", "Christian", "NJ test msg", &now, &now, fms,
//                    &end_of_db_idx);
//     // ms_view_all_entries(fms);
//
//     ms_stream_messages_desc(fms, &end_of_db_idx);
//     ms_stream_user_messages_desc(fms, &end_of_db_idx, "Christian");
//
//     // end_of_db_idx = ms_point_to_last_entry(fms);
//
//     free_memory(fms);
//     return (0);
// }
