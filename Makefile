All: User.o Date.o Meeting.o Storage.o AgendaService.o AgendaUI.o
User.o : User.h User.cpp
	g++ -std=c++0x -c User.cpp
Date.o : Date.h Date.cpp
	g++ -std=c++0x -c Date.cpp
Meeting.o : Meeting.h Meeting.cpp
	g++ -std=c++0x -c Meeting.cpp
Storage.o : Storage.h Storage.cpp
	g++ -std=c++0x -c Storage.cpp
AgendaService.o : AgendaService.h AgendaService.cpp
	g++ -std=c++0x -c AgendaService.cpp
AgendaUI.o : AgendaUI.h AgendaUI.cpp
	g++ -std=c++0x -c AgendaUI.cpp

start :
	g++ -std=c++0x -o a.out Agenda.cpp *.o
run : 
	./a.out
clean :
	rm *.o *.cpp~ *.h~
cleandata :
	rm *.data *.data~ 