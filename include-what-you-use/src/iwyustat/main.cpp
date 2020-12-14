//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2020 MuseScore BVBA and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

static const std::string QT_INC("#include <Qt");
static const std::string Q_INC("#include <Q");
static const std::string STD_INC("#include <");
static const std::string LOC_INC("#include \"");

bool startsWith(const std::string& str, const std::string& pref) {
    return pref.size() <= str.size() && std::equal(pref.begin(), pref.end(), str.begin());
}

struct Count
{
    int count = 0;
};

using Stat = std::unordered_map<std::string, Count>;

struct Stats
{
    Stat std;
    Stat qt;
    Stat loc;
};

bool readStat(const std::string& path, Stats& stats)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "failed open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {

        if (startsWith(line, QT_INC)) {
            continue;
        } else if (startsWith(line, Q_INC)) {
            stats.qt[line].count++;
        } else if (startsWith(line, STD_INC)) {
            stats.std[line].count++;
        } else if (startsWith(line, LOC_INC)) {
            stats.loc[line].count++;
        } else {
            continue;
        }
    }
    file.close();
    return true;
}

void printStat(const Stat& stat, int maxCount)
{
    struct Inc
    {
        std::string h;
        int count = 0;
    };

    std::vector<Inc> list;
    list.reserve(stat.size());

    for (auto it = stat.cbegin(); it != stat.cend(); ++it) {
        Inc inc;
        inc.h = it->first;
        inc.count = it->second.count;

        if (inc.count >= maxCount) {
            list.push_back(inc);
        }
    }

    std::sort(list.begin(), list.end(), [](const Inc& f, const Inc& s) {
        return f.count > s.count;
    });

    for (const Inc& i : list) {
        std::cout << i.h << " //" << i.count << std::endl;
    }
}

void printStats(const Stats& stats, int maxCount)
{
    std::cout << "// Std includes" << std::endl;
    printStat(stats.std, maxCount);
    std::cout << std::endl;

    std::cout << "// Qt includes" << std::endl;
    printStat(stats.qt, maxCount);
    std::cout << std::endl;

    std::cout << "// Local includes" << std::endl;
    printStat(stats.loc, maxCount);
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "Hello World! I am iwyu stat!" << std::endl;

    std::string headerInfoPath;
    if (argc > 1) {
        headerInfoPath = argv[1];
    }

    if (headerInfoPath.empty()) {
        headerInfoPath = "iwyu_includes.txt";
    }

    Stats stats;
    if (!readStat(headerInfoPath, stats)) {
        return 1;
    }

    printStats(stats, 20);

    return 0;
}
