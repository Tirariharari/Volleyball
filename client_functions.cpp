#include <QString>

void parsing_ip(QString ip, QString &serverIp, QString &serverPort)
{
    int good = 0;
    bool bad = 0;
    for(int i = ip.size()-1; i>-1; i--)
    {
        if(ip[i] == '.')
            good++;
        if(ip[i] == ':')
            good++;

        if((ip[i]).isNumber() || ip[i]=='.' || ip[i]==':')
        {
            //  Everything is good
        }else
        {
            bad = 1;
        }
    }

    if(good != 4 || bad)
    {
        serverIp = "Err";
        serverPort = "Err";
        return;
    }

    serverIp = "";
    serverPort = "";
    int i;
    for(i = 0 ; ip[i]!=':'; i++)
    {
        serverIp+=ip[i];
    }
    i+=1;
    for(i ; ip[i]!='\0'; i++)
    {
        serverPort+=ip[i];
    }
}
