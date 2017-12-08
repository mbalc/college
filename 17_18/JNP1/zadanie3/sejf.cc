#include "sejf.h"

using ::std::greater_equal;
using ::std::less_equal;
using ::std::minus;
using ::std::multiplies;
using ::std::ostream;
using ::std::plus;
using ::std::string;

Sejf::Sejf(const string &nap, dostep_t dost)
    : napis(nap), dostep(dost), czyWlamanie(false), czyZmanipulowany(false) {}

Sejf &Sejf::zmanipuluj(dostep_t zmiana, auto compute, auto compare) {
  dostep_t nowyDostep = compute(dostep, zmiana);
  if (compare(dostep, nowyDostep)) {
    dostep = nowyDostep;
    czyZmanipulowany = true;
  }
  return *this;
}

Sejf &Sejf::operator-=(dostep_t zmiana) {
  return zmanipuluj(zmiana, minus<dostep_t>(), greater_equal<dostep_t>());
}

Sejf &Sejf::operator+=(dostep_t zmiana) {
  return zmanipuluj(zmiana, plus<dostep_t>(), less_equal<dostep_t>());
}

Sejf &Sejf::operator*=(dostep_t zmiana) {
  return zmanipuluj(zmiana, multiplies<dostep_t>(), less_equal<dostep_t>());
}

short int Sejf::operator[](const size_t adres) {
  if (napis.length() <= adres)
    return -1;

  if (dostep == 0) {
    czyWlamanie = true;
    return -1;
  }

  dostep--;
  return (unsigned char)napis[adres];
}

Kontroler::Kontroler(const Sejf &obj) : sejf_obj(obj) {}

Kontroler const Sejf::kontroler() const {
  return Kontroler(*this);
}

Kontroler::operator bool() const {
  return sejf_obj.dostep > 0;
}

string Kontroler::stan() const {
  if (sejf_obj.czyWlamanie)
    return "ALARM: WLAMANIE\n";
  else if (sejf_obj.czyZmanipulowany)
    return "ALARM: ZMANIPULOWANY\n";
  else
    return "OK\n";
}

ostream &operator<<(ostream &stream, const Kontroler &k) {
  stream << k.stan();
  return stream;
}
