#include "cdict"
#include "dict.h"
#include <string.h>
#include <unordered_map>

/* typ dla słownika */
using dict_t = std::unordered_map<std::string, std::string>;
/* typ dla identyfikatorów słowników */
using identifier_t = unsigned long;

identifier_t DICT_COUNTER = 0;
std::unordered_map<identifier_t, dict_t> dictionaries;


namespace jnp1 {


unsigned long dict_new() {
  dict_t new_dict;
  dictionaries[DICT_COUNTER] = new_dict;
  return DICT_COUNTER++;
}

void dict_delete(unsigned long id) { dictionaries.erase(id); }

size_t dict_size(unsigned long id) {
  if (dictionaries.count(id) != 0)
    return dictionaries[id].size();
  else
    return 0;
}

void dict_insert(unsigned long id, const char *key, const char *value) {
  if (dictionaries.count(id) != 0 && key != NULL && value != NULL)
    dictionaries[id][key] = value;
}

/* Jeżeli istnieje słownik o identyfikatorze id i zawiera klucz key, to usuwa
   klucz oraz wartość związaną z tym kluczem, a w przeciwnym przypadku nic nie
   robi. */
void dict_remove(unsigned long id, const char *key) {
  if (dictionaries.count(id) != 0)
    dictionaries[id].erase(key); // erase radzi sobie z nieistniejącym kluczem
}

/* Jeżeli istnieje słownik o identyfikatorze id i zawiera wartość pod kluczem
   key, to zwraca wskaźnik do tej wartości, a w przeciwnym zwraca wartość
   ze słownika globalnego. Jeśli w słowniku globalnym nie ma wartości
   pod kluczem key, to zwraca NULL. */
const char *dict_find(unsigned long id, const char *key) {
  if (dictionaries.count(id) != 0 && dictionaries[id].count(key) != 0)
    return dictionaries[id][key].c_str();
}

/* Jeżeli istnieje słownik o identyfikatorze id, usuwa wszystkie jego
   elementy, a w przeciwnym przypadki nic nie robi. */
void dict_clear(unsigned long id) {
  if (dictionaries.count(id) != 0) {
    dictionaries[id].clear();
    dictionaries.erase(id);
  }
}

/* Jeżeli istnieją słowniki o identyfikatorach src_id oraz dst_id, to kopiuje
   zawartość słownika o identyfikatorze src_id do słownika o identyfikatorze
   dst_id, a w przeciwnym przypadku nic nie robi. */
void dict_copy(unsigned long src_id, unsigned long dst_id) {
  if (dictionaries.count(src_id) != 0 && dictionaries.count(dst_id) != 0) {
              /* rozumiem że powinien nadpisać powtarzające się klucze w słowniku dst_id */
    for (std::pair<std::string, std::string> word : dictionaries[src_id]) {
      dictionaries[dst_id][word.first] = word.second; // przejmuje stringi na własność więc okej
    }

  }
}


} /* namespace jnp1 */
