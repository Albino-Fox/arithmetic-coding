#include <iostream>
#include <tchar.h>

#include "algo.h"
#include "../tests/test.cpp"

//головная процедура
int main(int argc, char* argv[] ){ //работа с возможным wide character 
    std::string flags[] = {"-test", "-e", "-d"};
    if(argc == 3 && argv[1] == flags[0]){ //тестирование
        testIt(argv[2]);
        return 0;
    } else
    if(argc < 4){ //проверка на количество поступивших аргументов с запуском программы
        printf("\nUsing: cur_program_name -e|d <infile> <outfile>\n"); //подсказка о том, как пользоваться программой
        //ключи: e - encrypt; d - decrypt; | infile - наименование/путь к исходному файлу; outfile - наименование/путь для изменённого файла 
        return 0; //остановка программы (для повторного запуска пользователем, если ещё нужно)
    }
    if(argv[1] == flags[1]) //если первый аргумент -e
        encode (argv[2], argv[3]); // запускаем функцию кодирования с аргументами в виде обозначений исходного и изменённого файлов
    else if (argv[1] == flags[2]) //если первый аргумент -d 
        decode (argv[2], argv[3]); // запускаем функцию декодирования с аргументами в виде обозначений исходного и изменённого файлов

    return 0; //конец головной процедуры
}