// Copyright [2014] <Copyright Sury>
#include "AgendaUI.h"

AgendaUI::AgendaUI() {
  // std::cout << "UI constructer" ;
  userName_ = "";
  userPassword_ = "";
}

void AgendaUI::OperationLoop(void) {
  // std::cout << "fuck" << std::endl;
  startAgenda();
  std::string op = "";
  int login = 0;
  for (; op != "q"; ) {
    if (userName_ != "") {
      std::cout << "\nAgenda@" << userName_
                <<" : # ";
    }
    if (userName_ == "") {
      std::cout << "\nAgenda : ~$ ";
    }
    op = getOperation();
    // std::cout << "op :" << op << std::endl;
    // std::cout << "login :" << login << std::endl;
    if (executeOperation(op)) {
      if (login == 0 && op != "l" && op != "r" && op != "q") {
        std::cout << "\n[error] Wrong operation, please try again!\n";
      } else if (op == "l") {
        userLogIn();
        if (userName_ != "") login = 1;
      } else if (op == "r") {
        userRegister();
        if (userName_ != "") login = 1;
      } else if (op == "o") {
        userLogOut();
        login = 0;
      } else if (login == 1 && op == "dc") {
        deleteUser();
      } else if (login == 1 && op == "lu") {
        listAllUsers();
      } else if (login == 1 && op == "cm") {
        createMeeting();
      } else if (login == 1 && op == "la") {
        listAllMeetings();
      } else if (login == 1 && op == "las") {
        listAllSponsorMeetings();
      } else if (login == 1 && op == "lap") {
        listAllParticipateMeetings();
      } else if (login == 1 && op == "qm") {
        queryMeetingByTitle();
      } else if (login == 1 && op == "qt") {
        queryMeetingByTimeInterval();
      } else if (login == 1 && op == "dm") {
        deleteMeetingByTitle();
      } else if (login == 1 && op == "da") {
        deleteAllMeetings();
      }
    } else {
      std::cout << "\n[error] Wrong operation, please try again!!\n";
    }
  }
  quitAgenda();
}

void AgendaUI::startAgenda(void) {
  std::cout <<"// Copyright [2014] <Copyright Sury>\n"
            <<"// Phone : 13824481936(631936)\n"
            <<"// Email : suruoyanmashiro@qq.com\n"
            <<"// QQ : 408334011\n"
            <<"// Any Problem Please Contact Me\n"
            << "\n--------------- Agenda ---------------------"
            << std::endl
            << "Action :" << std::endl
            << "l   - log in Agenda by user name and password"
            << std::endl
            << "r   - register an Agenda account" << std::endl
            << "q   - quit Agenda" << std::endl
            << "----------------------------------------------"
            << std::endl;
  agendaService_.startAgenda();
}

std::string AgendaUI::getOperation() {
  std::string op;
  std::cin >> op;
  return op;
}

bool AgendaUI::executeOperation(std::string op) {
  if (op == "l" || op == "r" || op == "q" || op == "o" || op == "dc"
      || op == "lu" || op == "cm" || op == "la" || op == "las" || op == "lap"
      || op == "qm" || op == "qt" || op == "dm" || op == "da") {
    return true;
  } else {
    return false;
  }
}

void AgendaUI::userLogIn(void) {
  std::string userName, password;
  if (userName_ == "" && userPassword_ == "") {
    std::cout << "\n[log in] [user name] [password]" << std::endl << "[log in] ";
    std::cin >> userName >> password;
  } else {
    userName = userName_;
    password = userPassword_;
  }
  if (!agendaService_.userLogIn(userName, password)) {
    std::cout << "[error] log in fail!\n";
  } else {
    userName_ = userName;
    userPassword_ = password;
    std::cout << "[log in] succeed!" << std::endl
              << "\n--------------- Agenda ---------------" << std::endl
              << "o   - log out Agenda" << std::endl
              << "dc  - delete Agenda account" << std::endl
              << "lu  - list all Agenda user" << std::endl
              << "cm  - create a meeting" << std::endl
              << "la  - list all meetings" << std::endl
              << "las - list all sponsor meetings" << std::endl
              << "lap - list all paticipate meetings" << std::endl
              << "qm  - query meeting by titile" << std::endl
              << "qt  - query meeting by time interval" << std::endl
              << "dm  - delete meeting by title" << std::endl
              << "da  - delete all meetings" << std::endl
              << "--------------------------------------" << std::endl;
  }
}

void AgendaUI::userRegister(void) {
  std::cout << "\n[register] ";
  std::cout << "[user name] [password] [email] [phone]\n";
  std::cout << "[register] ";
  std::string userName, password, email, phone;
  std::cin >> userName >> password >>  email >> phone;
  if (agendaService_.userRegister(userName, password, email, phone)) {
    std::cout << "[register] succeed!" << std::endl;
    userName_ = userName;
    userPassword_ = password;
    std::cout << "\n--------------- Agenda ---------------" << std::endl;
    std::cout << "You Have login as " << userName << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    userLogIn();
  } else {
    std::cout << "[error] register fail! Maybe your [user name] exists!\n";
  }
}

void AgendaUI::quitAgenda(void) {
  agendaService_.quitAgenda();
  std::cout << "\n--------------Quiting Agenda--------------"
            << "\nDate Has Been Saved Successfully!"
            << "\nHave A Nice Day!"
            << "\nBy Sury"
            << "\n------------------------------------------\n";
}

void AgendaUI::userLogOut(void) {
  userName_ = "";
  userPassword_ = "";
  std::cout << "\n--------------- Agenda ---------------"
            << std::endl
            << "Action :" << std::endl
            << "l   - log in Agenda by user name and password"<< std::endl
            << "r   - register an Agenda account" << std::endl
            << "q   - quit Agenda" << std::endl
            << "--------------------------------------" << std::endl;
}

void AgendaUI::deleteUser(void) {
  if (agendaService_.deleteUser(userName_, userPassword_)) {
    userName_ = "";
    userPassword_ = "";
    std::cout << "\n[delete agenda account] succeed!\n" << std::endl;
  } else {
    std::cout << "\n[error] delete agenda account fail!" << std::endl;
  }
}

void AgendaUI::listAllUsers(void) {
  std::list<User> user = agendaService_.listAllUsers();
  std::list<User>::iterator it;
  std::cout << "\n[list all users]\n\n";
  std::cout << "name\t" << "email\t\t" << "phone\n";
  for (it = user.begin(); it != user.end(); ++it) {
    std::cout << it->getName() << "\t"
              << it->getEmail() << "\t\t"
              << it->getPhone() << "\n";
  }
}

void AgendaUI::createMeeting(void) {
  std::cout << "\n[create meeting]" << " [title] [participator]"
            << " [start time(yyyy-mm-dd/hh:mm)]"
            << " [end time(yyyy-mm-dd/hh:mm)]\n"
            << "[create meeting] ";
  std::string title, participator, sponsor, startTimestring, endTimestring;
  std::cin >> title >> participator >> startTimestring >> endTimestring;
  sponsor = userName_;
  if (agendaService_.createMeeting(sponsor, title, participator,
                                   startTimestring,
                                   endTimestring)) {
    std::cout << "[create meeting] succeed!\n";
  } else {
    std::cout << "\n[error] creat meeting fail!\n"
              << "\n[error] maybe you don't have time during this period!\n";
  }
}

void AgendaUI::listAllMeetings(void) {
  std::list<Meeting> meeting = 
    agendaService_.listAllMeetings(userName_);
  std::cout << "\n[list all meeting]\n\n";
  printMeetings(meeting);
}

void AgendaUI::listAllSponsorMeetings(void) {
  std::list<Meeting> meeting =
    agendaService_.listAllSponsorMeetings(userName_);
  std::cout << "\n[list all sponsor meeting]\n\n";
  printMeetings(meeting);
}

void AgendaUI::listAllParticipateMeetings(void) {
  std::list<Meeting> meeting =
    agendaService_.listAllParticipateMeetings(userName_);
  std::cout << "\n[list all paticipate meeting]\n\n";
  printMeetings(meeting);
}

void AgendaUI::queryMeetingByTitle(void) {
  std::cout << "\n[query meeting] [title]:" << std::endl
            << "[query meeting] ";
  std::string title;
  std::cin >> title;
  if (agendaService_.meetingQuery(userName_, title).size() == 1) {
    Meeting meeting = agendaService_.meetingQuery(userName_, title).front();
    std::cout << "\nsponsor\t\t" << "participator\t\t"
              << "start time\t\t" << "end time" << std::endl;
    std::cout << meeting.getSponsor() << "\t\t"
              << meeting.getParticipator() << "\t\t"
              << Date::dateToString(meeting.getStartDate()) << "\t"
              << Date::dateToString(meeting.getEndDate()) << std::endl;
  } else {
    std::cout << "\n[error] Didn't find a meeting matched!" << std::endl;
  }
}

void AgendaUI::queryMeetingByTimeInterval(void) {
  std::cout << "\n[query meeting] ";
  std::cout << "[start time(yyyy-mm-dd/hh:mm)]"
            <<" [end time(yyyy-mm-dd/hh:mm)]\n"
            << "[query meeting] ";
  std::string sTime, eTime;
  std::cin >> sTime >> eTime;
  std::list<Meeting> meeting
    = agendaService_.meetingQuery(userName_, sTime, eTime);
  std::cout << "\n[query meeting]\n\n";
  printMeetings(meeting);
}

void AgendaUI::deleteMeetingByTitle(void) {
  std::cout << "\n[delete meeting] [title]\n";
  std::cout << "[delete meeting] ";
  std::string title;
  std::cin >> title;
  if (agendaService_.deleteMeeting(userName_, title)) {
    std::cout << "\n[delete meeting by title] succeed!\n";
  } else {
    std::cout << "\n[error] delete meeting fail!"
              << "\n[error] maybe this meeting doesn't exsit!\n";
  }
}

void AgendaUI::deleteAllMeetings(void) {
  bool flag = agendaService_.deleteAllMeetings(userName_);
  if (flag == true) {
   std::cout << "\n[delete all meetings] succeed!\n";
  } else {
    std::cout << "\n[error] delete meeting fail!"
              << "\n[error] maybe you don't have any meeting!\n";
  }
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
  std::list<Meeting>::iterator it;
  Date a;
  std::cout << "title       \t" << "sponsor     \t" << "participator\t"
            << "start time  \t\t" << "end time    " << std::endl;
  for (it = meetings.begin(); it != meetings.end(); ++it) {
    std::cout << it->getTitle() << "\t\t"
              << it->getSponsor() << "\t\t"
              << it->getParticipator() << "\t\t"
              << a.dateToString(it->getStartDate()) << "\t"
              << a.dateToString(it->getEndDate()) << std::endl;
  }
}
