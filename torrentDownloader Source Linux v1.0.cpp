#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>

/* v1.2 Added History feature to prevent redundant donwloads */
/* v1.3 Added Notifiication support */

using namespace std;

void showFound(istream &katcr);

ofstream log("/home/ekansh/Log/torrentDownloaderLog.txt", std::ios::app);

int main(){

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

    system("curl --compressed https://kat.cr/tv/ > /media/ekansh/Stuff/Torrents/Torrent\\ Downloader/katcr.txt");

    ifstream theList("/media/ekansh/Stuff/Torrents/Torrent Downloader/The List.txt");
    string nameOfShow;

    while (getline(theList, nameOfShow)) {
        string tempLine;
        ifstream katcr("/media/ekansh/Stuff/Torrents/Torrent Downloader/katcr.txt");

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

        katcr.close();
    };

    log << endl << endl;
    log.close();
    theList.close();
};

void showFound(istream &katcr) {
    ifstream historyIn("/media/ekansh/Stuff/Torrents/Torrent Downloader/history.txt");
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

    string command = "curl --compressed -o /media/ekansh/Stuff/Torrents/Torrent\\ Downloader/Torrent\\ Files/" + nameOfTorrent + ".torrent -A \"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36\" \""
     + addOfSubpage + "\"";

    char commandChar[1000];
    strncpy (commandChar, command.c_str(), 1000);

    bool downloadedBefore = false;
    string tempHistoryIn;

    while (getline(historyIn, tempHistoryIn)) {
        if (tempHistoryIn == nameOfTorrent) {downloadedBefore = true;};
    };

    historyIn.close();


    if (!(downloadedBefore)) {
        log << "Status: DOWNLOADING" << endl;
        log << "Command run: " << command << endl;
        ofstream historyOut("/media/ekansh/Stuff/Torrents/Torrent Downloader/history.txt", std::ios::app);
        historyOut << nameOfTorrent << endl;
        historyOut.close();

        string notifCommand = "notify-send \"Torrent Autodownloader: Started the download of " + nameOfTorrent + "\"";
        string androidNotifCommand = "curl --data \"apikey=47131c97985f688d045927ab101530dc40600e3affb0c09d&application=Torrent%20Downloader&event=Started%20Torrent%20Download&description=" + nameOfTorrent + "\" https://www.notifymyandroid.com/publicapi/notify";

        char notifCommandChar[1000];
        char androidNotifCommandChar[1000];
        strncpy(notifCommandChar, notifCommand.c_str(), 1000);
        strncpy(androidNotifCommandChar, androidNotifCommand.c_str(), 1000);

        system(commandChar);
        system(notifCommandChar);
        system(androidNotifCommandChar);

        log << "Android Notif Command sent: " << androidNotifCommand << endl << endl;
    }

    else {
        log << "Status: ABORTED - DOWNLOADED BEFORE" << endl;
        log << "Command NOT run: " << command << endl << endl;
    };
};
