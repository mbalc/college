(************************************************************************)
(***                       Sortowanie topologiczne                    ***)
(************************************************************************)
(***        Author:     Michał Balcerzak             (mb385130)       ***)
(***        Reviewer:   Piotr Mitosek                (pm385901)       ***)
(************************************************************************)


(** wyjątek oznaczający, że dane zależności dla procedury [topol]
    są cykliczne *)
exception Cykliczne


(** [init_maps ls] funkcja, która na podstawie listy zależności otrzymywanej
    w argumencie przez procedurę [topol] zwraca parę referencji do map
    [elems] i [graph], których założenia są następujące
      <> [elems]: mapa "odwiedzeń", która dla każdego elementu przypisze jedną
        z wartości mówiących o stanie pewnego odpowiadającemu mu wierzchołka
        w grafie relacji przy zwiedzaniu go za pomocą przeszukiwania wgłąb:
          0 - gdy element ten nie został jeszcze odwiedzony
          1 - gdy pozycja tego elementu jest jeszcze rozważana; do danego
            wierzchołka weszliśmy, ale jeszcze z niego nie wyszliśmy
            (gdy dojdziemy do takiego wierzchołka ponownie, oznaczać to będzie,
            że w grafie zależności występuje cykl)
          2 - taki wierzchołek został już opuszczony i znamy jego pozycję
            (od końca) w szukanej kolejności topologicznej
      <> [graph]: mapa, która każdemu elementowi [x] przypisze listę elementów
        o których wiemy, że będą w kolejności topologicznej umieszczone po
        elemencie [x]; jest to dosłowne przepisanie zawartości elementów danej
        na wejściu funkcji [topol] listy na mapę - elementy listy postaci
        ([x], [y]) będą oznaczały przypisanie w mapie wartości [y]
        do argumentu [x] *)
let init_maps ls =
  let elems = ref PMap.empty
  and graph = ref PMap.empty in
  let creator (from, ls_to) =
    let browse el = elems := PMap.add el 0 !elems in
    elems := PMap.add from 0 !elems;
    begin
      if (PMap.exists from !graph) then
        graph := PMap.add from (ls_to @ (PMap.find from !graph)) !graph
      else graph := PMap.add from ls_to !graph
    end;
    List.iter browse ls_to in
  elems := PMap.empty;
  graph := PMap.empty;
  List.iter creator ls;
  (elems, graph)

(** [topol [(a_1,[a_11;...;a_1n]); ...; (a_m,[a_m1;...;a_mk])]]
    zwraca listę, na której każdy z elementów a_i oraz a_ij występuje
    dokładnie raz i która jest uporządkowana w taki sposób, że każdy
    element a_i jest przed każdym z elementów a_i1 ... a_il *)
let topol ls =
  let (elems, graph) = init_maps ls
  and tops = ref [] in
  let rec dfs el =
    if not (PMap.exists el !elems) then assert false;
    match (PMap.find el !elems) with
    | 1 -> raise Cykliczne
    | 2 -> ()
    | 0 ->
      elems := PMap.add el 1 !elems;
      if PMap.exists el !graph then List.iter dfs (PMap.find el !graph);
      elems := PMap.add el 2 !elems;
      tops := el :: !tops
    | _ -> assert false in
  PMap.iter (fun e _ -> dfs e) !elems;
  !tops

(* * * * *  * * * * *)
(* * * TESTZONE * * *) (*
(* * * * *  * * * * *)

open Topol;;
(* Funkcja sprawdzająca: spr *)
let head l =
     match l with
     | [] -> raise Not_found
     | h::_ -> h
let tail l =
     match l with
     | [] -> raise Not_found
     | _::t -> t
let spr l ls =
     if l = [] then if ls = [] then true else false else
     let n = List.length l in
     let t =  l in
     let ll = ref l in
     let m = ref PMap.empty in
     for i = 1 to n do
          m := PMap.add (head !ll) i !m;
          ll := tail !ll;
     done;
     let rec pom lx =
          match lx with
          | [] -> true
          | h::t ->
               let q = PMap.find (fst h) !m in
               let rec pom2 lh =
                    match lh with
                    | [] -> true
                    | h2::t2 ->
                         if (q < (PMap.find h2 !m)) then pom2 t2
                         else false
               in
               if pom2 (snd h) then pom t
               else false
     in
     pom ls;;
let spr ls = spr (topol ls) ls;;
(* Część I - małe testy poprawnościowe *)
assert(spr [(3, [2; 4; 5; 7; 10]); (11, []); (0, []); (2, [4; 5]); (3, [2])]);;
assert(spr []);;
assert(spr [(0, []); (0, [])]);;
assert(spr [("ala", ["ma"; "kota"; "i"; "psa"]); ("ma", ["kota"; "i"; "psa"]); ("pies", ["psa"])]);;
assert(try (let x = topol [([], [[]])] in false) with Cykliczne -> true);; (*cykl*)
assert(spr [([[]], [])]);;
assert(spr [(1, [6; 2]); (6, [2]); (5, [2; 3]); (2, [3]); (5, [4])]);;
assert(spr [("nie", [])]);;
assert(spr [([], [])]);;
assert(spr[([], []); ([[]], []); ([[[]]], [[]]); ([[[[]]]], [[[]]])]);;
assert(try (let x = topol [(1, [2]); (2, [3]); (3, [4]); (4, [5]); (5, [6]);
                           (6, [7]); (7, [8]); (8, [2])] in false) with Cykliczne -> true);; (*cykl*)
(* Część II - proste testy wydajnościowe *)
let rec f n l =
     if n = 0 then List.rev l
     else f (n - 1) ((n, [n + 1])::l);;
let f n = f n [];;
let s l = List.sort compare l;;
let l1 = topol (f 500);;
assert (l1 = s l1);;
let l1 = topol (f 1000);;
assert (l1 = s l1);;
let l1 = topol (f 10000);;
assert (l1 = s l1);;
let l1 = topol (f 20000);;
assert (l1 = s l1);;
let l1 = topol (f 30000);;
assert (l1 = s l1);;
let l1 = topol (f 40000);;
assert (l1 = s l1);;
let l1 = topol (f 50000);;
assert (l1 = s l1);;
let l1 = topol (f 60000);;
assert (l1 = s l1);;
let l1 = topol (f 70000);;
assert (l1 = s l1);;
let l1 = topol (f 80000);;
assert (l1 = s l1);;
let l1 = topol (f 85000);;
assert (l1 = s l1);;
let l1 = topol (f 90000);;
assert (l1 = s l1);;
let l1 = topol (f 100000);;
assert (l1 = s l1);;
*)
