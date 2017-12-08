#ifndef JNP1__SEJF_HPP
#define JNP1__SEJF_HPP

#include <string>

class Kontroler;

class Sejf {
  friend class Kontroler;

private:
  using dostep_t = unsigned long long;
  static const dostep_t DEFAULT_ATTEMPTS = 42;

  std::string napis;
  dostep_t dostep;
  bool czyWlamanie;
  bool czyZmanipulowany;
  Sejf &zmanipuluj(dostep_t zmiana, auto compare, auto compute);

public:
  Sejf(const std::string &nap, dostep_t dost = DEFAULT_ATTEMPTS);

  Sejf(Sejf &&przenoszony)
      : napis(przenoszony.napis), dostep(przenoszony.dostep),
        czyWlamanie(przenoszony.czyWlamanie),
        czyZmanipulowany(przenoszony.czyZmanipulowany){};
  Sejf(const Sejf &) = delete;            // copy constructor
  Sejf &operator=(Sejf &&) = default;     // move assignment
  Sejf &operator=(const Sejf &) = delete; // copy assignment

  short int operator[](const size_t adres); // zmienia dostep i czyZmanipulowany
  Sejf &operator-=(dostep_t zmiana);
  Sejf &operator+=(dostep_t zmiana);
  Sejf &operator*=(dostep_t zmiana);

  Kontroler const kontroler() const;
};

class Kontroler {
  friend class Sejf;

private:
  Kontroler(const Sejf &obj);
  const Sejf &sejf_obj;
  std::string stan() const;

public:
  Kontroler() = delete;                             // default constructor
  Kontroler(Kontroler &&) = delete;                 // move constructor
  Kontroler(const Kontroler &) = delete;            // copy constructor
  Kontroler &operator=(Kontroler &&) = delete;      // move assignment
  Kontroler &operator=(const Kontroler &) = delete; // copy assignment

  explicit operator bool() const;

  friend std::ostream &operator<<(std::ostream &stream, const Kontroler &k);
};

#endif // JNP1__SEJF_HPP guard
