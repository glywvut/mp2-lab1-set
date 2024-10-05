// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "cmath"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw "Incorrect lenght";
    else if (len == 0) {
        BitLen = 0;
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        this->BitLen = len;
        this->MemLen = static_cast<int>(((len - 1) / (8 * sizeof(TELEM))) + 1);
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования 
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[]pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n 
{
    if (n < 0 || n >= BitLen) throw "Incorrect bit(1)";
    else return  static_cast<int>(n / (sizeof(TELEM) * 8));

}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n оки  осознать
{
    if (n < 0 || n >= BitLen) throw "Incorrect bit(2)";
    else return static_cast<TELEM>(1 << n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen)) throw "Incorrect bit(3)";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) throw "Incorrect bit(4)";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) throw "Incorrect bit(3)";
    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    int res = pMem[i] & mask;
    return res;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        this->MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    this->BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    int tmp = 1;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            tmp = 0;
            break;
        }
    }
    return tmp;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 1;
    int tmp = 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            tmp = 1;
            break;
        }
    }
    return tmp;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField tmp(0);
    TBitField bitf(0);
    if (BitLen <= bf.BitLen) {
        tmp = bf;
        bitf = *this;
    }
    else
    {
        tmp = *this;
        bitf = bf;
    }
    for (int i = 0; i < bitf.MemLen; i++) {
        tmp.pMem[i] |= bitf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField tmp(0);
    TBitField bitf(0);
    if (BitLen <= bf.BitLen) {
        tmp = bf;
        bitf = *this;
    }
    else
    {
        tmp = *this;
        bitf = bf;
    }
    for (int i = 0; i < bitf.MemLen; i++) {
        tmp.pMem[i] &= bitf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
    for (int i = 0; i < GetLength(); i++) {
        if (tmp.GetBit(i) == 0)
            tmp.SetBit(i);
        else
            tmp.ClrBit(i);
    }
    return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int tmp;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> tmp;
        if (tmp)
        {
            bf.SetBit(i);
        }
        else
        {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (!bf.GetBit(i))
        {
            ostr << 1;
            continue;
        }
        ostr << 0;
    }
    return ostr;
}



