/*
 * StringAlgorithms.h
 *
 *  Created on: 25/05/2016
 *      Author: Asus
 */

#ifndef SRC_STRINGALGORITHMS_H_
#define SRC_STRINGALGORITHMS_H_

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

vector<int> computePrefixSum(string p) {

	int m = p.size();
	vector<int> pi(m);

	pi[0] = -1;
	int k = -1;
	for (int q = 1; q < m; q++) {
		while (k > -1 && p[k + 1] != p[q]) {
			k = pi[k];
		}
		if (p[k + 1] == p[q])
			k++;
		pi[q] = k;
	}

	return pi;
}

int kmp_matcher(string t, string p) {
	int count = 0;
	int n = t.size();
	int m = p.size();
	vector<int> pi = computePrefixSum(p);
	int q = -1;
	for (int i = 0; i < n; i++) {
		while (q > -1 && p[q + 1] != t[i]) {
			q = pi[q];
		}
		if (p[q + 1] == t[i]) {
			q++;
		}
		if (q == m - 1) {
			count++;
			q = pi[q];
		}
	}
	return count;
}

int numStringMatching(string filename, string toSearch) {
	int count = 0;
	ifstream ifs;
	ifs.open(filename.c_str(), std::ifstream::in);
	string p = "";
	char c = ifs.get();
	while (!ifs.eof()) {
		if (c == '\n') {
			count += kmp_matcher(p, toSearch);
			p = "";
		} else {
			p += c;
		}
		c = ifs.get();
	}
	return count;
}

int editDistance(string p, string t) {
	vector<vector<int> > dp(p.size() + 1, vector<int>(t.size() + 1));
	for (size_t i = 0; i <= p.size(); i++) {
		dp[i][0] = i;
	}
	for (size_t i = 0; i <= t.size(); i++) {
		dp[0][i] = i;
	}

	for (size_t i = 1; i <= p.size(); i++) {
		for (size_t j = 1; j <= t.size(); j++) {
			if (p[i - 1] == t[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1];
			} else {
				dp[i][j] = 1
						+ min(min(dp[i - 1][j], dp[i][j - 1]),
								dp[i - 1][j - 1]);
			}
		}
	}
	return dp[p.size()][t.size()];
}

float numApproximateStringMatching(string filename, string toSearch) {
	int count = 0;
	int sumEdit = 0;
	ifstream ifs;
	ifs.open(filename.c_str(), std::ifstream::in);
	string p = "";

	while (!ifs.eof()) {
		ifs >> p;
		count++;
		sumEdit += editDistance(p, toSearch);
	}
	return (float) sumEdit / count;
}

#endif /* SRC_STRINGALGORITHMS_H_ */
