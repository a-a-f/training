#include <string>
#include <vector>

// Returns the longest symmetry about a given center,
// starting from an offset from this center.
int getLongestSymmetry(const std::string& in, int center, int offset) {
  int count;
  if (center % 2 == 0 || offset > 0) {
    count = 0;
  } else {
    count = 1;
    offset = 1;
  }

  int pos1 = (center - offset) / 2 - 1;
  int pos2 = (center + offset) / 2;
  while (pos1 >= 0 && pos2 < static_cast<int>(in.length()) && in[pos1] == in[pos2]) {
    count += 2;
    --pos1;
    ++pos2;
  }

  return count;
}

// Returns a palindrome substring of 'in' having the maximum length.
// Complexity: O(n).
std::string getLongestPalindromicSubstring(const std::string& in) {
  if (in.empty()) { return ""; }

  const int centerCount = 2 * in.length() + 1;
  std::vector<int> maxLengths;
  maxLengths.reserve(centerCount);

  int center = 0;
  int maxCenter = -1;
  int offset = 0;
  while (center < centerCount) {
    const int length = offset + getLongestSymmetry(in, center, offset);
    offset = 0;
    maxLengths.push_back(length);
    if (maxCenter == -1 || length > maxLengths[maxCenter]) {
      maxCenter = center;
    }

    if (length == centerCount - center - 1) { 
        // The current palindrome is a suffix of the string 'in'.
        // Thus 'center' is the center of symmetry of the maxLengths array,
        // so that at this stage we have found a longest palindrome.
        break;
    }

    int newCenter = center + 1;
    for (int i = 1; i < length; ++i) {
      if (maxLengths[center - i] == length - i) {
        // We have a left prefix. Thus:
        // maxLengths[center + i] >= maxLengths[center - i]
        offset = length - i;
        newCenter = center + i;
        break;
      }
      // We have a symmetry about 'center'.
      // Thus maxLengths[center + i] == maxLengths[center - i]
      maxLengths.push_back(maxLengths[center - i]);
      newCenter = center + i + 1;
    }
    center = newCenter;
  }

  int begin = maxCenter - maxLengths[maxCenter];
  if (maxCenter % 2 == 0) { ++begin; }
  return in.substr(begin / 2, maxLengths[maxCenter]);
}
