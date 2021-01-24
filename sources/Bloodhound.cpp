// Copyright 2021 LemuriiL <LemuriiL@yandex.ru>
#include <Bloodhound.hpp>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>

FileCrash::FileCrash(const string &ArgName, const string &ArgDate)
    : name(ArgName), date(ArgDate) {}
string FileCrash::NameTaking() const { return name; }
string FileCrash::DateTaking() const { return date; }

Brokers::Brokers(const string &ArgName, const MapOfFiles &ArgFiles)
    : name(ArgName), accFiles(ArgFiles) {}
string Brokers::NameTaking() const { return name; }
MapOfFiles Brokers::FileTaking() const { return accFiles; }
bool Brokers::Value() const { return !name.empty() && !accFiles.empty(); }

inline bool is_separator(char c) { return c == '_'; }
inline bool not_separator(char c) { return !is_separator(c); }

std::vector<string> StringSlasher(const string &text) {
  std::vector<string> ret;
  for (auto i = text.cbegin(); i != text.cend();) {
    i = std::find_if(i, text.end(), not_separator);
    auto j = std::find_if(i, text.end(), is_separator);
    ret.emplace_back(i, j);
    i = j;
  }
  return ret;
}

string lastDate(const vector<FileCrash> &files) {
  string res;
  for (const auto &file : files) {
    res = std::max(res, file.DateTaking());
  }
  return res;
}

Brain::Brain(const string &path_to_inspect)
    : brokersDirectory(path_to_inspect), brokers() {}

void Brain::takeBrokers() {
  for (const auto &entry :
       boost::filesystem::directory_iterator{brokersDirectory}) {
    auto brok = takeSingleBroker(entry.path());
    if (brok.Value()) brokers.push_back(brok);
  }
}

Brokers Brain::takeSingleBroker(const boost::filesystem::path &path) {
  string name;
  MapOfFiles files;
  if (!boost::filesystem::is_directory(path))
    throw std::runtime_error("Your argument not path to the brokers");

  name = path.filename().string();

  for (const auto &entry : boost::filesystem::directory_iterator{path}) {
    if (boost::filesystem::is_directory(entry) ||
        !entry.path().stem().extension().empty() ||
        entry.path().extension().string() != ".txt")
      continue;

    auto filenameTokens = StringSlasher(entry.path().stem().string());
    if (filenameTokens.size() != TokensCount) continue;

    auto type = filenameTokens[0];
    auto account = filenameTokens[1];
    auto date = filenameTokens[2];

    if (type != FileType) continue;
    if (account.length() != AccountLength) continue;
    if (date.length() != DateLength) continue;

    files[account].emplace_back(entry.path().filename().string(), date);
  }
  return Brokers(name, files);
}

void Brain::Print() {
  std::cout << "total brokers: " << brokers.size() << std::endl;
  for (const auto &Broker : brokers) {
    for (const auto &af_pair : Broker.FileTaking()) {
      std::cout << "broker:" << Broker.NameTaking()
                << " account:" << af_pair.first
                << " files:" << af_pair.second.size()
                << " lastdate:" << lastDate(af_pair.second) << std::endl;
    }
  }

  std::cout << std::endl;

  for (const auto &Broker : brokers) {
    for (const auto &af_pair : Broker.FileTaking()) {
      for (const auto &file : af_pair.second) {
        std::cout << Broker.NameTaking() << " " << file.NameTaking()
                  << std::endl;
      }
    }
  }
}
