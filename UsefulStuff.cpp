#include "UsefulStuff.h"


#include <bitset>
#include <iostream>

#include <QtGlobal>

void byteOrderTest()
{
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    quint8 c_arr[4]{0x01, 0x02, 0x03, 0x04};
    quint16 _val_1 = 256;
    quint16 _val_2 = 0b1111111100000011;
    std::bitset<16> _x_1(_val_1);
    std::bitset<16> _x_2(_val_2);

    quint8* b0 = (reinterpret_cast<quint8*>(&_val_1) + 0);
    quint8* b1 = (reinterpret_cast<quint8*>(&_val_1) + 1);
    std::cout << "_val_1 = " << _val_1 << " bitwise = " << _x_1 << std::endl;
    std::cout << "b0 = " << std::bitset<8>(*b0) << " addr = " << (void*)b0 << std::endl;
    std::cout << "b1 = " << std::bitset<8>(*b1) << " addr = " << (void*)b1 << std::endl;

    b0 = (reinterpret_cast<quint8*>(&_val_2) + 0);
    b1 = (reinterpret_cast<quint8*>(&_val_2) + 1);
    std::cout << "_val_2 = " << _val_2 << " bitwise = " << _x_2 << std::endl;
    std::cout << "b0 = " << std::bitset<8>(*b0) << " addr = " << (void*)b0 << std::endl;
    std::cout << "b1 = " << std::bitset<8>(*b1) << " addr = " << (void*)b1 << std::endl;

    std::cout << "_val_1 as 4-byte = " << std::bitset<32>(*reinterpret_cast<int*>(&_val_1)) << std::endl;
    std::cout << "_val_2 as 4-byte = " << std::bitset<32>(*reinterpret_cast<int*>(&_val_2)) << std::endl;
    std::cout << "reinterpret_cast<quint8*>(&_val_1) + 2 = " << std::bitset<8>(*(reinterpret_cast<quint8*>(&_val_1) + 2)) << std::endl;
    std::cout << "reinterpret_cast<quint8*>(&_val_1) + 1 = " << std::bitset<8>(*(reinterpret_cast<quint8*>(&_val_1) + 1)) << std::endl;
    std::cout << "reinterpret_cast<quint8*>(&_val_1) + 0 = " << std::bitset<8>(*(reinterpret_cast<quint8*>(&_val_1) + 0)) << std::endl;
    std::cout << "reinterpret_cast<quint8*>(&_val_1) - 1 = " << std::bitset<8>(*(reinterpret_cast<quint8*>(&_val_1) - 1)) << std::endl;
    std::cout << "reinterpret_cast<quint8*>(&_val_1) - 2 = " << std::bitset<8>(*(reinterpret_cast<quint8*>(&_val_1) - 2)) << std::endl;
    std::cout << "c_arr[0] = " << std::bitset<8>(c_arr[0]) << " address = " << (void*)&c_arr[0] << std::endl;
    std::cout << "c_arr[1] = " << std::bitset<8>(c_arr[1]) << " address = " << (void*)&c_arr[1] << std::endl;
    std::cout << "c_arr[2] = " << std::bitset<8>(c_arr[2]) << " address = " << (void*)&c_arr[2] << std::endl;
    std::cout << "c_arr[3] = " << std::bitset<8>(c_arr[3]) << " address = " << (void*)&c_arr[3] << std::endl;
#else
    // write respective test for big endian
#endif
    int _1(void);
}
