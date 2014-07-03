// Copyright [2014] <Copyright Sury>
#include "Storage.h"
#include <cstdio>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

Storage* Storage::instance_ = NULL;

Storage* Storage::getInstance() {
  if (instance_ == NULL)
    instance_ = new Storage();
  return instance_;
}

Storage::Storage() {
  readFromFile("agenda.data");
}

Storage::~Storage() {
  writeToFile("agenda.data");
  instance_ = NULL;
}

bool Storage::sync(void) {
  return writeToFile("agenda.data");
}

bool Storage::readFromFile(const char *fpath) {
  // std::cout << "READ FROM FILE!!" << std::endl;
  std::FILE *fp;
  int count;
  std::string name, password, email, phone;
  std::string sponsor, participator, sdate, edate, title;
  Date sDate, eDate;
  fp = fopen("agenda.data", "r");
  if (fp == NULL) {
    // std::cout << "ERROR OPEN FILE!!" << std::endl;
    return false;
  } else {
    // std::cout << "SUCCESS OPEN FILE!!" << std::endl;
    fscanf(fp, "{collection:\"User\",total:%d}", &count);
    for (int i = 0; i < count; i++) {
      char attr1[20], attr2[20], attr3[20], attr4[20];
      char value1[100], value2[100], value3[100], value4[100];
      fscanf(fp,"\n{%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\"}", attr1, value1, attr2, value2, attr3, value3, attr4, value4);
      std::string a[4];
      a[0] = attr1;
      a[1] = attr2;
      a[2] = attr3;
      a[3] = attr4;
      std::string v[4];
      v[0] = value1;
      v[1] = value2;
      v[2] = value3;
      v[3] = value4;
      for (int i = 0; i < 4; i++) {
        if (a[i] == "name") name = v[i];
        else if (a[i] == "password") password = v[i];
        else if (a[i] == "email") email = v[i];
        else if (a[i] == "phone") phone = v[i];
      }
      User newUser(name, password, email, phone);
      // std::cout << name << " add to list" << std::endl;
      userList_.push_back(newUser);
    }
    // std::cout << "END USER BEGIN MEETING" << std::endl;
    fscanf(fp,"\n{collection:\"Meeting\",total:%d}",&count);
    for (int i = 0; i < count; i++) {
      char attr5[20], attr6[20], attr7[20], attr8[20], attr9[20];
      char value5[100], value6[100], value7[100], value8[100], value9[100];
      fscanf(fp,"\n{%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\",%[^:]:\"%[^\"]\"}", attr5, value5, attr6, value6, attr7, value7, attr8, value8, attr9, value9);
      std::string a1[5];
      a1[0] = attr5;
      a1[1] = attr6;
      a1[2] = attr7;
      a1[3] = attr8;
      a1[4] = attr9;
      std::string v1[5];
      v1[0] = value5;
      v1[1] = value6;
      v1[2] = value7;
      v1[3] = value8;
      v1[4] = value9;
      // std::cout << "attr9: " << attr9 << " value9 : " << value9 << std::endl;
      // std::cout << "attr8: " << attr8 << " value8 : " << value8 << std::endl;
      // std::cout << "attr7: " << attr7 << " value7 : " << value7 << std::endl;
      // std::cout << "attr6: " << attr6 << " value6 : " << value6 << std::endl;
      for (int i = 0; i < 5; i++) {
        if (a1[i] == "sponsor") sponsor = v1[i];
        else if (a1[i] == "participator") participator = v1[i];
        else if (a1[i] == "sdate") sdate = v1[i];
        else if (a1[i] == "edate") edate = v1[i];
        else if (a1[i] == "title") title = v1[i];
      }
      sDate = sDate.stringToDate(sdate);
      eDate = eDate.stringToDate(edate);
      Meeting newMeeting(sponsor, participator, sDate, eDate, title);
      meetingList_.push_back(newMeeting);
    }
  }
  fclose(fp);
}

bool Storage::writeToFile(const char *fpath) {
  std::ofstream out("agenda.data", std::ios_base::out);
  int n1 = 0, n2 = 0;
  if (out.is_open()) {
    n1 = userList_.size();
    n2 = meetingList_.size();
    std::list<User>::iterator users_iterator;
    std::list<Meeting>::iterator meetings_iterator;
    std::string name, password, email, phone;
    std::string sponsor, participator, title;
    Date sdate, edate;
    out << "{collection:\"User\",total:" << n1 << "}" << std::endl;
    for (users_iterator = userList_.begin();
         users_iterator != userList_.end();
         ++users_iterator) {
      name = users_iterator->getName();
      password = users_iterator->getPassword();
      email = users_iterator->getEmail();
      phone = users_iterator->getPhone();
      out << "{name:\"" << name
          << "\",password:\"" << password
          << "\",email:\"" << email
          << "\",phone:\"" << phone << "\"}" << std::endl;
    }
    out << "{collection:\"Meeting\",total:" << n2 << "}" << std::endl;
    for (meetings_iterator = meetingList_.begin();
         meetings_iterator != meetingList_.end();
         ++meetings_iterator) {
      sponsor = meetings_iterator->getSponsor();
      participator = meetings_iterator->getParticipator();
      sdate = meetings_iterator->getStartDate();
      edate = meetings_iterator->getEndDate();
      title = meetings_iterator->getTitle();
      out << "{sponsor:\"" << sponsor << "\",participator:\""
          << participator << "\",sdate:\""
          << sdate.dateToString(sdate) << "\",edate:\""
          << edate.dateToString(edate) << "\",title:\""
          << title << "\"}" << std::endl;
    }
    return true;
  } else {
    return false;
  }
}

void Storage::createUser(const User &users) {
  userList_.push_back(users);
}

std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
  // std::cout << "query user" << std::endl;
  std::list<User>::iterator userList_iterator;
  std::list<User> newUserList;
  for (userList_iterator = userList_.begin();
       userList_iterator != userList_.end();
       ++userList_iterator) {
    // std::cout << userList_iterator->getName() << std::endl;
    if (filter(*userList_iterator)) {
      newUserList.push_back(*userList_iterator);
    }
  }
  // std::cout << "newUserList.size " << newUserList.size() <<std::endl;
  return newUserList;
}

int Storage::updateUser(std::function<bool(const User&)> filter,
                        std::function<void(User&)> switcher) {
  std::list<User>::iterator userList_iterator;
  int count = 0;
  for (userList_iterator = userList_.begin();
       userList_iterator != userList_.end();
       ++userList_iterator) {
    if (filter(*userList_iterator)) {
      switcher(*userList_iterator);
      count++;
    }
  }
  return count;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
  std::list<User>::iterator userList_iterator;
  int count = 0;
  // std::cout << "Storage::deleteUser" << std::endl;
  for (userList_iterator = userList_.begin();
       userList_iterator != userList_.end();) {
    // std::cout << "Storage::deleteUser in for " << std::endl;
    if (filter(*userList_iterator)) {
      // std::cout << "Storage::deleteUser in if" << std::endl;
      // std::cout << "size: " << userList_.size() << std::endl;
      userList_.erase(userList_iterator++);
      count++;
    } else {
      userList_iterator++;
    }
    // std::cout << "Storage::deleteUser count "<< count << std::endl;
  } 
  // std::cout << "Storage::deleteUser ret" << std::endl;
  return count;
}

void Storage::createMeeting(const Meeting& meetings) {
  meetingList_.push_back(meetings);
}

std::list<Meeting> Storage::queryMeeting(
  std::function<bool(const Meeting&)> filter) {
  std::list<Meeting>::iterator meetingList_iterator;
  std::list<Meeting> newMeetingList;
  for (meetingList_iterator = meetingList_.begin();
       meetingList_iterator != meetingList_.end();
       ++meetingList_iterator) {
    if (filter(*meetingList_iterator)) {
      newMeetingList.push_back(*meetingList_iterator);
    }
  }
  return newMeetingList;
 }

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
                           std::function<void(Meeting&)> switcher) {
  std::list<Meeting>::iterator meetingList_iterator;
  int count = 0;
  for (meetingList_iterator = meetingList_.begin();
       meetingList_iterator != meetingList_.end();
       ++meetingList_iterator) {
    if (filter(*meetingList_iterator)) {
      switcher(*meetingList_iterator);
      count++;
    }
  }
  return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
  std::list<Meeting>::iterator meetingList_iterator;
  int count = 0;
  for (meetingList_iterator = meetingList_.begin();
       meetingList_iterator != meetingList_.end();) {
    if (filter(*meetingList_iterator)) {
      meetingList_.erase(meetingList_iterator++);
      count++;
    } else {
      meetingList_iterator++;
    }
  }
  return count;
}
