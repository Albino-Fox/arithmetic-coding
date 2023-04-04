#ifndef DECODE_H
#define DECODE_H

#include "bits.h"

//начало декодирования потока символов
void start_decoding(){
    int i;
    value = 0l;
    //ввод битов для заполнения значения кода
    for(i = 1; i <= BITS_IN_REGISTER; i++)
        value = 2 * value + input_bit();
    //сначала рабочий интервал равен исходному
    low = 0l;
    high = TOP_VALUE;
}
//декодирование очередного символа
int decode_symbol(){
    long range = (long)(high - low) + 1; //ширина интервала
    int cum; //накопленная частота
    int symbol; //декодируемый символ
    //нахождение значения накопленной частоты для value
    cum = (int)((((long)(value - low) + 1) * cum_freq[0] - 1) / range);
    //ищем нужный нам символ в модели
    for(symbol = 1; cum_freq[symbol] > cum; symbol++);
    //после нахождения корректируем границы интервала
    high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
    low = low + (range * cum_freq[symbol]) / cum_freq[0];
    //цикл отбрасывания битов
    while(1){
        if(high < HALF){} //если верхняя граница находится ниже середины, расширяем нижнюю
        else if(low >= HALF){ //если нижняя граница находится выше или на середине, расширяем верхнюю половину после вычитания смещения HALF
            value -= HALF;
            low -= HALF;
            high -= HALF;
        }
        else if(low >= FIRST_QTR && high < THIRD_QTR){ //если границы между первой и третью четвертями, расширяем среднюю половину после вычитания FIRST_QTR
            value -= FIRST_QTR;
            low -= FIRST_QTR;
            high -= FIRST_QTR;
        }
        else break; //если условия не выполняются, выходим из цикла
        //увеличиваем масштаб интервала
        low = 2 * low;
        high = 2 * high + 1;
        //добавляем новый бит
        value = 2 * value + input_bit();
    }
    return symbol; //возвращаем индекс символа с нужной вероятности в модели
}
//собственно адаптивное арифметическое декодирование
void decode(const char *infile, const char *outfile){
    int ch, symbol;

    //готовим файлы для чтения и записи в бинарном режиме
    in = fopen(infile, "r+b");
    out = fopen(outfile, "w+b");
    if(in == NULL || out == NULL){//в случае неудачи открытия какого-либо из файлов - прерываем функцию
        printf("Something is wrong with inputted files.\n");
        return;
    }
        
    //подготовления
    start_model(); //инициализируем перекодировочные таблицы
    start_inputting_bits(); //готовимся к побитовому вводу
    start_decoding(); //инициализируем регистры границ и кода перед началом разжатия 

    //цикл обработки символов
    while(1){
        symbol = decode_symbol(); //возвращаем закодированный символ в виде индекса
        if (symbol == EOF_SYMBOL) //если получили индекс символа концы строки, прерываем цикл
            break;
        ch = index_to_char[symbol]; //по индексу символа получаем сам символ
        putc(ch, out); //выводим декодированный символ
        update_model(symbol); //обновляем таблицы перекодировки после декодирования символа
    }
    //закрываем файлы
    fclose(in);
    fclose(out);
}

#endif