#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <syslog.h>
#define ROWS 30
#define COLS 80
#define MAXRECVSTRING 255
#define BROADCASTPORT 5678
#define FIELDCOUNT 5
#define GREEN 1
#define WHITE 2
#define RED 3
#define ORANGE 4
#define YELLOW 5
#define BLUE 6
int strpos(char *, char *);
int main(int argc, char *argv[]) {

        int i,j,sock,startpos,recvStringLen,packetCount,humidity;
        WINDOW *wnd;
        char recvString[MAXRECVSTRING+1];
        char values[FIELDCOUNT][100];
        char names[FIELDCOUNT][100];
        char progressBar[22][40];
        float temp,pressure,windspeed;
        unsigned short broadcastPort=BROADCASTPORT;
        struct sockaddr_in broadcastAddr;
        strcpy(progressBar[0],"[                     ] 0");
        strcpy(progressBar[1],"[#                    ] 1");
        strcpy(progressBar[2],"[##                   ] 2");
        strcpy(progressBar[3],"[###                  ] 3");
        strcpy(progressBar[4],"[####                 ] 4");
        strcpy(progressBar[5],"[#####                ] 5");
        strcpy(progressBar[6],"[######               ] 6");
        strcpy(progressBar[7],"[#######              ] 7");
        strcpy(progressBar[8],"[########             ] 8");
        strcpy(progressBar[9],"[#########            ] 9");
        strcpy(progressBar[10],"[##########           ]10");
        strcpy(progressBar[11],"[###########          ]11");
        strcpy(progressBar[12],"[############         ]12");
        strcpy(progressBar[13],"[#############        ]13");
        strcpy(progressBar[14],"[##############       ]14");
        strcpy(progressBar[15],"[###############      ]15");
        strcpy(progressBar[16],"[################     ]16");
        strcpy(progressBar[17],"[#################    ]17");
        strcpy(progressBar[18],"[###################  ]18");
        strcpy(progressBar[19],"[#################### ]19");
        strcpy(progressBar[20],"[#####################]20");

        strcpy(names[0],"Temperature: ");
        strcpy(names[1],"Humidity: ");
        strcpy(names[2],"Pressure: ");
        strcpy(names[3],"WindSpeed: ");
        strcpy(names[4],"Rainfall: ");

        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                fprintf(stderr,"Could not open listening port\n");
                exit(EXIT_FAILURE);
        }
        memset(&broadcastAddr, 0, sizeof(broadcastAddr));
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        broadcastAddr.sin_port = htons(broadcastPort);
        if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0) {
                fprintf(stderr,"Could not bind to listening port\n");
                exit(EXIT_FAILURE);
        }
        wnd=initscr();
        start_color();
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(ORANGE,203, COLOR_BLACK);
        clear();
        packetCount=0;
        while(1) {
                recvStringLen=0;
                recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0);
                if(recvStringLen>0) {
                        startpos=0;
                        recvString[recvStringLen] = '\0';
                        for(j=0;j<FIELDCOUNT-1;j++) {
                                memset(values[j],0,100);
                                i=strpos(",",recvString+startpos);
                                strncpy(values[j],recvString+startpos,i);
                                startpos+=(i+1);
                        }
                        strcpy(values[j],recvString+startpos);
                        //Temperature
                        attron(COLOR_PAIR(GREEN));
                        mvaddstr(1,1,names[0]);
                        temp=strtof(values[0],NULL);
                        if(temp>30) {
                                attron(COLOR_PAIR(RED));
                                attron(A_BLINK);
                                attron(A_BOLD);
                        }
                        else if(temp>25) attron(COLOR_PAIR(ORANGE));
                        else if(temp>20) attron(COLOR_PAIR(YELLOW));
                        else if(temp>10) attron(COLOR_PAIR(GREEN));
                        else {
                                attron(COLOR_PAIR(BLUE));
                                attron(A_BLINK);
                                attron(A_BOLD);
                        }
                        mvaddstr(1,15,values[0]);
                        attroff(A_BLINK);
                        attroff(A_BOLD);
                        //Humidity
                        attron(COLOR_PAIR(GREEN));
                        mvaddstr(2,1,names[1]);
                        humidity=atoi(values[1]);
                        if(humidity>90) attron(COLOR_PAIR(BLUE));
                        else if(humidity>50) attron(COLOR_PAIR(GREEN));
                        else attron(COLOR_PAIR(YELLOW));
                        mvaddstr(2,15,values[1]);

                        //Pressure
                        attron(COLOR_PAIR(GREEN));
                        mvaddstr(3,1,names[2]);
                        pressure=strtof(values[2],NULL);
                        if(pressure>999) attron(COLOR_PAIR(WHITE));
                        else {
                                attron(COLOR_PAIR(RED));
                                attron(A_BLINK);
                                attron(A_BOLD);
                        }
                        mvaddstr(3,15,values[2]);
                        attroff(A_BLINK);
                        attroff(A_BOLD);

                        //Windspeed
                        windspeed=strtof(values[3],NULL);
                        attron(COLOR_PAIR(GREEN));
                        mvaddstr(4,1,names[3]);
                        if(windspeed>20) {
                                attron(A_BLINK);
                                attron(A_BOLD);
                                attron(COLOR_PAIR(RED));
                        }
                        else attron(COLOR_PAIR(WHITE));
                        mvaddstr(4,15,values[3]);
                        attroff(A_BLINK);
                        attroff(A_BOLD);

                        //Rain
                        attron(COLOR_PAIR(GREEN));
                        mvaddstr(5,1,names[4]);
                        attron(COLOR_PAIR(WHITE));
                        mvaddstr(5,15,values[4]);
                        move(6,1);

                        //Bargraph
                        mvaddstr(6,1,progressBar[packetCount]);
                        packetCount++;
                        if(packetCount>20) packetCount=0;
                        refresh();

                }

        }
        endwin();
        return 0;
}

int strpos(char *needle, char *haystack) {
char *offset;
        offset=strstr(haystack,needle);
        if(offset !=NULL) {
                return offset-haystack;
        }
        return -1;
}

