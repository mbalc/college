// Task: Given a template of string (chosen from *[\*A-Z]), find minimum length
// // of a text that fits in the template regardless of where it is matched

// Approach: Let's consider the smallest distance [k] between two different
// letters in given template with [n] characters. We notice that there can be no
// solution shorter than [n]-[k]+1 (see *1).  Yet there's a way to build
// expected string of such length (see *2), so this formula represents our
// solution.

// (*1) If letters at positions [i] and [i]+[k] are different, if we fit any
// string shorter than specified above in a leftmost-possible way that [i]-th
// letter is matched, we can also fit this string [k] positions further to the
// right. Thus, there exist one position in that supposed string that should
// contain letters both from [i]-th and [i]+[k]-th positions, which is not
// possible.

// (*2) To construct such string we can fill each * occurrence with the first
// letter that comes after any series of stars that may or may not follow. Then,
// the expected string will consist of first [n]-[k]+1 letters of a template
// prepared this way.

/* Example test:

  IN:
A*B*B*A

  OUT:
6

Explanation based on our approach - 6 = 7 - 2 + 1;
    string of this length can be fitted in two ways, both acceptable

A*B*B*A
  \/       A*B*B*A
ABBBBAA => ABBBBA    fits both ways
|____|    A*B*B*A

*/

#include <algorithm>
#include <climits>
#include <cstdio>

using namespace std;

void read(char *letter, int *length) {
  scanf("%c", letter);
  ++(*length);
}

int main() {
  const char STAR = '*';

  char letter;
  int length = -1;  // we'll read EOF too

  do {
    read(&letter, &length);
  } while (letter == STAR);

  char lastLetter = letter;

  if (letter >= STAR) read(&letter, &length);
  int minDist = INT_MAX, actDist = 1, changes = 0;

  while (letter >= STAR) {
    if (letter == STAR || letter == lastLetter) {
      ++actDist;
      if (letter == lastLetter) actDist = 1;
    } else {
      minDist = min(minDist, actDist);
      actDist = 1;
      lastLetter = letter;
      ++changes;
    }
    read(&letter, &length);
  }

  if (changes == 0) minDist = length;  // not more than one letter existent

  printf("%d\n", length - minDist + 1);

  return 0;
}
