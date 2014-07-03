// Copyright [2014] <Copyright Sury>
#include "AgendaService.h"
#include <iostream>
AgendaService::AgendaService() {
  storage_ = Storage::getInstance();
}

AgendaService::~AgendaService() {
  delete storage_;
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
  // std::cout << "agenda service" << std::endl;
  if (!storage_->queryUser([&](User u)->bool{
        //std::cout << u.getName() << "1" << u.getPassword()<< std::endl;
        if (userName == u.getName() && password == u.getPassword()) {
          // std::cout << u.getName() << "2" << u.getPassword()<< std::endl;
          return true;
        } else {
          return false;
        }
      }).empty()) {
    return true;
  } else {
    return false;
  }
}

bool AgendaService::userRegister(std::string userName, std::string password,
                                 std::string email, std::string phone) {
  User newUser(userName, password, email, phone);
  if (storage_->queryUser([&](User u)->bool{
        if (userName == u.getName()){
          return true;
        } else {
          return false;
        }
     }
     ).size() == 0) {
    storage_->createUser(newUser);
    return true;
  } else {
    return false;
  }
}

bool AgendaService::deleteUser(std::string userName,
                               std::string password) {
  int c = 0;
  // std::cout << "AgendaServive::deleteUser" << std::endl;
  c = storage_->deleteUser([&](User u)->bool {
      if (userName == u.getName() && password == u.getPassword()) {
        return true;
      } else {
        return false;
      }
    });
  bool flag1 = (c==1);
  bool flag2 = true;
  // std::cout << "AgendaServive::deleteUser" << std::endl;
  if (flag1) {
    int n = storage_->deleteMeeting([&](Meeting m)->bool {
        if (m.getSponsor() == userName || m.getParticipator() == userName) {
          return true;
        } else {
          return false;
        }
      });
  }
  return flag1 && flag2;
}

std::list<User> AgendaService::listAllUsers(void) {
  return storage_->queryUser([&](User u)->bool {return true;} );
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                                  std::string participator,
                                  std::string startDate, std::string endDate) {
  bool flag1 = false;
  bool flag2 = false;
  Date sDate, eDate;
  sDate = sDate.stringToDate(startDate);
  eDate = eDate.stringToDate(endDate);
  // check participator
  int c;
  c = storage_->queryUser([&](User u)->bool {
      if (u.getName() == participator)
        return true;
      return false;
    }).size();
  // std::cout << "c :::::::::::::::" << c << std::endl;
  flag1 = (c != 0) && sDate.isValid(sDate) && eDate.isValid(eDate);
  // flag2 : check time
  if (flag1) {
    int n;
    n = storage_->queryMeeting([&](Meeting m)->bool {
      if (m.getSponsor() != userName && m.getParticipator() != participator
          && m.getSponsor() != participator && m.getParticipator() != userName)
        return false;
      if ((m.getStartDate() < eDate) &&
          (m.getStartDate() > sDate))
        return true;
      if ((m.getEndDate() < eDate) &&
          (m.getEndDate() > sDate))
        return true;
      if ((m.getEndDate() >= eDate) &&
          (m.getStartDate() <= sDate))
        return true;
      if ((m.getEndDate() <= eDate) &&
          (m.getStartDate() >= sDate))
        return true;
      if (m.getTitle() == title)
        return true;
      // std::cout << "sdate:" << startDate << "   " << endDate << std::endl;
      return false;
      }).size();
    // std::cout << "n:::::" << n << std::endl;
    flag2 = (n == 0);
    if (sDate >= eDate) {
      // std::cout << "sdate:" << startDate << " !!! " << endDate << std::endl;
      flag2 = false;
    }
  }
  if (flag1 && flag2) {
    Meeting newMeeting(userName, participator, sDate, eDate, title);
    // std::cout << "sdate:" << startDate << "   " << endDate << std::endl;
    storage_->createMeeting(newMeeting);
    return true;
  } else {
    return false;
  }
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string title) {
  return storage_->queryMeeting([&](Meeting m)->bool {
      if (m.getSponsor() == userName && m.getTitle() == title)
        return true;
      if (m.getParticipator() == userName && m.getTitle() == title)
        return true;
      return false;
    });
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string startDate,
                                               std::string endDate) {
  Date sDate, eDate;
  sDate = sDate.stringToDate(startDate);
  eDate = eDate.stringToDate(endDate);
  return storage_->queryMeeting([&](Meeting m)->bool {
      if ((m.getSponsor() == userName || m.getParticipator() == userName)
          && ((m.getStartDate() > sDate && m.getStartDate() < eDate)
              || (m.getEndDate() < eDate && m.getEndDate() > sDate))) {
        return true;
      } else {
        return false;
      }
    }
  );
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
  return storage_->queryMeeting([&](Meeting m)->bool {
      if (m.getSponsor() == userName || m.getParticipator() == userName) {
        return true;
      } else {
        return false;
      }
    });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
  return storage_->queryMeeting([&](Meeting m)->bool {
      if (m.getSponsor() == userName) {
        return true;
      } else {
        return false;
      }
    });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(std::string userName) {
  return storage_->queryMeeting([&](Meeting m)->bool {
      if (m.getParticipator() == userName) {
        return true;
      } else {
        return false;
      }
    });
}

bool AgendaService::deleteMeeting(std::string userName, std::string title) {
  int n = 0;
  n = storage_->deleteMeeting([&](Meeting m)->bool {
      if (m.getSponsor() == userName && m.getTitle() == title) {
        return true;
      } else {
        return false;
      }
    });
  if (n == 0) {
    return false;
  } else {
    return true;
  }
}

bool AgendaService::deleteAllMeetings(std::string userName) {
  int n = 0;
  n = storage_->deleteMeeting([&](Meeting m)->bool {
      if (m.getSponsor() == userName) {
        return true;
      } else {
        return false;
      }
    });
  if (n == 0) {
    return false;
  } else {
    return true;
  }
}

void AgendaService::startAgenda(void) {
  storage_ = storage_->getInstance();
}

void AgendaService::quitAgenda(void) {
  storage_->sync();
}
