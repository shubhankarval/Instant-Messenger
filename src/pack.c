#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "serialize.h"

/* Pack the user input provided in input into the appropriate message
 * type in the space provided by packed.  You can assume that input is a
 * NUL-terminated string, and that packed is a buffer of size
 * PACKET_SIZE.
 *
 * Returns the packet type for valid input, or -1 for invalid input.
 */
char *UBIT = "skvalimb";
int pack(void *packed, char *input) {
    //status
    if(input[0]=='/' && input[1]=='m' && input[2]=='e'){
        if(input[3]!=' '){
            return -1;
        }
         bool x = true;
         size_t len = 0;
         int j=4;
         size_t space_len = 0;
         while(input[j]!='\0'){
             if(x && input[j]==' '){
                 space_len += 1;
             }
             else{
                 x = false;
                 len+=1;
             }
             j+=1;
         }
         if(len > MAX_MESSAGE_SIZE){
             return -1;
         }
         *(int *)packed = STATUS;
         packed += sizeof(int);
         for(int i=0; i<8; i++){
             *(char *)packed = UBIT[i];
             packed += sizeof(char);
         }
         for(int i=1; i<=8; i++){
             *(char *)packed = '\0';
             packed += sizeof(char);
         }
         *(size_t *)packed = len;
         packed += sizeof(size_t);
         *(size_t *)packed = 0;
         packed += sizeof(size_t);
         for(size_t i=4+space_len;i<len+4+space_len;i++){
             *(char *)packed = input[i];
             packed += sizeof(char);
         }
         return STATUS;
    }
    //labelled
    else if(input[0]=='@'){
        int j = 1;
        size_t target_len = 0;
        while(input[j]!='\0'){
            if(input[j]==' '){
                break;
            }
            else{
                target_len += 1;
            }
            j+=1;
        }
        if(target_len<1 || target_len>NAME_SIZE){
            return -1;
        }
        bool x = true;
        size_t len = 0;
        size_t space_len = 0;
        while(input[j]!='\0'){
            if(x && input[j]==' '){
                space_len += 1;
            }
            else{
                x = false;
                len+=1;
            }
            j+=1;
        }
        if(len+target_len > MAX_MESSAGE_SIZE){
            return -1;
        }
        *(int *)packed = LABELED;
         packed += sizeof(int);
         for(int i=0; i<8; i++){
             *(char *)packed = UBIT[i];
             packed += sizeof(char);
         }
         for(int i=1; i<=8; i++){
             *(char *)packed = '\0';
             packed += sizeof(char);
         }
         *(size_t *)packed = len;
         packed += sizeof(size_t);
         *(size_t *)packed = target_len;
         packed += sizeof(size_t);
         *(size_t *)packed = 0;
         packed += sizeof(size_t);
         int l = 1 + target_len + space_len;
         for(size_t i=l;i<len+l;i++){
             *(char *)packed = input[i];
             packed += sizeof(char);
         }
         for(size_t i=1;i<target_len+1;i++){
             *(char *)packed = input[i];
             packed += sizeof(char);
         }
         return LABELED;
    }
    //statistics
    else if(input[0]=='/' && input[1]=='s' && input[2]=='t' && input[3]=='a' && input[4]=='t' && input[5]=='s'){
        if(input[6]!='\0'){
            return -1;
        }
        *(int *)packed = STATISTICS;
         packed += sizeof(int);
         for(int i=0; i<8; i++){
             *(char *)packed = UBIT[i];
             packed += sizeof(char);
         }
         for(int i=1; i<=8; i++){
             *(char *)packed = '\0';
             packed += sizeof(char);
         }
         return STATISTICS;
    }
    //message
    else{
         size_t len = 0;
         bool p = false;
         while(input[len]!='\0'){
             if(input[len]!=' '){
                 p = true;
             }
             len +=1;
         }
         if(len > MAX_MESSAGE_SIZE || !p){
             return -1;
         }
         *(int *)packed = MESSAGE;
         packed += sizeof(int);
         for(int i=0; i<8; i++){
             *(char *)packed = UBIT[i];
             packed += sizeof(char);
         }
         for(int i=1; i<=8; i++){
             *(char *)packed = '\0';
             packed += sizeof(char);
         }
         *(size_t *)packed = len;
         packed += sizeof(size_t);
         *(size_t *)packed = 0;
         packed += sizeof(size_t);
         for(size_t i=0;i < len;i++){
             *(char *)packed = input[i];
             packed += sizeof(char);
         }
         return MESSAGE;
    }
    return -1;
}

/* Create a refresh packet for the given message ID.  You can assume
 * that packed is a buffer of size PACKET_SIZE.
 *
 * You should start by implementing this method!
 *
 * Returns the packet type.
 */
int pack_refresh(void *packed, int message_id) {
    *(int *)packed = REFRESH;
    packed += sizeof(int);
    for(int i=0; i<8; i++){
        *(char *)packed = UBIT[i];
        packed += sizeof(char);
    }
    for(int i=1; i<=8; i++){
        *(char *)packed = '\0';
        packed += sizeof(char);
    }
    *(int *)packed = message_id;
    packed += sizeof(int);
    return REFRESH;
}
