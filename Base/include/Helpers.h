#ifndef HELPERS_H
#define HELPERS_H

namespace MLB
{

    //char
    bool IsDigit(char c)
    {
        return (48 <= c && c <= 57);
    }

    bool IsNonZeroDigit(char c)
    {
        return (49 <= c && c <= 57);
    }

    //Int & UInts
    UInt8 GetUInt8(const char* buffer)
    {
        UInt8 result;
        memcpy(&result, buffer, 1);
        return result;
    }

    UInt16 GetUInt16(const char* buffer, bool networkOrder = true)
    {
        UInt16 result;
        memcpy(&result, buffer, 2);
        if(networkOrder)
        {
            return __bswap_16(result);
        }
        return result;
    }

    UInt32 GetUInt32(const char* buffer, bool networkOrder = true)
    {
        UInt32 result;
        memcpy(&result, buffer, 4);
        if(networkOrder)
        {
            return __bswap_32(result);
        }
        return result;
    }

    UInt64 GetUInt64(const char* buffer, bool networkOrder = true)
    {
        UInt64 result;
        memcpy(&result, buffer, 8);
        if(networkOrder)
        {
            return __bswap_64(result);
        }
        return result;
    }

    void SetUInt8(char* buffer, UInt8 value)
    {
        memcpy(buffer, &value, 1);
    }

    void SetUInt16(char* buffer, UInt16 value, bool networkOrder = true)
    {
        if(networkOrder)
        {
            UInt16 reverse = __bswap_16(value);
            memcpy(buffer, &reverse, 2);
        }
        else
        {
            memcpy(buffer, &value, 2);
        }
    }

    void SetUInt32(char* buffer, UInt32 value, bool networkOrder = true)
    {
        if(networkOrder)
        {
            UInt32 reverse = __bswap_32(value);
            memcpy(buffer, &reverse, 4);
        }
        else
        {
            memcpy(buffer, &value, 4);
        }
    }

    void SetUInt64(char* buffer, UInt64 value, bool networkOrder = true)
    {
        if(networkOrder)
        {
            UInt64 reverse = __bswap_64(value);
            memcpy(buffer, &reverse, 8);
        }
        else
        {
            memcpy(buffer, &value, 8);
        }
    }
}

#endif
