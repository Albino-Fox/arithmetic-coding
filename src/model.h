#ifndef MODEL_H
#define MODEL_H

#include "consts.h"

//таблицы перекодировки
unsigned char index_to_char[NO_OF_SYMBOLS]; //массив для подготовки перехода индексов в символ
int char_to_index[NO_OF_CHARS]; //массив для подготовки перехода символов в индексы

//таблицы частот
int cum_freq[NO_OF_SYMBOLS+1]; //массив накопления частот
int freq[NO_OF_SYMBOLS+1]; //массив частот символов

long low, high, value; //регистры границ и кода
//поддержка побитовых операций с файлами
long bits_to_follow; //количество битов, выводимых после следующего бита с обpатным ему значением
int buffer; //буфер
int bits_to_go; //сколько битов в буфере
int garbage_bits; //кол-во битов после конца файла (чтобы нормально закончить кодировку)


//инициализация адаптивной модели
void start_model(){
    int i;
    //установка таблиц перекодировки типов
    for(int i = 0; i < NO_OF_CHARS; i++){ 
        char_to_index[i] = i+1;
        index_to_char[i+1] = i;
    }
    //установка счётчиков накопленных частот
    for(i = 0; i <= NO_OF_SYMBOLS; i++){
        freq[i] = 1;
        cum_freq[i] = NO_OF_SYMBOLS - i;
    }
    freq[0] = 0; //начинаем с нуля
}

// обновление модели очередным символом
void update_model(int symbol){
    int i; //итератор
    int ch_i; //индекс символа
    int ch_symbol; //сам символ
    int cum; //накопленная частота
    //если счётчики частот достигли своего максимума 
    if(cum_freq[0] == MAX_FREQUENCY){
        cum = 0;
        //делим их все пополам, не приводя к нулю (масштабируем)
        for(i = NO_OF_SYMBOLS; i >= 0; i--){
            freq[i] = (freq[i] + 1) / 2;
            cum_freq[i] = cum;
            cum += freq[i];
        }
    }
    //обновление перекодировочных таблиц в случае перемещения символа
    for(i = symbol; freq[i] == freq[i-1]; i--);
    if(i < symbol){
        ch_i = index_to_char[i]; //получаем индекс символа
        ch_symbol = index_to_char[symbol]; //получаем сам символ
        //меняем местами символ с индексом в массиве для подготовки перехода индексов в символ
        index_to_char[i] = ch_symbol;
        index_to_char[symbol] = ch_i;
        //меняем местами индекс с символом в массиве для подготовки перехода символов в индекс
        char_to_index[ch_i] = symbol;
        char_to_index[ch_symbol] = i;
    }

    //увеличение значения счётчика частоты для символа
    freq[i]++;
    // и обновление накопленных частот
    while(i > 0){
        i--;
        cum_freq[i]++;
    }
}

#endif