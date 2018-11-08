/*
 * Copyright (c) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Oreo RIL library for MTK devices - Version (0.9).
 * by: daniel_hk(https://github.com/danielhk)
 *
 */

#ifndef ANDROID_RIL_INTERNAL_H
#define ANDROID_RIL_INTERNAL_H

namespace android {

#define RIL_SERVICE_NAME_BASE "slot"
#define RIL1_SERVICE_NAME "slot1"
#define RIL2_SERVICE_NAME "slot2"
#define RIL3_SERVICE_NAME "slot3"
#define RIL4_SERVICE_NAME "slot4"

/* Constants for response types */
#define RESPONSE_SOLICITED 0
#define RESPONSE_UNSOLICITED 1
#define RESPONSE_SOLICITED_ACK 2
#define RESPONSE_SOLICITED_ACK_EXP 3
#define RESPONSE_UNSOLICITED_ACK_EXP 4

// Enable verbose logging
#define VDBG 1

#define MIN(a,b) ((a)<(b) ? (a) : (b))

// Enable RILC log
#define RILC_LOG 1

#if RILC_LOG
    #define startRequest           sprintf(printBuf, "(")
    #define closeRequest           sprintf(printBuf, "%s)", printBuf)
    #define printRequest(token, req)           \
            RLOGD("[%04d]> %s %s", token, requestToString(req), printBuf)

    #define startResponse           sprintf(printBuf, "%s {", printBuf)
    #define closeResponse           sprintf(printBuf, "%s}", printBuf)
    #define printResponse           RLOGD("%s", printBuf)

    #define clearPrintBuf           printBuf[0] = 0
    #define removeLastChar          printBuf[strlen(printBuf)-1] = 0
    #define appendPrintBuf(x...)    snprintf(printBuf, PRINTBUF_SIZE, x)
#else
    #define startRequest
    #define closeRequest
    #define printRequest(token, req)
    #define startResponse
    #define closeResponse
    #define printResponse
    #define clearPrintBuf
    #define removeLastChar
    #define appendPrintBuf(x...)
#endif

typedef struct CommandInfo CommandInfo;

extern "C" const char * requestToString(int request);

typedef struct RequestInfo {
//    RIL_Client_Type *client;	// MTK, why must be in beginning? ***seem for external SIM,volte ???
    int32_t token;	//this is not RIL_Token
    CommandInfo *pCI;
    struct RequestInfo *p_next;
    char cancelled;
    char local;		// responses to local commands do not go back to command process
    RIL_SOCKET_ID socket_id;
    int wasAckSent;	// Indicates whether an ack was sent earlier
    RILChannelId cid;	// MTK, for command dispatch after onRequest()
} RequestInfo;

typedef struct CommandInfo {
    int requestNumber;
    int(*responseFunction) (int slotId, int responseType, int token,
            RIL_Errno e, void *response, size_t responselen);
    RILChannelId proxyId;
} CommandInfo;

RequestInfo * addRequestToList(int serial, int slotId, int request);

char * RIL_getServiceName();

void releaseWakeLock();

void onNewCommandConnect(RIL_SOCKET_ID socket_id);
// added for mtk
typedef enum {
    FMT_IGNORE = 0,	// don't care
    FMT_VOID,
    FMT_INTS,
    FMT_STR,
    FMT_STRS,
    FMT_CallFWST,
    FMT_IccAPDU,
    FMT_SIMIO,
    FMT_WrSMSSIM,
    FMT_RAW,
    FMT_AttchAPN,
    FMT_ImsSms,
    FMT_SIMAUTH,
    FMT_DATAPROF
} BUF_FMTS;

void my_enqueue(int request, void *buf, size_t buflen, BUF_FMTS bf, RequestInfo *pRI);

}   // namespace android

#endif //ANDROID_RIL_INTERNAL_H
