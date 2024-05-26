#ifndef TYPES_H
#define TYPES_H
//#pragma once

enum OptionType 
{   
    Call, 
    Put, 
    BinaryCall, 
    BinaryPut
};

const char* OptionTypeToString(OptionType type);

#endif
