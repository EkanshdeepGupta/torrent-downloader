#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <windows.h>

using namespace std;

void showFound(istream &katcr);

ofstream log("C:\\Log\\torrentDonwloaderLog.txt", std::ios::app);

int main(){
    HWND hwnd_win = GetForegroundWindow(); //Hide the window
    ShowWindow(hwnd_win,SW_HIDE);

    time_t currentTime;
    struct tm *localTime;

    time( &currentTime );
    localTime = localtime( &currentTime );

    int Day    = localTime->tm_mday;
    int Month  = localTime->tm_mon + 1;
    int Year   = localTime->tm_year + 1900;
    int Hour   = localTime->tm_hour;
    int Min    = localTime->tm_min;
    int Sec    = localTime->tm_sec;

    log << "-----RUN AT " << Day << "/" << Month << "/" << Year << " " << Hour << ":" << Min << ":" << Sec << "-----" << endl << endl;

    system("@echo off && curl --compressed https://kat.cr/tv/ > \"C:\\Torrents\\Torrent Downloader\\katcr.txt\"");

    ifstream theList("C:\\Torrents\\Torrent Downloader\\The List.txt");
    string nameOfShow;

    while (getline(theList, nameOfShow)) {
        string tempLine;
        ifstream katcr("C:\\Torrents\\Torrent Downloader\\katcr.txt");

        int count1 = 0;

        while (getline(katcr, tempLine)) {
            count1++;
            int posNameOfShow = tempLine.find(nameOfShow);
            if (posNameOfShow != -1) {
                cout << nameOfShow << "----" << "LINE NUMBER-----" << count1 << endl;
                showFound(katcr);
                break;
            };
        };
    };

    log << endl << endl;
};

void showFound(istream &katcr) {
    string tempLine;
    getline(katcr, tempLine);
    getline(katcr, tempLine);

    int posOfSite1 = tempLine.find("href=\"");
    int posOfSite2 = tempLine.find(".torrent?", posOfSite1+6);
    int posOfName = tempLine.find("[kat.cr]");
    int posOfName2 = tempLine.find("\"", posOfName+ 10);
    string nameOfTorrent = tempLine.substr(posOfName+8, posOfName2-posOfName-8);

    log << "Name Of Torrent: " << nameOfTorrent << endl;

    string addOfSubpage = "https:" + tempLine.substr(posOfSite1+6, posOfSite2-posOfSite1+2);

    log << "Address of torrent page: " << addOfSubpage << endl;

    string command = "curl --compressed -o \"C:\\Torrents\\Torrent Downloader\\Torrent Files\\" + nameOfTorrent + ".torrent\" -A \"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36\" \""
     + addOfSubpage + "\"";

     log << "Command run: " << command << endl << endl;

    char commandChar[1000];
    strncpy (commandChar, command.c_str(), 1000);

    system(commandChar);
}
