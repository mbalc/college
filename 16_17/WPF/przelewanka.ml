(************************************************************************)
(***                                                                  ***)
(***                           Przelewanka                            ***)
(***                                               _                  ***)
(***                                            __/ \                 ***)
(***                                         __/  __/\                ***)
(***          ___________                 __/  __/    \               ***)
(***         |\_________/|              ~~~~__/~~~~~~~~\              ***)
(***         |           |            //// \            \             ***)
(***         |           |            |||   \        __/              ***)
(***         |           |           |||     \    __/                 ***)
(***         |           |       ________     \__/                    ***)
(***         |           |      |\______/|                            ***)
(***         |~~~~~~~~~~~|      |~~~~~~~~|                            ***)
(***         |___________|      |________|                            ***)
(***                                                                  ***)
(************************************************************************)
(***                                                                  ***)
(***        Author:     Michał Balcerzak             (mb385130)       ***)
(***                                                                  ***)
(***        Reviewer:   Katarzyna Kańska             (kk320728)       ***)
(***                                                                  ***)
(************************************************************************)

(** [val array_foldi : 'a t -> init:'b -> f:(int -> 'b -> 'a -> 'b) -> 'b]
    [array_foldi tab a f] działa podobnie do zwykłego Array.fold_left z tym,
    że funkcja dodatkowo otrzymuje indeks rozważanego elementu tablicy *)
let array_foldi f tab a =
  let rec pom i a =
    if i = Array.length tab then a
    else pom (i + 1) (f i a tab.(i))
  in
  pom 0 a
  
let array_for_all p tab = 
  Array.fold_left (fun a x -> (a && (p x))) true tab

(** rodzaje operacji możliwych do wykonania na kubkach; w komórkach pamięci
    przechowywane są indeksy kubków, na których operacje są wykonywane *)
type operacja =
  | Przelej of int * int (* [Przelej (a, b)] przelewa z kubka a do b *)
  | Wylej of int
  | Nalej of int

(** [zmiana_il_kub pr po ocz_x] zwraca liczbę określającą to, jak zmieni się
    ilość odpowiednio napełnionych kubków po zmianie ilości wody w kubku
    o oczekiwanej objętości wody [ocz_x] z [pr] na [po] *)
let zmiana_il_kub pr po ocz_x =
  let a = ref 0 in
  if pr = ocz_x then decr a;
  if po = ocz_x then incr a;
  !a

(** [zmien (tab, k) x ocz teraz] dla danych:
      [tab] - tablicy stanów napełnienia kubków,
      [k]   - liczby odpowiednio napelnionych kubkow,
      [x]   - indeksu kubka,
      [ocz] - tablicy oczekiwanych objętości w kubkach,
      [teraz] - nowej pojemnosci [x]-tego kubka
    zwraca nowe ([tab], [k]): tablicę stanów kubków i nową ilość
    dobrze napelnionych kubków po zamianie objętości wody
    w [x]-tym kubku na [teraz] *)
let zmien (tab, k) x ocz teraz =
  let dod = zmiana_il_kub tab.(x) teraz ocz.(x) in
  tab.(x) <- teraz;
  (tab, k + dod)

(** [zrob (tab, k) poj oper ocz] dla danych:
      [tab]  - tablicy stanów napełnienia kubków,
      [k]    - liczby dobrze napełnionych kubków,
      [poj]  - tablicy pojemności kubków,
      [oper] - pewnej operacji,
      [ocz]  - tablicy wymagań
    zwraca nowe ([tab], [k]): tablicę kubków oraz ilość
    dobrze napelnionych kubków po wykonaniu [oper] na [tab] *)
let zrob (tab, k) poj oper ocz =
  let tab = Array.copy tab in
  match oper with
  | Przelej (a, b) ->
    let u = min tab.(a) (poj.(b) - tab.(b)) in (* tyle trzeba przelać *)
    let (pomtab, pomk) = zmien (tab, k) a ocz  (tab.(a) - u) in
    zmien (pomtab, pomk) b ocz (pomtab.(b) + u)
  | Nalej a -> zmien (tab, k) a ocz poj.(a)
  | Wylej a -> zmien (tab, k) a ocz 0


(** [dodaj (tab, k) (kol, m) poj oper ocz st] dla danych:
      [tab]  - tablicy stanów napełnienia kubków,
      [k]    - liczby dobrze napełnionych kubków,
      [kol]    - kolejki stanów do rozważenia w przyszlosci,
      [m]    - mapy już rozważonych stanów,
      [poj]  - tablicy pojemności kubków,
      [oper] - pewnej operacji,
      [ocz]  - tablicy wymagań,
      [st]   - liczby kroków potrzebnych do otrzymania stanu opisanego w [tab]
               ze stanu początkowego (wszystkie kubki puste)
    zwraca nowe ([kol], [m]): kolejkę i mapę po rozważeniu stanu otrzymanego
    po wykonaniu [oper] na [tab] *)
let dodaj (tab, k) (kol, m) poj oper ocz st =
  let (tab1, k1) = zrob (tab, k) poj oper ocz in
  if PMap.exists tab1 m then (kol, m) else
    begin
      Queue.add (tab1, k1) kol;
      (kol, (PMap.add tab1 (st + 1) m))
    end


(** [dziel tab poj] dla danych:
      [tab] - stanu kubków [tab],
      [poj] - tablicy pojemności kubków
    zwraca ([ful], [wat], [emp]): podział kubków na trzy listy indeksów do nich
    tak, że listy zawierają indeksy do kubków kolejno: pełnych,
    częściowo napełnionych oraz pustych *)
let dziel tab poj =
  let rec pom (a, b, c) i =
    if i = Array.length tab then (a, b, c) else
    if tab.(i) = poj.(i) then pom (i :: a, b, c) (i + 1) else
    if tab.(i) = 0 then pom (a, b, i :: c) (i + 1) else
      pom (a, i :: b, c) (i + 1) in
  pom ([], [], []) 0

(** [generuj tab poj ocz kol m st] dla danych:
      [tab] - stanu kubków,
      [poj] - tablicy pojemności kubków,
      [ocz] - tablicy oczekiwań,
      [kol]   - kolejki stanów do rozważenia w przyszłości,
      [m]   - mapy już rozważonych stanów,
      [st]  - ilości kroków, w której udało się osiągnąć stan opisany w [tab]
    zwraca nowe ([kol], [m]): kolejkę i mapę uwzględniające wszystkie stany,
    których osiągnięcie jest możliwe poprzez wykonanie pewnej operacji
    na danym stanie [tab] *)
let generuj (tab, k) poj ocz kol m st =
  let wyj (ful, wat, emp) =
    let pary l1 l2 (q2, m2)=
      let wszystkie el ls (q1, m1) =
        let pom a x = dodaj (tab, k) a poj (Przelej (el, x)) ocz st in
        List.fold_left pom (q1, m1) ls in
      List.fold_left (fun a x -> wszystkie x l2 a) (q2, m2) l1 in
    let wykonaj l (q1, m1) f =
      let pom a x = dodaj (tab, k) a poj (f x) ocz st in
      List.fold_left pom (q1, m1) l in
    let df = pary (ful @ wat) (wat @ emp) (kol, m) in
    let df = wykonaj (ful @ wat) df (fun x -> Wylej x) in
    wykonaj (wat @ emp) df (fun x -> Nalej x) in
  wyj (dziel tab poj)

(** procedura, która sprawdza, czy wśród oczekiwanych objętości wody
    znajduje się przynajmniej jeden pusty lub pełny kubek (jest to
    niezmiennik każdej z dozwolonych w zadaniu operacji, więc nie da się
    otrzymać stanu który nie spełnia takiego warunku) *)
let wyp_test ocz poj =
  let pom i a x = a || x = poj.(i) || x = 0 in
  array_foldi pom ocz false

(** sprawdza, czy wszystkie oczekiwane objętosci są podzielne przez najmniejszy
    wspólny dzielnik objętości kubków (wszystkie dozwolone operacje mają taki
    niezmiennik) *)
let nwd_test ocz poj =
  let rec nwd a b =
    if a > b then nwd b a else (* a <= b *)
    if b = 0 then 0 else
    if a = 0 then b else
      nwd (b mod a) a in
  let u = Array.fold_left (fun a x -> nwd a x) poj.(0) poj in
  if u = 0 then true else
    array_for_all (fun x -> x mod u = 0) ocz

(** [inicjalizuj pocz] dla danych:
      [pocz] - tablica opisana w treści zadania
    zwraca ([poj], [ocz], [kol], [m], [n]) takie, że:
      [poj] - tablica pojemności kolejnych kubków,
      [ocz] - tablica oczekiwanych objętości wody w kubkach,
      [kol]   - kolejkę, w której będziemy przechowywać stany kubków
              do rozważenia w przyszłości w postaci (tab, k), gdzie:
                [tab] - tablica zawartości kubków
                [k]   - liczba dobrze napełnionych kubków (w sensie [ocz])
      [m]   - mapę, na której już rozważonym stanom przypiszemy ilość operacji,
              które trzeba było wykonywać, by otrzymać taki stan
      [n]   - liczbę kubków *)
let inicjalizuj pocz =
  let n = Array.length pocz in
  let tab = Array.make n 0
  and poj = Array.init n (fun i -> fst pocz.(i))
  and ocz = Array.init n (fun i -> snd pocz.(i)) in
  let k = array_foldi (fun i a x -> if x = ocz.(i) then a + 1 else a) tab 0 in
  let kol = Queue.create ()
  and m = PMap.empty in
  Queue.add (tab, k) kol;
  (poj, ocz, kol, PMap.add tab 0 m, n)

(** [przelewanka pocz] dla danej tablicy opisanej w treści zadania, składającej
    się z par postaci (xi, yi) oznaczających kolejno pojemność i oczekiwaną
    objętość wody w i-tym kubku zwraca minimalną liczbę operacji (opisanych w
    treści) które trzeba podjąć, by móc otrzymać oczekiwane objętości wody
    w kubkach lub (-1), gdy nie jest to możliwe *)
let przelewanka pocz =
  if pocz = [| |] then 0 else
    let (poj, ocz, kolejka, mapp, n) = inicjalizuj pocz in
    if not (wyp_test ocz poj && nwd_test ocz poj) then -1 else
      let rec pom (kol, m) =
        if Queue.is_empty kol then -1 else
          let (tab, k) = Queue.pop kol in
          if k = n then PMap.find tab m else
            pom (generuj (tab, k) poj ocz kol m (PMap.find tab m))
      in
      pom (kolejka, mapp)


(* * * TESTZONE * * *)
(* let _ =
   assert(przelewanka [|(5, 0); (3, 0); (1, 0) |] = 0);;

   assert(przelewanka [|(5, 5); (3, 3); (1, 1) |] = 3);;

   assert(przelewanka [|(5, 3); (3, 3); (1, 0) |] = 3);;

   assert(przelewanka [|(5, 2); (3, 2); (1, 0) |] = 4);;

   assert(przelewanka [|(5, 1); (3, 1); (7, 1) |] = -1);;



   assert(przelewanka [|(5, 1); (3, 1); (7, 0) |] = 7);;

   assert(przelewanka [|(5,1); (3, 1); (7, 7) |] = 7);;

   assert(przelewanka [|(5,0); (3, 1); (7, 1) |] = 6);;

   assert(przelewanka [|(5,1); (3, 0); (7, 1) |] = 7);;

   assert(przelewanka [|(5,5); (3, 1); (7, 1) |] = 5);;

   assert(przelewanka [|(5,1); (3, 0); (7, 1) |] = 7);;

   assert(przelewanka [|(5,3); (3, 3); (7, 1) |] = 4);;

   assert(przelewanka [|(5,3); (3, 0); (7, 4) |] = 3);;

   assert(przelewanka [|(5,0); (3, 3); (7, 4) |] = 2);;

   assert(przelewanka [|(5,0); (3, 0); (7, 7) |] = 1);;


   assert(przelewanka [|(9,6); (4,4)|] = 8);;

   assert(przelewanka [|(8,1); (5,4); (3,3)|] = 7);;

   assert(przelewanka [|(8,8); (5,5); (3,3)|] = 3);;

   assert(przelewanka [| |] = 0);;

   assert(przelewanka [|(6,0); (4,0);(9,0)|] = 0);;

   assert(przelewanka [|(0,0); (0,0)|] = 0);;


   assert(przelewanka [||] = 0);;

   assert(przelewanka [|(2, 1); (4, 3); (8, 0)|] = -1);;

   assert(przelewanka [|(7, 6); (5, 3); (9, 2)|] = -1);;

   assert(przelewanka [|(7, 7); (0, 0); (2, 2)|] = 2);;

   assert(przelewanka [|(7, 7); (0, 0); (9, 2)|] = 2);;

   assert(przelewanka [|(3, 3); (4, 4); (4, 4)|] = 3);;

   assert(przelewanka [|(8, 8); (7, 1); (4, 4); (1, 0)|] = 4);;

   assert(przelewanka [|(8, 8); (3, 0); (9, 9); (0, 0); (9, 9)|] = 3);;

   assert(przelewanka [|(9, 6); (1, 0); (4, 0); (5, 5); (0, 0); (8, 8)|] = 5);;

   assert(przelewanka [|(3, 3); (4, 4); (7, 4); (8, 8); (6, 6); (5, 0)|] = 5);;

   assert(przelewanka [|(9, 9); (2, 1); (1, 1); (6, 6); (6, 6); (6, 6)|] = 6);;

   assert(przelewanka [|(8, 7); (4, 0); (5, 5); (8, 8); (9, 9); (8, 8)|] = 7);;

   assert(przelewanka [|(7, 0); (6, 0); (8, 8); (2, 2); (7, 7); (9, 5)|] = 5);;

   assert(przelewanka [|(0, 0); (4, 0); (10, 0); (10, 10); (11, 11); (4, 4); (10, 10); (5, 5)|] = 5);;

   assert(przelewanka [|(0, 0); (9, 9); (9, 9); (9, 9); (7, 7); (11, 0); (1, 1); (0, 0); (2, 2)|] = 6);;*)
