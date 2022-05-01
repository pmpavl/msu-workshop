#ifndef RECORD_HPP
#define RECORD_HPP
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
class record
{
public:
    char *name;
    int phone;
    int group;
    int init(char *iname, int igroup, int iphone);
    int reinit(char *iname, int igroup, int iphone);
    ~record();

    record(char *iname = 0, int igroup = 0, int iphone = 0);
    record(const record &init);

    int set_name(char *iname);
    void set_group(int igroup);
    void set_phone(int iphone);

    char *get_name();
    int get_group();
    int get_phone();

    void show();

    int operator<(const record &rec);
    int operator>(const record &rec);
    int operator<=(const record &rec);
    int operator>=(const record &rec);
    int operator==(const record &rec);
    int operator!=(const record &rec);
    record &operator=(const record &irec);
};

int record::init(char *iname, int igroup, int iphone)
{
    if (iname)
    {
        int len = strlen(iname);
        name = new char[len + 1];
        if (name)
            strcpy(name, iname);

        else
            return 1;
    }
    else
    {
        name = 0;
    }
    group = igroup;
    phone = iphone;
    return 0;
}

int record::reinit(char *iname, int igroup, int iphone)
{
    this->~record();
    return init(iname, igroup, iphone);
}

record::~record()
{
    if (name)
    {
        delete[] name;
    }
    name = 0;
    group = 0;
    phone = 0;
}

record::record(char *iname, int igroup, int iphone)
{
    init(iname, igroup, iphone);
}

record::record(const record &rec)
{
    init(rec.name, rec.group, rec.phone);
}

char *record::get_name()
{
    return (char *)name;
}

int record::get_group()
{
    return group;
}

int record::get_phone()
{
    return phone;
}

int record::set_name(char *iname)
{
    return reinit(iname, group, phone);
}

void record::set_group(int igroup)
{
    group = igroup;
}

void record::set_phone(int iphone)
{
    phone = iphone;
}

void record::show()
{
    cout << name << ' ' << group << ' ' << phone << ' ' << endl;
}

int record::operator<(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);

    if (i < 0)
        return 1;
    else if (i > 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i < 0)
            return 1;
        else if (i > 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i < 0);
        }
    }
}

int record::operator>(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);

    if (i > 0)
        return 1;
    else if (i < 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i > 0)
            return 1;
        else if (i < 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i > 0);
        }
    }
}

int record::operator<=(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);
    if (i < 0)
        return 1;
    else if (i > 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i < 0)
            return 1;
        else if (i > 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i <= 0);
        }
    }
}

int record::operator>=(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);
    if (i > 0)
        return 1;
    else if (i < 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i > 0)
            return 1;
        else if (i < 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i >= 0);
        }
    }
}

int record::operator==(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);
    if (i != 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i != 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i == 0);
        }
    }
}

int record::operator!=(const record &rec)
{
    int i;
    i = strcmp(name, rec.name);
    if (i == 0)
        return 0;
    else
    {
        i = group - rec.group;
        if (i == 0)
            return 0;
        else
        {
            i = phone - rec.phone;
            return (i != 0);
        }
    }
}

record &record::operator=(const record &irec)
{
    reinit(irec.name, irec.group, irec.phone);
    return *this;
}

#endif
