#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <time.h>
#define SERVER_PORT 8888
#define UDP_PORT 5678
#define BROADCAST_IP "192.168.1.255"

int createTcpListenSocket(int);
int createUdpBroadcastSocket(struct sockaddr_in *,char *, int);
int strpos(char *, char *);
void extract_values(char *, char *, char *, char *);


int main(int argc, char *argv[]) {
        int UDPsock=-1;                   /* Socket FD for UDP broadcast */
        int sockfd=-1;                    /* Socket FD for weather server listen */       
        int StationID=1;
        int SoftwareVer=2;
        char TimeStr[100];
        time_t now;
        struct tm *tim;
        int client_sock=-1;               /* Socket for weather connect instance */
        struct sockaddr_in client;        /* Weather Stn client */
        struct sockaddr_in broadcastAddr; /* UDP broadcast */

        unsigned int sendStringLen;       /* Length of string to UDP broadcast */
        int c, bytes_returned,x;
        char input_line[20000];       /* Input lines from the weater station are <200 bytes */
        char sendString[20000];           /* UDP Output string < input_line size */

        /* variables for storing weather values */
        char outTemp[10],outHumi[10],dewptf[10],windchillf[10],dailyrainmm[10];
        char winddir[10],avgwind[10],gustspeed[10],rainin[10],weeklyrainin[10];
        char dailyrainin[10],monthlyrainin[10],yearlyrainin[10],inTemp[10];
        char solarradiation[10],indoortempf[10],inHumi[10],RelPress[10];


    sockfd = createTcpListenSocket(SERVER_PORT);      //Create a listening socket on port SERVER_PORT for weather data
    UDPsock = createUdpBroadcastSocket(&broadcastAddr, BROADCAST_IP, UDP_PORT);       //Create udp broadcast socket on UDP_PORT

    while(1) {
        client_sock = accept(sockfd, (struct sockaddr *) &client, (socklen_t*) &c);
        bytes_returned =read(client_sock ,input_line , sizeof(input_line));
        if(bytes_returned<0) {
             syslog(LOG_NOTICE,"Server read returned %d (%s). Exiting\n",errno,strerror(errno));
             exit(EXIT_FAILURE);
       }
       close(client_sock);
       if(bytes_returned>0) {
        extract_values(input_line,"&tempf=","&", outTemp);
        extract_values(input_line,"&humidity=","&",outHumi);
        extract_values(input_line,"&dewptf=","&",dewptf);
        extract_values(input_line,"&windchillf=","&",windchillf);
        extract_values(input_line,"&winddir=","&",winddir);
        extract_values(input_line,"&windspeedmph=","&",avgwind);
        extract_values(input_line,"&windgustmph=","&",gustspeed);
        extract_values(input_line,"&rainin=","&",rainin);
        extract_values(input_line,"&dailyrainin=","&",dailyrainin);
        extract_values(input_line,"&weeklyrainin=","&",weeklyrainin);
        extract_values(input_line,"&monthlyrainin=","&",monthlyrainin);
        extract_values(input_line,"&yearlyrainin=","&",yearlyrainin);
        extract_values(input_line,"&solarradiation=","&",solarradiation);
        extract_values(input_line,"&indoortempf=","&",indoortempf);
        extract_values(input_line,"&indoorhumidity=","&",inHumi);
        extract_values(input_line,"&baromin=","&",RelPress);
        sprintf(RelPress,"%.1f",(strtof(RelPress,NULL)*33.86));
        sprintf(outTemp,"%.1f",(((strtof(outTemp,NULL)-32)/9)*5));
        //sprintf(dailyrainin,"%.1f",(strtof(dailyrainin,NULL)*25.4)); //Uncomment for mm instead of inches
               
        now = time(NULL);
        tim = localtime(&now);
        strftime(TimeStr, sizeof(TimeStr)-1,"%Y%m%d%H%M%S",tim);

        sprintf(sendString,"%d,%03d,%s,%s,%s,%s,%s,,%s,,,,,,,,,,",SoftwareVer,StationID,TimeStr,outTemp,outHumi,RelPress,avgwind,dailyrainin);
        sendStringLen=strlen(sendString);
        x=sendto(UDPsock, sendString, sendStringLen, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr));
        if(x!=sendStringLen) {
                syslog(LOG_NOTICE,"sendto(UDP) sent a different number of bytes than expected\n");
                exit(EXIT_FAILURE);
         }
       }
    }
}
void extract_values(char *input_line, char *start_needle, char *end_needle, char *ret_value) {
char field_value[20];
int fieldname_start, fieldname_end;
int value_end,needle_length;
        needle_length=strlen(start_needle);
        fieldname_start=strpos(start_needle, input_line);
        if(fieldname_start>0) {
                fieldname_end=fieldname_start+needle_length;
                value_end=strpos(end_needle, input_line+fieldname_end);
                if(value_end>0) {
                        strncpy(ret_value,input_line+fieldname_end, value_end);
                        ret_value[value_end]=0;
                }
                else {
                        strcpy(ret_value,"Unk");
                }
        }
        else {
                strcpy(ret_value,"Unk");
        }
}


int strpos(char *needle, char *haystack) {
char *offset;
        offset=strstr(haystack,needle);
        if(offset !=NULL)  return offset-haystack;
        return -1;
}

int createUdpBroadcastSocket(struct sockaddr_in *bcastAddr, char *broadcastip, int port) {
    int UDPsock;
    int broadcastPermission = 1;
    if ((UDPsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                syslog(LOG_NOTICE,"Failed to open UDP socket : %d (%s)\n",errno,strerror(errno));
                exit(EXIT_FAILURE);
    }
    if (setsockopt(UDPsock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0) {
                syslog(LOG_NOTICE,"setsockopt failed : %d (%s)\n",errno,strerror(errno));
                exit(EXIT_FAILURE);
    }
    memset(bcastAddr, 0, sizeof(bcastAddr));
    bcastAddr->sin_family = AF_INET;
    bcastAddr->sin_addr.s_addr = inet_addr(broadcastip);
    bcastAddr->sin_port = htons(port);
    syslog(LOG_NOTICE,"Ready to broadcast UDP on port %d\n",port);
    return UDPsock;
}


int createTcpListenSocket(int port) {
        int sockfd;
        struct sockaddr_in server_address;
        sockfd=socket(AF_INET , SOCK_STREAM , 0);
        if (sockfd == -1) {
                syslog(LOG_NOTICE,"Could not create listen socket: %d (%s)\n",errno,strerror(errno));
                exit(EXIT_FAILURE);
        }
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons( port );
        if( bind(sockfd,(struct sockaddr *) &server_address , sizeof(server_address)) < 0) {
                syslog(LOG_NOTICE,"Bind failed : %d (%s)\n",errno,strerror(errno));
                exit(EXIT_FAILURE);
        }
        syslog(LOG_NOTICE,"Listening for TCP connections on port %d\n",port);
        listen(sockfd,3);
        return sockfd;
}
