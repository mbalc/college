#include "dict.h"
#include "dictglobal.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <limits.h>


/* Debugging "function", is optimized by compiler to blank code if logging is
   disabled
   . - log an elemental step taken during computations
   + - successful execution
   = - successful computation
   ± - successful execution that returns some important value
   ! - failure, proceeding would break global dictionary
   W - warning
   */
#define DEBUG                                                                  \
  if (DEBUG_CONST)                                                             \
  std::cerr << "DEBUG:  | "

/* Compilation flag -DNDEBUG disables debug logging */
#ifndef NDEBUG
static const bool DEBUG_CONST = true;
#else
static const bool DEBUG_CONST = false;
#endif

/* backup
#define DEBUG                                                                  \
  if (DEBUG_CONST)                                                             \
  std::cerr << "DEBUG:  | "

Compilation flag -DNDEBUG disables debug logging
#ifndef NDEBUG
static const bool DEBUG_CONST = true;
#else
static const bool DEBUG_CONST = false;
#endif
*/


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
  bool dict_exists(unsigned long id) {
    return dictionaries().count(id) != 0;
  }

  /* Unified dictionary name for debug logger */
  std::string dict_log_name(unsigned long id) {
    if (!dict_exists(id)) {
      return "BLANK";
    }
    if (id == jnp1::dict_global()) {
      return "THE_GLOBAL_DICTIONARY";
    }
    return "DICT_" + std::to_string(id);
  }

  /* Unified string object printing format used by debug logger */
  std::string parse_debug_string(const char *str) {
    if (str == NULL) {
      return "NULL";
    }
    return "\"" + (std::string)str + "\"";
  }

  /* Get value stored under [key] from dictionary with a given [id] */
  const char *dict_try_find(unsigned long id, const char *key) {
    if (dict_exists(id) && dictionaries()[id].count(key) != 0) {
      size_t old_dict_size = dictionaries()[id].size(); //!
      const char *out = dictionaries()[id][key].c_str();
      DEBUG << "=      " << dict_log_name(id) << "[" << parse_debug_string(key)
            << "] == " << parse_debug_string(out) << "\n";
      assert(old_dict_size == dictionaries()[id].size()); //!
      return out;
    }
    DEBUG << "W      (key " << parse_debug_string(key) << " not found in "
          << dict_log_name(id) << ")\n";
    return NULL;
  }

  bool dict_equal(dictionary_t first, dictionary_t second) {
    bool is_equal = true;
    dictionary_t::iterator it = second.begin();
    for (std::pair<std::string, std::string> first_pair : first) {
      std::pair<std::string, std::string> second_pair = *it;
      is_equal = is_equal && first_pair == second_pair;
      it = next(it); // Raczej tu nie robie żadnego wycieku pamięci.
    }
    is_equal = is_equal && it == second.end();
    return is_equal;
  }

  size_t get_common(unsigned long) {}
}

/* Creates a new dictionary entry in our collection with the first id that is
 * found not to be occupied by any other dictionary */
unsigned long jnp1::dict_new() {
  DEBUG << "Called dict_new()\n";
  static identifier_t DICT_COUNTER;
  size_t old_number_of_dicts = dictionaries().size();

  // while (dict_exists(DICT_COUNTER)) {
  //   DEBUG << ".      " << dict_log_name(DICT_COUNTER)
  //         << " with id=" << DICT_COUNTER << " already exists, skipping...";
  //   DICT_COUNTER++;
  // }
  if (DICT_COUNTER != ULONG_MAX) {
    dictionary_t new_dict;
    dictionaries()[DICT_COUNTER] = new_dict;
    DEBUG << "±      New dictionary set up with id=" << DICT_COUNTER << "!\n";
    assert(old_number_of_dicts + 1 == dictionaries().size());
    return DICT_COUNTER++;
  }
  else {
    DEBUG << "W      Max number of dictionaries exceeded" << DICT_COUNTER << "!\n";
    return 0;
  }

}

void jnp1::dict_delete(unsigned long id) {
  DEBUG << "Called dict_delete(" << id << ")\n";
  size_t old_number_of_dicts = dictionaries().size();
  if (!dict_exists(id)) {
    DEBUG << "W      Aborting delete, no such dictionary exists!\n";
    //! wiem że dość durne asserty ale jakieś miały być więc..
    assert(old_number_of_dicts == dictionaries().size());
  } else if (id == jnp1::dict_global()) {
    DEBUG << "!      Aborting delete, you can't delete " << dict_log_name(id)
          << "!\n";
    assert(old_number_of_dicts == dictionaries().size()); //!
  } else {
    DEBUG << "+      Deleting " << dict_log_name(id)
          << " in all its glory...\n";
    dictionaries().erase(id);
    assert(old_number_of_dicts - 1 == dictionaries().size());
  }
}

size_t jnp1::dict_size(unsigned long id) {
  DEBUG << "Called dict_size(" << id << ")\n";
  if (!dict_exists(id)) {
    DEBUG << "W      No such dictionary exists, returning (0)...\n";
    return 0;
  }
  size_t out = dictionaries()[id].size();
  DEBUG << "=      dict_size(" << id << ")=" << out << "\n";
  return out;
}

void jnp1::dict_insert(unsigned long id, const char *key, const char *value) {
  DEBUG << "Called dict_insert(" << id << ", (" << parse_debug_string(key)
        << "), (" << parse_debug_string(value) << "))\n";
  unsigned long globalId = jnp1::dict_global();
  if (!dict_exists(id) || key == NULL || value == NULL) {
    DEBUG << "W      Aborting insert, criteria not met...\n";
  } else if (id == globalId && dict_size(globalId) >= MAX_GLOBAL_DICT_SIZE) {
    DEBUG << "!      Aborting insert, " << dict_log_name(globalId)
          << " has reached its maximum capacity!\n";
    assert(dict_size(globalId) == MAX_GLOBAL_DICT_SIZE);
  } else {
    DEBUG << "+      " << dict_log_name(id) << "[" << parse_debug_string(key)
          << "] <- " << parse_debug_string(value) << "\n";
    dictionaries()[id][key] = value;
  }
}

void jnp1::dict_remove(unsigned long id, const char *key) {
  DEBUG << "Called dict_remove(" << id << ", " << parse_debug_string(key)
        << ")\n";
  size_t old_number_of_dicts = dictionaries().size();
  if (dict_exists(id)) {
    if (dictionaries()[id].count(key)) {
      DEBUG << "+      Key " << parse_debug_string(key) << " removed from "
            << dict_log_name(id) << " successfully!\n";
      size_t old_dict_size = dictionaries()[id].size();
      dictionaries()[id].erase(key);
      assert(old_dict_size - 1 == dictionaries()[id].size());
    } else {
      DEBUG << "W      No value declared in " << dict_log_name(id)
            << " under key " << parse_debug_string(key) << "...\n";
    }
  } else {
    DEBUG << "W      " << dict_log_name(id) << "does not exist!\n";
  }
  assert(old_number_of_dicts == dictionaries().size());
}


const char *jnp1::dict_find(unsigned long id, const char *key) {
  DEBUG << "Called dict_find(" << id << ", " << parse_debug_string(key)
        << ")\n";
  DEBUG << ".      Searching for " << parse_debug_string(key) << " in "
        << dict_log_name(id) << "...\n";
  size_t old_number_of_dicts = dictionaries().size(); //!
  const char *out = dict_try_find(id, key);
  if (out == NULL) {
    unsigned long globalId =
        jnp1::dict_global(); //! może sie zdarzyć że w tym momencie
    // utworzony zostanie słownik globalny czy na pewno tego chcemy?
    DEBUG << ".      ...did not succeed; trying explicit search in "
          << dict_log_name(globalId) << "...\n";
    out = dict_try_find(globalId, key);
  }
  assert(old_number_of_dicts == dictionaries().size()); //!
  return out;
}

void jnp1::dict_clear(unsigned long id) {
  DEBUG << "Called dict_clear(" << id << ")\n";
  size_t old_number_of_dicts = dictionaries().size();
  if (!dict_exists(id)) {
    DEBUG << "W      Dictionary of id=" << id << " not found!\n";
  } else {
    DEBUG << "+      Contents of " << dict_log_name(id)
          << " cleared successfully!\n";
    dictionaries()[id].clear();
  }
  assert(old_number_of_dicts == dictionaries().size());
}

void jnp1::dict_copy(unsigned long src_id,
               unsigned long dst_id) { // TODO globalDict support
  DEBUG << "Called dict_copy(" << src_id << ", " << dst_id << ")\n";
  size_t old_number_of_dicts = dictionaries().size(); //!
  if (dict_exists(src_id) && dict_exists(dst_id)) {
    dictionary_t copy_of_src_dict(
        dictionaries()[src_id]); //! to jest czasochłonne
    // trzeba by coś zrobić z tym optymalizowaniem kompilatora
    // i wtedy do porównywania czy sie zmienił trzeba by dać
    // nową funkcje żeby móc ją wsadzić do asserta.

    unsigned long global_id = jnp1::dict_global();
    /* rozumiem że powinien nadpisać powtarzające się klucze w słowniku dst_id
     */
    for (std::pair<std::string, std::string> word : dictionaries()[src_id]) {
      if (dst_id == global_id &&
          dictionaries()[dst_id].size() >= jnp1::MAX_GLOBAL_DICT_SIZE &&
          dictionaries()[dst_id].count(word.first) == (size_t)0) {
        DEBUG << "W      Try of copying into global dictionary which exceeded "
              << "its max size\n";
        break;
      }
      else {
        dictionaries()[dst_id][word.first] =
          word.second; // przejmuje stringi na własność więc okej
      }
    }
    assert(dict_equal(copy_of_src_dict, dictionaries()[src_id]));
  }
  assert(old_number_of_dicts == dictionaries().size());
}
