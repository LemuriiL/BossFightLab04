// Copyright 2021 LemuriiL <LemuriiL@yandex.ru>
#ifndef INCLUDE_BLOODHOUND_HPP_
#define INCLUDE_BLOODHOUND_HPP_

#include <boost/filesystem.hpp>
#include <map>
#include <string>
#include <vector>

using std::string;
using std::vector;

const int AccountLength = 8;
const int DateLength = 8;
const int TokensCount = 3;
const char FileType[] = "balance";

class FileCrash {
 public:
  FileCrash(const string& name, const string& date);

  string NameTaking() const;
  string DateTaking() const;

 private:
  string name;
  string date;
};

typedef std::map<string, vector<FileCrash>> MapOfFiles;

class Brokers {
 public:
  Brokers(const string& name, const MapOfFiles& files);
  string NameTaking() const;
  MapOfFiles FileTaking() const;
  bool Value() const;

 private:
  string name;
  MapOfFiles accFiles;
};

class Brain {
 public:
  Brain(const string& PathToInspect);
  void takeBrokers();
  void Print();

 private:
  string brokersDirectory;
  vector<Brokers> brokers;
  Brokers takeSingleBroker(const boost::filesystem::path& path);
};

#endif  // INCLUDE_INSPECTOR_HPP_
