#ifndef HELPERS_H
#define HELPERS_H

namespace MLB
{

    //Alerts
    void ERROR(String Args);
    
    //String
    String Itos(int i);

    //char
    bool IsDigit(char c);

    bool IsNonZeroDigit(char c);

    bool CharToBuy(char side);

    //Int & UInts
    UInt8 GetUInt8(const char* buffer);

    UInt16 GetUInt16(const char* buffer, bool networkOrder = true);

    UInt32 GetUInt32(const char* buffer, bool networkOrder = true);

    UInt64 GetUInt64(const char* buffer, bool networkOrder = true);

    void SetUInt8(char* buffer, UInt8 value);

    void SetUInt16(char* buffer, UInt16 value, bool networkOrder = true);

    void SetUInt32(char* buffer, UInt32 value, bool networkOrder = true);

    void SetUInt64(char* buffer, UInt64 value, bool networkOrder = true);
}

#endif
