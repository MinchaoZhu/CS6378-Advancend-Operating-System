#include "util/test_interval.h"
#include "util/config_parser.h"
#include "util/string_helper.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

TestInterval::TestInterval(){}


void TestInterval::Test() {
    int n = config.nodesNumber;
    int numEach = config.requestSum;

    vector<vector<pair<int,int>>> intervalsArr(n, vector<pair<int,int>>());

    for(int i = 0; i < n; ++i) {
        string line;
        string path = "out"+to_string(i)+".txt";
        ifstream ifs(path);
        vector<pair<int,int>> intervals;
        for (unsigned long j = 0; std::getline(ifs, line); ++j) {
            auto tmp = split(line," ");
            intervals.emplace_back(stoi(tmp[0]), stoi(tmp[1]));
        }
        intervalsArr[i] = intervals;

        if (intervals.size() != numEach) {
            cout << "Fail: " << "export records of node " << i << "expected " << numEach << ", actual: " << intervals.size() << endl;
            return;
        }
    }

    vector<pair<int,int>> intervals;
    for(int i = 0; i < n; ++i) {
        intervals.insert(intervals.end(), intervalsArr[i].begin(), intervalsArr[i].end());
    }

    auto cmp = [](pair<int,int> left, pair<int,int> right){return left.first < right.first;};
    sort(intervals.begin(),intervals.end(), cmp);

    pair<int,int> pre = {0, 0};
    for(auto cur : intervals) {
        if(cur.first < pre.second) {
            cout << "Fail: " << "overlaped intervals: " << "{"<<pre.first<<","<<pre.second<<"} "<<"{"<<cur.first<<","<<cur.second<<"} "<< endl;
            return;
        }
        pre = cur;
    }

    cout << "Pass." << endl;
}