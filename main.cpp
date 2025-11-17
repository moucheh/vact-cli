#include <stdlib.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

void printHelp() {
  std::cout << " Help\n"
            << "  Usage: vact [location]\n"
            << "  --help, -h, help\tPrints help and exits\n"
            << "  --locations, -l\tPrints available locations" << std::endl;
}

void printResult(json&& data) {
  const auto& vakat = data["vakat"];
  const auto& datum = data["datum"];
  const auto& lokacija = data["lokacija"];

  const std::string separator = std::string(70, '=');

  if (vakat.size() == 6 && datum.size() == 2)
    std::cout << separator << std::endl
              << "\t\e[1;32mVaktija\e[0;37m\t\t\t" << std::string(lokacija)
              << '\n'
              << std::endl
              << "\t\e[1;32mDatum\e[0;37m\t\t\t" << std::string(datum[1])
              << std::endl
              << "\t\e[1;32mSabah\e[0;37m\t\t\t" << std::string(vakat[0])
              << std::endl
              << "\t\e[1;32mIzlazak sunca\e[0;37m\t\t" << std::string(vakat[1])
              << std::endl
              << "\t\e[1;32mPodne\e[0;37m\t\t\t" << std::string(vakat[2])
              << std::endl
              << "\t\e[1;32mIkindija\e[0;37m\t\t" << std::string(vakat[3])
              << std::endl
              << "\t\e[1;32mAksham\e[0;37m\t\t\t" << std::string(vakat[4])
              << std::endl
              << "\t\e[1;32mJacija\e[0;37m\t\t\t" << std::string(vakat[5])
              << std::endl
              << separator << std::endl;
}

void readLocations(std::map<std::string, int>& locations) {
  fs::path home_dir(getenv("HOME"));
  fs::path locations_path = home_dir / ".local/bin/locations.txt";

  std::ifstream l(locations_path);

  if (l.is_open()) {
    auto counter = 0;
    std::string input;

    while (std::getline(l, input))
      locations[input] = counter++;
  } else {
    std::cout << "Locaitons file not found, exiting!" << std::endl;
    exit(1);
  }

  l.close();
}

void printLocations(const std::map<std::string, int>& locations) {
  std::cout << "All available locations: " << std::endl;
  for (const auto [location, _] : locations)
    std::cout << location << '\n';
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printHelp();
    return 0;
  }

  std::string city_str = argv[1];

  std::transform(city_str.begin(), city_str.end(), city_str.begin(), tolower);

  if (city_str == "--help" || city_str == "-h" || city_str == "help") {
    printHelp();
    return 0;
  }

  std::map<std::string, int> locations;

  readLocations(locations);

  if (city_str == "-l" || city_str == "--locations") {
    printLocations(locations);
    return 0;
  }

  int city = -1;

  if (locations.find(city_str) != locations.end())
    city = locations[city_str];

  if (city == -1) {
    std::cout << "Invalid city entered!" << std::endl;
    return 0;
  }

  const std::string api_url =
      std::format("https://api.vaktija.ba/vaktija/v1/{}", city);

  system(
      (std::format("curl -s {} | jq . > /tmp/vaktija.json", api_url)).c_str()
  );

  sleep(1);

  std::ifstream f("/tmp/vaktija.json");

  if (f.is_open())
    printResult(json::parse(f));
  else
    std::cout << "File not found, exiting!" << std::endl;

  f.close();

  return 0;
}
