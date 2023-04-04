#ifndef ENCODE_H
#define ENCODE_H

#include "bits.h"

//инициализация регистров границ и кода перед началом сжатия 
void start_encoding(){
    low = 0l;
    high = TOP_VALUE;
    bits_to_follow = 0l; //биты пока добавлять не нужно
}
//очистка побитового вывода
void done_encoding(){
    bits_to_follow++;
    //вывод какого-либо из двух битов, определяющих четверть, лежащую в текущем интервале
    if(low < FIRST_QTR)
        output_bit_plus_follow(0);
    else
        output_bit_plus_follow(1);
}
//кодирование символа
void encode_symbol(int symbol){
    long range = (long)(high-low) + 1; //ширина текущего кодового интервала

    //сужение интервала кодов до выделенного для symbol
    high = low + (range * cum_freq[symbol-1]) / cum_freq[0] - 1;
    low = low + (range * cum_freq[symbol]) / cum_freq[0];

    //цикл по выводу битов
    while(1){
        if(high < HALF) //если в нижней половине исходного интенрвала
            output_bit_plus_follow(0); //то вывод 0
        else if(low >= HALF){ //если в верхней
            output_bit_plus_follow(1); //то вывод 1
            //и убирание известной у границ общей части
            low -= HALF;
            high -= HALF;
        }
        else if(low >= FIRST_QTR && high < THIRD_QTR){ //если текущий интервал содержит середину исходного
            bits_to_follow += 1; //то вывод ещё одного обратного бита позже
            //а сейчас убирание общей части
            low -= FIRST_QTR;
            high -= FIRST_QTR;
        }
        else break; //если ни то, ни другое, тогда выходим из цикла
        //расширяем текущий рабочий кодовый интервал
        low = 2 * low;
        high = 2 * high + 1;
    }
}
//собственно адаптивное арифметическое кодирование
void encode(const char *infile, const char *outfile){
    int ch, symbol;

    //готовим файлы для чтения и записи в бинарном режиме
    in = fopen(infile, "r+b");
    out = fopen(outfile, "w+b");
    if(in == NULL || out == NULL){ //в случае неудачи открытия какого-либо из файлов - прерываем функцию
        printf("Something is wrong with inputted files.\n");
        return;
    }

    //подготовления
    start_model(); //инициализируем перекодировочные таблицы
    start_outputting_bits(); //готовимся к побитовому выводу
    start_encoding(); //инициализируем регистры границ и кода перед началом сжатия 

    //цикл обработки символов
    while(1){ 
        ch = getc(in); //чтение исходного символа
        if (ch == EOF) //если встречаем конец файла - прерываем цикл
            break;
        symbol = char_to_index[ch]; //находим рабочий символ
        encode_symbol(symbol); //кодируем его
        update_model(symbol); //обновляем перекодировочные таблицы
    }
    encode_symbol(EOF_SYMBOL); //кодирование EOF
    done_encoding(); //заканчиваем кодировку добавлением бита
    done_outputting_bits(); //заканчиваем запись результатов
    //закрываем рабочие файлы
    fclose(in);
    fclose(out);
}

#endif