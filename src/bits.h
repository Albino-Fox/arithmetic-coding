#ifndef BITS_H
#define BITS_H

#include "stdio.h"
#include "stdlib.h"
#include "consts.h"
#include "model.h"

FILE *in, *out; //указатели: in для файла, над которым будем проводить преобразования, out для файла, в который запишем результат

//инициализируем побитовый ввод (вначале буфер пуст)
void start_inputting_bits(){
    bits_to_go = 0;
    garbage_bits = 0;
}

//ввод очередного бита сжатой информации
int input_bit(){
    int t;
    if (bits_to_go == 0){ //если буфер пуст, читаем байт
        buffer = getc(in);
        if(buffer == EOF){ //если буфер встретил конец файла
            garbage_bits += 1; //вносим "мусорный"(любой) бит
            //если "мусора" слишком много, выбрасываем ошибку
            if(garbage_bits > BITS_IN_REGISTER - 2)
            {
                printf("Error in compressed file\n");
                exit(-1);
            }
        }
        bits_to_go = 8; //если не конец файла, готовимся записывать следующие биты в буфер
    }
    //выдача очередного бита с правого конца(дна) буффера
    t = buffer & 1;
    buffer >>= 1;
    bits_to_go--;
    return t; //возвращаем выданный бит
}

//инициализация побитового вывода
void start_outputting_bits(){
    buffer = 0;
    bits_to_go = 8; //готовимся записывать следующие биты в буфер
}
//вывод очередного бита сжатой информации
void output_bit(int bit){
    buffer >>= 1; //бит идёт в начало буффера
    if(bit) buffer |= 0x80; //0x80 = 128 | если бит ненулевой, режем буфер
    bits_to_go -= 1; //теперь надо будет записывать на один бит меньше
    if (bits_to_go == 0){ //если больше не ожидаем биты на запись,
        putc(buffer, out); //выводим буфер в файл для результата 
        bits_to_go = 8; //и снова готовимся записывать следующие биты в буфер
    }
}
//"вымывание" последних битов из буфера побитового вывода
void done_outputting_bits(){
    putc(buffer >> bits_to_go, out);
}
//ввод указанного бита и отложенных ранее, которые обратны указанному
void output_bit_plus_follow(int bit){
    output_bit(bit); //выводим бит
    while(bits_to_follow > 0){ //пока есть биты для вывода
        //выводим бит с обратным значением и убираем из очереди
        output_bit(!bit);
        bits_to_follow--;
    }
}

#endif