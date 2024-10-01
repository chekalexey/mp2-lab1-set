// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <bitset>
#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw "Wrong TBitfield size";
    if (len == 0) return;

    BitLen = len;
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) throw "index out of range";

    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) throw "index out of range";

    return static_cast<TELEM>(1) << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw "index out of range";

    pMem[n / (sizeof(TELEM) * 8)] |= GetMemMask(n % (sizeof(TELEM) * 8));
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw "index out of range";

    pMem[n / (sizeof(TELEM) * 8)] &= ~GetMemMask(n % (sizeof(TELEM) * 8));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw "index out of range";

    if (pMem[GetMemIndex(n)] & GetMemMask(n % (sizeof(TELEM) * 8)))
        return 1;
    else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if(this == &bf)
        return *this;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if(BitLen != bf.BitLen) return 0;

    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
     
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
  return ~(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField res(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField res(std::max(BitLen,bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];
    
    int LastBit = BitLen % (sizeof(TELEM) * 8);

    if (LastBit)
        res.pMem[MemLen - 1] &= ((static_cast<TELEM>(1) << LastBit) - 1);

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        char bit;
        cin >> bit;

        if (bit == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        cout << bf.GetBit(i);

    return ostr;
}