#define LS_WINDOWS_IMPLEMENTATION
#include "lsWindows.h"
#undef LS_WINDOWS_IMPLEMENTATION

#define LS_ARENA_IMPLEMENTATION
#include "lsArena.h"
#undef LS_ARENA_IMPLEMENTATION

#define LS_CRT_IMPLEMENTATION
#include "lsCRT.h"
#undef LS_CRT_IMPLEMENTATION

#define LS_STRING_IMPLEMENTATION
#include "lsString.h"
#undef LS_STRING_IMPLEMENTATION

#define LS_HTTP_IMPLEMENTATION
#include "lsHTTP.h"
#undef LS_HTTP_IMPLEMENTATION


int main()
{
    WSAData wsadata = {};
    s32 result = WSAStartup(0x0202, &wsadata);
    
    string url = ls_strConstant("http://golarion.altervista.org/");
    HTTPConnection conn = ls_httpConnectByUrl(url, 80);
    
    ls_printf("Socket: %ld\n", conn.socket);
    ls_printf("ServerName: %s\n", conn.uri.authority.s);
    
    string response = ls_httpGET(conn);
    
    if(response.data) ls_printf("Response: \n\n%s\n\n", response);
    
    return 0;
}