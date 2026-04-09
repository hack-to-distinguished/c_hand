#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "message_store.h"
#include "../../utils/JSON/cJSON.h"

# define BUFFER_SIZE 2048
# define START_SIZE 1024

// fms is extern so it will only be declared here
flat_message_store fms[MSG_STORE_SIZE];
chand_users c_users[CHAND_USERS_SIZE];


void ms_view_all_entries(flat_message_store* fms, int* end_of_db_idx, int limit)
{
    if (limit == 0) {
        limit = 10;
    }
    printf("Searching the last %d entries\n", limit);

    int idx = *end_of_db_idx;
    int i = 0;
    if (idx - limit > i) {
        i = idx - 1;
    }
    int upper_bound = i + limit;

    for (i = i; i < upper_bound; i++) {
        if (fms[i].message != NULL) {
            printf("\nIteration num: %d\n", i);
            printf("Message Store ID: %d\n", fms[i].ID);
            printf("Message Store sender_id: %s\n", fms[i].sender_id);
            printf("Message Store recipient_id: %s\n", fms[i].recipient_id);
            printf("Message Store message: %s\n", fms[i].message);
            printf("Message Store send_time: %jd\n", fms[i].send_time);
            printf("Message Store recv_time: %jd\n", fms[i].recv_time);
        }
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

void ms_add_message(char* recipient_id, char* message, flat_message_store* fms, int *end_of_db_idx)
{
    int idx = *end_of_db_idx;
    idx++;
    printf("Inserting data at index %d\n", idx);
    time_t now = time(NULL);

    cJSON *json = cJSON_Parse(message);
    char *string = cJSON_Print(json);
    printf("cJSON string: %s\n", string);
    free(string);


    cJSON *jsonUserMessage = cJSON_GetObjectItem(json, "message");
    if (!cJSON_IsString(jsonUserMessage)) {
        perror("Message isn't a string");
        return;
    }

    cJSON *jsonSenderID = cJSON_GetObjectItem(json, "sender_id");
    cJSON *jsonSendTime = cJSON_GetObjectItem(json, "send_time");


    strcpy(fms[idx].sender_id, jsonSenderID->valuestring);
    strcpy(fms[idx].recipient_id, recipient_id);
    fms[idx].msg_len = strlen(jsonUserMessage->valuestring);
    fms[idx].message = malloc(fms[idx].msg_len + 1);
    strcpy(fms[idx].message, jsonUserMessage->valuestring);
    fms[idx].send_time   = time(&now);
    fms[idx].recv_time   = time(&now);
    fms[idx].send_status = 1;
    fms[idx].recv_status = 1;
    fms[idx].ID          = idx;

    cJSON_Delete(json);

    *end_of_db_idx = idx;
    printf("Successfully added %s to index %d - ID: %d\n\n", message, idx, fms[idx].ID);
    ms_view_all_entries(fms, end_of_db_idx,        2);
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


int ms_register_user(int client_fd, char* payload, chand_users* c_users) {
    printf("MS REGISTER USER\n");
    // INFO: 
    // - client_fds are not tied to a specific user, I move them around
    // as users change username, connect, reconnect etc
    // A new user can pick a username that already exists (if the user isn't conncted)
    // and at that point, all his information should be transferred to that user

    // TODO: call the ms_change_username (or however it's called) to
    // check that the user doesn't already exists in the db
    int index = 0;

    cJSON *json = cJSON_Parse(payload);
    if (!json) {
        fprintf(stderr, "ms_register user: invalid JSON payload\n");
        return -1;
    }
    cJSON *jsonUsername = cJSON_GetObjectItem(json, "username");
    if (!cJSON_IsString(jsonUsername) || jsonUsername->valuestring == NULL) {
        perror("Message isn't a string");
        fprintf(stderr, "ms_register_user: missing or invalid username\n");
        cJSON_Delete(json);
        return -1;
    }
    char* cur_user = jsonUsername->valuestring;
    printf("User to update: %s\n", cur_user);


    while (index < CHAND_USERS_SIZE && c_users[index].username != NULL) {
        printf("Searching %d\n", index);
        char* db_username = c_users[index].username;
        if (strcmp(cur_user, db_username) == 0) {
            // We've found the user
            printf("User %s exists in the store, update their info\n", db_username);
            ms_update_user(client_fd, index, USER_ACTION_CONNECT, c_users, "");
            cJSON_Delete(json);
            return 0;
        };
        index++;
    }
    
    // we have the index of the current user now, we check that they're not just looking to change username
    cJSON_bool isNewUsername = cJSON_HasObjectItem(json, "new_username");
    if (isNewUsername) {
        cJSON *jsonNewUsername = cJSON_GetObjectItem(json, "new_username");
        if (!cJSON_IsString(jsonNewUsername) || jsonNewUsername->valuestring == NULL) {
            printf("New username field found as null. Assuming there is no desire to change username\n");
        } else {
            ms_update_user(client_fd, index, USER_ACTION_CHANGE_USERNAME, c_users, jsonNewUsername->valuestring);
            cJSON_Delete(json);
            return 0;
        }
    }
    
    if (index >= CHAND_USERS_SIZE) {
        fprintf(stderr, "ms_register_user: user store full, cannot register user %s\n", cur_user);
    }

    printf("No existing user under that name, registering now\n");
    time_t now = time(NULL);
    
    c_users[index].ID = index;
    c_users[index].username = malloc(strlen(cur_user) + 1);
    strcpy(c_users[index].username, cur_user);
    
    c_users[index].client_fd = client_fd;
    c_users[index].connected_at = now;
    c_users[index].disconnected_at = (time_t)(-1); // NULL time
    c_users[index].last_message_send_time = (time_t)(-1); // NULL time

    printf("User %s added\n\n", c_users[index].username);
    cJSON_Delete(json);
    
    return 0;
}

void ms_disconnect_user(int client_fd, char* payload, chand_users* c_users) {
    int index = 0;

    cJSON *json = cJSON_Parse(payload);
    if (!json) {
        fprintf(stderr, "ms_disconnect_user: invalid JSON payload\n");
        return;
    }

    cJSON *jsonUsername = cJSON_GetObjectItem(json, "username");
    if (!cJSON_IsString(jsonUsername) || jsonUsername->valuestring == NULL) {
        fprintf(stderr, "ms_disconnect_user: missing or invalid 'username'\n");
        cJSON_Delete(json);
        return;
    }

    const char *cur_user = jsonUsername->valuestring;

    while (index < CHAND_USERS_SIZE && c_users[index].username != NULL) {
        if (strcmp(c_users[index].username, cur_user) == 0) {
            printf("disconnecting user %s\n", c_users[index].username);
            time_t now = time(NULL);
            c_users[index].disconnected_at = now;
            c_users[index].client_fd = -1;
            cJSON_Delete(json);
            return;
        }
        index++;
    }

    fprintf(stderr, "ms_disconnect_user: user '%s' not found in store\n", cur_user);
    cJSON_Delete(json);
    return;
}



int ms_update_user(int client_fd, int index, user_action action_field, chand_users* c_users, char* new_username) {
    if (index < 0 || index >= CHAND_USERS_SIZE) {
        fprintf(stderr, "ms_update_user: index out of range\n");
        return -1;
    }

    printf("Updating information for %s\n", c_users[index].username ? c_users[index].username : "(unknown)");

    time_t now = time(NULL);
    // Update client fd for the user record at index
    c_users[index].client_fd = client_fd;

    switch(action_field) {
        case USER_ACTION_CONNECT:
            printf("%s connected\n", c_users[index].username ? c_users[index].username : "(unknown)");
            c_users[index].connected_at = now;
            break;

        case USER_ACTION_DISCONNECT:
            printf("%s disconnected\n", c_users[index].username ? c_users[index].username : "(unknown)");
            c_users[index].disconnected_at = now;
            // Optionally mark client fd as not connected
            c_users[index].client_fd = -1;
            break;

        case USER_ACTION_SEND_MESSAGE:
            printf("%s sent message\n", c_users[index].username ? c_users[index].username : "(unknown)");
            c_users[index].last_message_send_time = now;
            break;

        case USER_ACTION_CHANGE_USERNAME: {
            if (!new_username) {
                fprintf(stderr, "ms_update_user: new_username is NULL\n");
                return -1;
            }

            // Check that the new username doesn't already exist in the store
            char *cur_user = new_username;
            int i = 0;
            for (i = 0; i < CHAND_USERS_SIZE && c_users[i].username != NULL; i++) {
                if (strcmp(cur_user, c_users[i].username) == 0) {
                    printf("User %s exists in the store you can't select this username\n", c_users[i].username);
                    perror("Username selected already exists");
                    fprintf(stderr, "ms_update_user: username already exists\n");
                    return -1;
                }
            }

            // Replace the username at the provided index
            printf("Replacing user %s with: %s\n\n", c_users[index].username ? c_users[index].username : "(null)", cur_user);
            free(c_users[index].username);
            c_users[index].username = malloc(strlen(cur_user) + 1);
            if (!c_users[index].username) {
                fprintf(stderr, "ms_update_user: malloc failed\n");
                return -1;
            }
            strcpy(c_users[index].username, cur_user);
            c_users[index].client_fd = client_fd;
            c_users[index].connected_at = now;

            printf("User %s added\n\n", c_users[index].username);
            break;
        }

        default:
            fprintf(stderr, "ms_update_user: unknown action field\n");
            return -1;
    }

    return 0;
}


user_list_buffer ms_get_all_users(chand_users* c_users) {
    cJSON *users_obj = cJSON_CreateObject();
    if (!users_obj) {
        user_list_buffer out = {0, NULL};
        return out;
    }
    
    int index = 0;
    while (index < CHAND_USERS_SIZE && c_users[index].username != NULL) {
        cJSON *u = cJSON_CreateObject();
        if (!u) {
            cJSON_Delete(users_obj);
            user_list_buffer out = {0, NULL};
            return out;
        }
        
        cJSON_AddNumberToObject(u, "client_fd", c_users[index].client_fd);
        cJSON_AddNumberToObject(u, "connected_at", (double)c_users[index].connected_at);
        cJSON_AddNumberToObject(u, "disconnected_at", (double)c_users[index].disconnected_at);
        cJSON_AddNumberToObject(u, "last_message_send_time", (double)c_users[index].last_message_send_time);
    
        cJSON_AddItemToObject(users_obj, c_users[index].username, u);
        index++;
    }
    
    char *json_str = cJSON_PrintUnformatted(users_obj);
    if (!json_str) {
        json_str = strdup("{}");
    }
    
    user_list_buffer out = {(int)strlen(json_str), json_str};
    // json_str remains allocated and is deleted later (in http.c)
    cJSON_Delete(users_obj);
    return out;
}