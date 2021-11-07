#ifndef HEADER_H
#define HEADER_H

const int VERSION_LENGTH = 12;
const int SIGNATURE_LENGTH = 14;

struct Header
{
    const char version[VERSION_LENGTH] = "Version 2.1";
    const char signature[SIGNATURE_LENGTH] = "Nehochuvarmiy";
};

#endif