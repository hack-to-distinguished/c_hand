#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "message_store.h"
#include "cJSON.h"

# define BUFFER_SIZE 2048
# define START_SIZE 1024

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


msg_buffer ms_get_all_messages(flat_message_store* fms) {

    int index = 1;

    char* msg_by_user = malloc(START_SIZE);
    msg_by_user[0] = '\0';
    strcat(msg_by_user, "[");
    size_t mbu_len = strlen(msg_by_user);
    size_t mbu_cap = START_SIZE; // cap size needs to be big enough to include the first snprintf
    char* msg_construction_buffer = malloc(BUFFER_SIZE);

    while (fms[index].message != NULL)
    {
        char send_date_time[64];
        strftime(send_date_time, sizeof(send_date_time), "%b %d %T %Y", localtime(&fms[index].send_time));
        snprintf(
            msg_construction_buffer, BUFFER_SIZE,
            "{'sender_id': '%s', 'send_time': '%s', 'message': '%s'}", fms[index].sender_id, send_date_time, fms[index].message
        );

        int msg_c_b_len = strlen(msg_construction_buffer);
        if (msg_c_b_len + mbu_len + 1 >= mbu_cap) {
            mbu_cap = mbu_cap * 2;
            char *tmp_ptr = realloc(msg_by_user, mbu_cap);
            if (!tmp_ptr) {
                printf("Failed to reallocate memory for the messages\n");
            }
            msg_by_user = tmp_ptr;
        }
        strcat(msg_by_user, msg_construction_buffer);
        mbu_len += msg_c_b_len;
        index++;

        if (fms[index].message != NULL) {
            // We only add the comma if there is more data to append
            strcat(msg_by_user, ", ");
            mbu_len += strlen(", ");
        }

    }
    strcat(msg_by_user, "]");
    mbu_len += strlen("]");
    free(msg_construction_buffer);
    msg_buffer out = {mbu_len, msg_by_user};

    return out; // msg_by_use needs to be freed after use
}

msg_buffer ms_get_all_messages_desc(flat_message_store* fms, int* latest_entry_ptr) {

    int index = *latest_entry_ptr;

    char* msg_by_user = malloc(START_SIZE);
    msg_by_user[0] = '\0';
    strcat(msg_by_user, "[");
    size_t mbu_len = strlen(msg_by_user);
    size_t mbu_cap = START_SIZE;
    char* msg_construction_buffer = malloc(BUFFER_SIZE);

    while (index > 0)
    {
        snprintf(
            msg_construction_buffer, BUFFER_SIZE,
            "{'sender_id': '%s', 'message': '%s'}", fms[index].sender_id, fms[index].message
        );

        int msg_c_b_len = strlen(msg_construction_buffer);
        if (msg_c_b_len + mbu_len + 1 >= mbu_cap) {
            mbu_cap = mbu_cap * 2;
            char *tmp_ptr = realloc(msg_by_user, mbu_cap);
            if (!tmp_ptr) {
                printf("Failed to reallocate memory for the messages\n");
            }
            msg_by_user = tmp_ptr;
        }
        strcat(msg_by_user, msg_construction_buffer);
        mbu_len += msg_c_b_len;
        index--;

        if (index > 0) {
            // We only add the comma if there is more data to append
            strcat(msg_by_user, ", ");
            mbu_len += 2;
        }

    }
    strcat(msg_by_user, "]");
    printf("Message buffer: %s\n", msg_by_user);
    free(msg_construction_buffer);
    msg_buffer out = {mbu_len, msg_by_user};

    return out;
}

void ms_add_message(char* sender_id, char* recipient_id, char* message,
                    time_t* sent_time, time_t* recieved_time,
                    flat_message_store* fms, int *end_of_db_idx)
{
    int idx = *end_of_db_idx;
    idx++;
    printf("Inserting data at index %d\n", idx);
    time_t now = time(NULL);
    
    char *message_copy = strdup(message);

    cJSON *json = cJSON_Parse(message);
    char *string = cJSON_Print(json);
    printf("cJSON string: %s\n", string);
    free(string);
    cJSON_GetStringValue(json);
    free(json);

    cJSON *user_message = NULL;
    cJSON *s_id = NULL;
    cJSON *s_time = NULL;
    printf("Mate it was parsed\n");


    strcpy(fms[idx].sender_id, sender_id);
    strcpy(fms[idx].recipient_id, recipient_id);
    fms[idx].msg_len = strlen(message);
    fms[idx].message = malloc(fms[idx].msg_len + 1);
    strcpy(fms[idx].message, message);
    fms[idx].send_time   = time(&now);
    fms[idx].recv_time   = time(&now);
    fms[idx].send_status = 1;
    fms[idx].recv_status = 1;
    fms[idx].ID          = idx;

    *end_of_db_idx = idx;
    printf("Successfully added %s to index %d - ID: %d\n\n", message, idx, fms[idx].ID);
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
