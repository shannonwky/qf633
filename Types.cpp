#include "Types.h"

const char* OptionTypeToString(OptionType type) {
    switch (type) {
        case Call: return "Call";
        case Put: return "Put";
        case BinaryCall: return "BinaryCall";
        case BinaryPut: return "BinaryPut";
        default: return "Unknown";
    }
}
