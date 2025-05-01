#include "pch.h"
#include "Util.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


string Util::toUpper(string s) {
    string result(s);
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return result;
}


double Util::StringEquality(const string& originalCased, const string& otherCased) {
    auto original = toUpper(originalCased);
    auto other = toUpper(otherCased);

    if (original.find(other) != string::npos || other.find(original) != string::npos) return 1;

    size_t len1 = original.size();
    size_t len2 = other.size();

    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            int cost = (original[i - 1] == other[j - 1]) ? 0 : 1;
            dp[i][j] = min(dp[i - 1][j] + 1,                // deletion
                           min(dp[i][j - 1] + 1,            // insertion
                               dp[i - 1][j - 1] + cost));   // substitution
        }
    }

    double absDistance(dp[len1][len2]);
    double matchPercentage = max(0.0, 1 - (absDistance / len1));

    return matchPercentage;
}

