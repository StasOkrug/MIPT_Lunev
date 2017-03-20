#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

//Как узнать на один ли файл ссылаются два файловых дескриптора?

/*char check(int a, int b)
{
	struct stat s1, s2;
	if (stat(a, &s1) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	if (stat(b, &s2) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	return (s1.st_ino == s2.st_ino) && (s1.st_dev == s2.st_dev);
}*/

//В одном процессе открываем файл дважды. Оба open отработали успешно. 
//Всегда ли check вернет True?
///Нет, между открытиями можно вызвать mv, rm, mount

//Что будет напечатано? 
/*int main()
{
	char buf[3];
	mkfifo("question_fifo", 666);
	int fd = open("question_fifo", O_RDWR);
	write(fd, "aaa", 3);
	close(fd);
	fd = open("question_fifo", O_RDONLY);
	write(1, buf, read(fd, buf, 3));
	return 0;
}*/
///Создается файл с правами p-w--wx--T (так как пропущен ноль в \
															правах 666)
///Иначе создался бы prw-rw-r-- (0666)
///Первоначальная версия входит в deadlock, так как open блокирующий

//Теперь открываем файл в нонблок режиме
///из рута :после закрытия fifo происходит очищение -> получаем \
														пустую строку
///пользователь : получаем ошибку при попытке открыть O_RDWR так как


//Когда процесс выходит из таблицы процессов?+
///1. Смерть родителя
///2. wait у родителя 
///3. Явное игнорирование SIGCHLD

//Что происходит при SEGMENTFAULT
///Аппаратное исключение MMU in kernel
///Вызов обработчика искл ситуации. Обраб в ядре 
///Переключение в ядро
///Разрешение segmentfault SIGSEGV посылается - процесс умирает
///Восстановление контекста

//Кто печатает SIGFAULT
//Ядро посылает сигнал SIGSEGV
//По умолчанию - смерть
//bash узнает о смерти через wait														

//Что делает процесс при смерти(Какие сигналы посылает?)+
///Посылаемые сигналы:
///SIGPIPE 3 - закрыть пайп
///SIGHUP 2 - убить их всех
///SIGCONT 1 - все процессы в стопе продолжили
///SIGCHLD 4- убить ребенка
///SIGPOLL / SIGIO - апаратная ошибка
///от prctl любой сигнал

//На SIGSEGV установлен пустой обработчик. Что произойдет 
// при разыменовании пустого указателя?
///Будет крутиться на разыменовании, так как приходить аппаратное 
///исключение, которое действует с текущей команды(в отличии от прерывания),
///которое никогда не будет разрешено

void empty(int signum){
}

int main(){
	signal(SIGSEGV, empty);
	int* a = NULL;
	int b = *a;
}