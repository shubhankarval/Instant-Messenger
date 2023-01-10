#include <stdlib.h>
#include <string.h>

#include "serialize.h"

/* Unpack the given packet into the buffer unpacked.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that unpacked points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack(char *unpacked, void *packed) {
    int packet = *(int*)packed;
    packed += sizeof(int);
    if(packet==MESSAGE){
        strcpy(unpacked,"");
        char c = *(char*)packed;
        packed += sizeof(char);
        int name_len = 0;
        while(c!='\0'){
            if(name_len==NAME_SIZE){
                break;
            }
            strncat(unpacked,&c,1);
            name_len +=1;
            c = *(char*)packed;
            packed += sizeof(char);
        }
        strcat(unpacked,": ");
        //loop through padding
        for(int j=1;j<=NAME_SIZE-name_len-1;j++){
            packed += sizeof(char); //for '\0'
        }
        size_t len  = *(size_t*)packed;
        packed += sizeof(size_t);
        packed += sizeof(size_t); //for 0
        if(len==0){
            strcpy(unpacked,"");
            return -1;
        }
        for(size_t i=0;i<len;i++){
            char x = *(char*)packed;
            packed += sizeof(char);
            strncat(unpacked,&x,1);
        }
        char y = '\0';
        strncat(unpacked,&y,1);
        return MESSAGE;
    }
    if(packet==STATUS){
        strcpy(unpacked,"");
        char c = *(char*)packed;
        packed += sizeof(char);
        int name_len = 0;
        while(c!='\0'){
            if(name_len==NAME_SIZE){
                break;
            }
            strncat(unpacked,&c,1);
            name_len +=1;
            c = *(char*)packed;
            packed += sizeof(char);
        }
        strcat(unpacked," ");
        //loop through padding
        for(int j=0;j<NAME_SIZE-name_len-1;j++){
            packed += sizeof(char);
        }
        size_t len  = *(size_t*)packed;
        packed += sizeof(size_t);
        packed += sizeof(size_t); //for 0
        if(len==0){
            strcpy(unpacked,"");
            return -1;
        }
        for(size_t i=0;i<len;i++){
            char x = *(char*)packed;
            packed += sizeof(char);
            strncat(unpacked,&x,1);
        }
        char y = '\0';
        strncat(unpacked,&y,1);
        return STATUS;
    }
    if(packet==LABELED){
        strcpy(unpacked,"");
        char c = *(char*)packed;
        packed += sizeof(char);
        int name_len = 0;
        while(c!='\0'){
            if(name_len==NAME_SIZE){
                break;
            }
            strncat(unpacked,&c,1);
            name_len +=1;
            c = *(char*)packed;
            packed += sizeof(char);
        }
        strcat(unpacked,": @");
        //loop through padding
        for(int j=0;j<NAME_SIZE-name_len-1;j++){
            packed += sizeof(char);
        }
        size_t len  = *(size_t*)packed;
        packed += sizeof(size_t);
        if(len==0){
            strcpy(unpacked,"");
            return -1;
        }
        size_t target_len  = *(size_t*)packed;
        packed += sizeof(size_t);
        packed += sizeof(size_t); //for 0
        char *msg = (char *)malloc(1+(sizeof(char)*len));
        for(size_t i=0;i<len;i++){
            char x = *(char*)packed;
            packed += sizeof(char);
            strncat(msg,&x,1);
        }
        for(size_t i=0;i<target_len;i++){
            char x = *(char*)packed;
            packed += sizeof(char);
            strncat(unpacked,&x,1);
        }
        strcat(unpacked," ");
        for(size_t i=0;i<len;i++){
            char x = msg[i];
            strncat(unpacked,&x,1);
        }
        free(msg);
        char y = '\0';
        strncat(unpacked,&y,1);
        return LABELED;
    }
    return -1;
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack_statistics(struct statistics *statistics, void *packed) {
    int packet = *(int*)packed;
    packed += sizeof(int);
    if(packet==STATISTICS){
        int ubit_len = 0;
        char c;
        while(ubit_len<NAME_SIZE){
            c = *(char*)packed;
            if(c=='\0'){
                packed += sizeof(char);
            }
            else{
                statistics->sender[ubit_len] = c;
                packed += sizeof(char);
            }
            ubit_len += 1;
        }
        statistics->sender[ubit_len]='\0';
        int active_len = 0;
        char d;
        while(active_len<NAME_SIZE){
            d = *(char*)packed;
            if(d=='\0'){
                packed += sizeof(char);
            }
            else{
                statistics->most_active[active_len] = d;
                packed += sizeof(char);
            }
            active_len += 1;
        }
        statistics->most_active[active_len]='\0';
        statistics->most_active_count = *(int*)packed;
        packed += sizeof(int);
        statistics->invalid_count = *(long*)packed;
        packed += sizeof(long);
        statistics->refresh_count = *(long*)packed;
        packed += sizeof(long);
        statistics->messages_count = *(int*)packed;
        return STATISTICS;
    }
    return -1;
}


//stats
/*
        char c = *(char*)packed;
        packed += sizeof(char);
        int name_len = 0;
        int k=0;
        while(c!='\0'){
            if(name_len==NAME_SIZE){
                break;
            }
            statistics->sender[k] = c;
            name_len +=1;
            c = *(char*)packed;
            packed += sizeof(char);
            k+=1;
        }
        statistics->sender[k]='\0';
        //loop through padding
        for(int j=0;j<NAME_SIZE-name_len;j++){
            packed += sizeof(char);
        }
        */
/////
        /*
        char d = *(char*)packed;
        packed += sizeof(char);
        int act_len = 0;
        int p = 0;
        while(d!='\0'){
            if(act_len==NAME_SIZE){
                break;
            }
            statistics->most_active[p] = d;
            act_len +=1;
            d = *(char*)packed;
            packed += sizeof(char);
            p+=1;
        }
        statistics->most_active[p]='\0';
        //loop through padding
        for(int j=0;j<NAME_SIZE-act_len;j++){
            packed += sizeof(char);
        }
        */
