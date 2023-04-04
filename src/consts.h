#ifndef CONSTS_H
#define CONSTS_H

//берём количество битов в реестре, 16 для большего быстродействия
#define BITS_IN_REGISTER 16

//здесь же максимально возможное число в реестре
#define TOP_VALUE (((long)1 << BITS_IN_REGISTER) - 1)

//диапазоны числовых отрезков
#define FIRST_QTR (TOP_VALUE / 4 + 1) //первая четверть
#define HALF (2 * FIRST_QTR) //половина
#define THIRD_QTR (3 * FIRST_QTR) //третья четверть

#define NO_OF_CHARS 256 //кол-во символов алфавита
#define EOF_SYMBOL (NO_OF_CHARS + 1) //специальный символ конца файла
#define NO_OF_SYMBOLS (NO_OF_CHARS + 1) //всего символов в модели

#define MAX_FREQUENCY 16383 //(2^14 - 1) | порог частоты, после которого требуется масштабирование

#endif