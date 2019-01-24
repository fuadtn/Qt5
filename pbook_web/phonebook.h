#ifndef PBOOK_H
#define PBOOK_H

#include <QObject>
#include <qrestapi.h>
#include <stdexcept>

#define PHONEBOOK_CONTACTS      "contacts"
#define PHONEBOOK_BD_TODAY      "bd_today"
#define PHONEBOOK_BD_TOMORROW   "bd_tomorrow"
#define PHONEBOOK_URL           "http://kk.kandk.ru/api/ph/"
#define PHONEBOOK_NAME          1
#define PHONEBOOK_PHONE         2
#define PHONEBOOK_EMAIL         3
#define PHONEBOOK_STATE         4


class phonebook;


class List
{
    friend class phonebook;
public:
    List();

    const QString at(int i, int j)
    {
        if(i >= list.size() || i < 0)
            throw std::out_of_range("invalid i index");
        if(j >= list.size() || j < 0)
            throw std::out_of_range("invalid j index");
        return this->list.at(i).toArray().at(j).toString();
    }

    const int size()
    {
        return this->list.size();
    }

    bool isEmpty() {
        return this->list.size() == 0 ? true : false;
    }

protected:
    QJsonArray list;
};


class EList : public List
{
public:
    int contains(int i, QString to_search)
    {
        if(i >= this->list.size())
            throw std::out_of_range("invalid index");

            for(int j = 1; j < this->list.at(i).toArray().size(); j++)
                if(this->list.at(i).toArray().at(j).toString().contains(to_search))
                    return j;
        return -1;
    }
};


class phonebook
{
public:
    EList contacts;
    List bdays_today;
    List bdays_tomorrow;

    bool upload() {
        QJsonDocument doc;
        QRestAPI RestAPI;
        if(RestAPI.GET(PHONEBOOK_URL, QJsonObject(), doc) == HTTP_200_OK)
        {
            this->contacts.list = doc.object().value(PHONEBOOK_CONTACTS).toArray();
            this->bdays_today.list = doc.object().value(PHONEBOOK_BD_TODAY).toArray();
            this->bdays_tomorrow.list = doc.object().value(PHONEBOOK_BD_TOMORROW).toArray();
            return true;
        }
        return false;
    }

    bool send_to_edit(QString message) {
        QRestAPI RestAPI;
        QJsonObject obj;
        obj["message"] = message;
        return RestAPI.POST(PHONEBOOK_URL, obj) == HTTP_200_OK ? true : false;
    }
};

#endif // PBOOK_H
