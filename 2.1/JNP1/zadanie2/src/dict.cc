#include "dict.h"
#include "dictglobal.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <limits>
#include <unordered_map>


/* Compilation flag -DNDEBUG disables debug logging */
#ifndef NDEBUG
/* Main debug printer macro; prints message along with details regarding message
 * origin */
#define DEBUG(...)                                                             \
  do {                                                                         \
    dict_debug(__VA_ARGS__, " [{", __FUNCTION__, ", ", __FILE__, ":",          \
               __LINE__, "}]");                                                \
  } while (0)

#else
#define DEBUG(...) ((void)0)
#endif // NDEBUG


namespace {
using identifier_t = unsigned long;
using dictionary_t = std::unordered_map<std::string, std::string>;
/* Type of the main collection of dictionaries we use throughout the whole
 * implementation */
using dict_collection_t = std::unordered_map<identifier_t, dictionary_t>;


/* Dictionary collection getter - avoiding "the static init order fiasco" */
dict_collection_t &dictionaries() {
  static dict_collection_t *dictionaries = new dict_collection_t();
  return *dictionaries;
}

/* Returns whether our collection has a dictionary declared with a given id */
bool dict_exists(identifier_t id) {
  return dictionaries().count(id) != 0;
}

/* Checks if a given [key] is written in dictionary with given [id] */
bool dict_has_key(identifier_t id, const char *key) {
  if (key == NULL || !dict_exists(id)) {
    return false;
  }
  return dictionaries()[id].count(key) != 0;
}

/* Prints all arguments on standard error */
inline void print_debug() {
  std::cerr << "\n"; // there are no arguments left to print
}
template <typename T, typename... R> void print_debug(T first, R... args) {
  std::cerr << first;
  print_debug(args...);
}

/* Returns debug message relevant prefix according to its enumerated [type] */
inline std::string get_debug_prefix(char &type) {
  switch (type) {
    case 'C':
      return "Called "; // elemental jnp1:: namespace function call
    case '.':
      return ".      "; // elemental step taken during computations
    case '+':
      return "+      "; // successful execution
    case '=':
      return "=      "; // successful computation
    case '/':
      return "±      "; // successful execution that returns some important
                        // value
    case '!':
      return "!      "; // failure, proceeding would break global dictionary
    case 'W':
      return "W      "; // other warning
    default:
      return "       "; // undefined type
  };
}

/* Unified dictionary name for debug logger */
inline std::string dict_log_name(identifier_t id) {
  if (!dict_exists(id)) {
    return "BLANK";
  }
  if (id == jnp1::dict_global()) {
    return "THE_GLOBAL_DICTIONARY";
  }
  return "DICT_" + std::to_string(id);
}

/* Unified string object printing format used by debug logger */
inline std::string parse_debug_string(const char *str) {
  if (str == NULL) {
    return "NULL";
  }
  return "\"" + (std::string)str + "\"";
}

/* Prints a debug message of enumerated [type] and contents of [args...] */
template <typename... T> void dict_debug(char type, T... args) {
  std::cerr << "DEBUG:  | ";
  std::cerr << get_debug_prefix(type);
  print_debug(args...);
}

/* Get value stored under [key] from dictionary with a given [id] if it
 * exists, or [NULL] otherwise */
const char *dict_try_find(identifier_t id, const char *key) {
  if (!dict_exists(id) || !dict_has_key(id, key)) {
    DEBUG('W', "(key ", parse_debug_string(key), " not found in ",
          dict_log_name(id), ")");
    return NULL;
  }
  const char *out = dictionaries()[id][key].c_str();
  DEBUG('=', dict_log_name(id), "[", parse_debug_string(key), "] == ",
        parse_debug_string(out));
  return out;
}

/* Tells how many keys have any values defined in both dictionaries */
size_t get_common_quantity(identifier_t a_id, identifier_t b_id) {
  if (jnp1::dict_size(a_id) > jnp1::dict_size(b_id))
    return get_common_quantity(b_id, a_id);

  size_t sum = 0;
  for (const std::pair<std::string, std::string> &key : dictionaries()[a_id]) {
    if (dict_has_key(b_id, (key.first).c_str()))
      ++sum;
  }
  return sum;
}
} /* namespace */


namespace jnp1 {
/* Creates a new dictionary entry in our collection with the first id that is
 * found not to be occupied by any other dictionary */
identifier_t dict_new() {
  DEBUG('C', __FUNCTION__, "()");
  static identifier_t DICT_COUNTER;

  size_t size_limit = std::numeric_limits<size_t>::max();
  if (dictionaries().size() == size_limit) {
    DEBUG('W', "Aborting creation attempt, there are already ", size_limit,
          " dictionaries defined, isn't that quite a number?");
    return 0;
  }

  while (dict_exists(DICT_COUNTER)) {
    // this code may be called in case DICT_COUNTER overflows
    DEBUG('.', dict_log_name(DICT_COUNTER), " with id=", DICT_COUNTER,
          " already exists, skipping...");
    DICT_COUNTER++;
  }
  dictionary_t new_dict;
  dictionaries()[DICT_COUNTER] = new_dict;
  DEBUG('/', "New dictionary set up with id=", DICT_COUNTER, "!");
  return DICT_COUNTER++;
}

void dict_delete(identifier_t id) {
  DEBUG('C', __FUNCTION__, "(", id, ")");
  if (!dict_exists(id)) {
    DEBUG('W', "Aborting delete, no such dictionary exists!");
  }
  else if (id == jnp1::dict_global()) {
    DEBUG('!', "Aborting delete, you can't delete ", dict_log_name(id), "!");
  }
  else {
    DEBUG('+', "Deleting ", dict_log_name(id), " in all its glory...");
    dictionaries().erase(id);
  }
}

size_t dict_size(identifier_t id) {
  DEBUG('C', __FUNCTION__, "(", id, ")");
  if (!dict_exists(id)) {
    DEBUG('W', "No such dictionary exists, returning (0)...");
    return 0;
  }
  size_t out = dictionaries()[id].size();
  DEBUG('=', __FUNCTION__, "(", id, ")=", out);
  return out;
}

void dict_insert(identifier_t id, const char *key, const char *value) {
  DEBUG('C', __FUNCTION__, "(", id, ", (", parse_debug_string(key), "), (",
        parse_debug_string(value), "))");
  identifier_t global_id = jnp1::dict_global();
  if (!dict_exists(id) || key == NULL || value == NULL) {
    DEBUG('W', "Aborting insert, criteria not met...");
  }
  else if (id == global_id && dict_size(global_id) >= MAX_GLOBAL_DICT_SIZE &&
           !dict_has_key(id, key)) { // we allow insert if inserted key is
                                     // already in the global dictionary
    DEBUG('!', "Aborting insert, ", dict_log_name(global_id),
          " has reached its maximum capacity!");
    assert(dict_size(global_id) == MAX_GLOBAL_DICT_SIZE);
  }
  else {
    DEBUG('+', dict_log_name(id), "[", parse_debug_string(key), "] <- ",
          parse_debug_string(value));
    // if [key] is already written, we overwrite it
    dictionaries()[id][key] = value;
  }
}

void dict_remove(identifier_t id, const char *key) {
  DEBUG('C', __FUNCTION__, "(", id, ", ", parse_debug_string(key), ")");
  if (!dict_exists(id)) {
    DEBUG('W', "Dictionary with id=", id, " does not exist!");
  }
  else if (!dict_has_key(id, key)) {
    DEBUG('W', "No value declared in ", dict_log_name(id), " under key ",
          parse_debug_string(key), "...");
  }
  else {
    DEBUG('+', "Key ", parse_debug_string(key), " removed from ",
          dict_log_name(id), " successfully!");
    dictionaries()[id].erase(key);
  }
}

const char *dict_find(identifier_t id, const char *key) {
  DEBUG('C', __FUNCTION__, "(", id, ", ", parse_debug_string(key), ")");
  DEBUG('.', "Searching for ", parse_debug_string(key), " in ",
        dict_log_name(id), "...");
  const char *out = dict_try_find(id, key);
  identifier_t global_id = jnp1::dict_global();
  if (out == NULL && id != global_id) {
    DEBUG('.', "...did not succeed; trying explicit search in ",
          dict_log_name(global_id), "...");
    out = dict_try_find(global_id, key);
  }
  return out;
}

void dict_clear(identifier_t id) {
  DEBUG('C', __FUNCTION__, "(", id, ")");
  if (!dict_exists(id)) {
    DEBUG('W', "Dictionary of id=", id, " not found!");
  }
  else {
    DEBUG('+', "Contents of ", dict_log_name(id), " cleared successfully!");
    dictionaries()[id].clear();
  }
}

void dict_copy(identifier_t src_id, identifier_t dst_id) {
  DEBUG('C', __FUNCTION__, "(", src_id, ", ", dst_id, ")");
  identifier_t global_id = jnp1::dict_global();
  if (!dict_exists(src_id) || !dict_exists(dst_id)) {
    DEBUG('W', "Dictionary with id either ", src_id, " or ", dst_id,
          " doesn't exist!");
  }
  else if (dst_id == global_id &&
           ((dict_size(dst_id) - get_common_quantity(src_id, dst_id)) +
            dict_size(src_id) > jnp1::MAX_GLOBAL_DICT_SIZE)) {
    DEBUG('!', "Aborting, can't copy this much elements into ",
          dict_log_name(dst_id));
  }
  else {
    for (const std::pair<std::string, std::string> &word :
         dictionaries()[src_id]) {
      dictionaries()[dst_id][word.first] = word.second;
    }
  }
}
} /* namespace jnp1 */
