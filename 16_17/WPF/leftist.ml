(******************************************************)
(*  Implementacja złączalnych kolejek priorytetowych  *)
(*-          (oparta na drzewach lewicowych)          *)
(******************************************************)
(*    Autor:      Michał Balcerzak      (mb385130)    *)
(*    Recenzent:  Krzysztof Piesiewicz  (kp385996)    *)
(******************************************************)



(** a' queue: typ przetrzymujący złączalne kolejki priorytetowe
 *    kolejka taka konstrukcyjnie będzie stanowiła drzewo lewicowe
 *
 *  Założenia typu:
 *  - spełnianie warunku kopca:
 *      wszystkie ewewntualne elementy o niższych priorytetach od priorytetu
 *      pewnego wierzchołka będą zawarte w poddrzewach tego wierzchołka;
 *      dzięki temu w korzeniu zawsze będzie element o najwyższym priorytecie,
 *      co gwarantuje, że drzewo będzie zwracało elementy w dobrej kolejności
 *  - spełnianie warunku lewicowości:
 *      skrajnie prawa głębokość takiego drzewa będzie najmniejsza,
 *      nie większa niż logarytm z liczby wierzchołków całego drzewa;
 *      dzięki temu będziemy mogli podłączać do siebie drzewa w taki sposób,
 *      że w każdym kroku łączenia będziemy mogli wybrać mniejsze z poddrzew
 *      do połączenia ze sobą, co pozwoli zamortyzować pesymistyczną złożoność
 *      algorytmu łączenia drzew z kwadratowej do liniowo-logarytmicznej
 *      (w każdym kroku będziemy "odrzucać" przynajmniej połowę pozostałych do
 *      połączenia wierzchołków drzewa)
 *
 *  Struktura:
 *    Node: reprezentacja wierzchołka o elementach:
 *      ('a queue): pierwszy, lewy syn wierzchołka
 *      ('a queue): drugi, prawy syn wierzchołka (krótka skrajnie pr. głębokość)
 *      ('a):       wartość priorytetowa wierzchołka
 *      (int):      głębokość skrajnie prawej ścieżki drzewa
 *    Leaf: odpowiednik pustego liścia/drzewa/poddrzewa
 **)
type 'a queue =
  | Node of ('a queue) * ('a queue) * 'a * int
  | Leaf


(** {empty} - procedura zwracająca nową, pustą kolejkę *)
let empty =
  Leaf


(** Empty <-> wyjątek podnoszony przez [delete_min] gdy kolejka jest pusta *)
exception Empty


(** {join q1 q2} - procedura łącząca dwie kolejki;
 *    dla danych kolejek [q1] i [q2] o tych samych typach
 *    zwraca kolejkę zawierającą wszystkie elementy obydwu kolejek
 *  {merge tree pt1 pt2 e} - procedura pomocnicza, zwracająca kolejkę
 *    spełniającą założenia zadania, na którą składają się:
 *      [e] - korzeń drzewa,
 *      [tree] - jedno z poddrzew,
 *    a drugie podddrzewo powstaje w wyniku połączenia składowych [pt1] i [pt2]
 *  {arrange_trees t1 t2} - procedura pomocnicza porządkująca drzewa [t1] i [t2]
 *    pod względem głębokości minimalnego (prawego) poddrzewa w parę kolejek
 *
 *  Działanie: procedura wykonuje się rekurencyjnie dla coraz mniejszych
 *    poddrzew, wykorzystując założenia typu tak, by jak najszybciej dotrzeć
 *    do jednego z trywialnych przypadków, gdy przynajmniej jedno z drzew
 *    do podłączenia jest puste; wtedy, podczas zwijania się rekurencji
 *    poddrzewa łączone są tak, by dla wynikowego drzewa były zachowane
 *    założenia typu
 **)
let rec join (q1:('a queue)) (q2:('a queue)) =
  let arrange_trees t1 t2 =
    match t1, t2 with
    | _, Leaf -> (t1, t2)
    | Leaf, _ -> (t2, t1)
    | Node (_, _, _, x), Node (_, _, _, y) ->
      if x < y then (t2, t1)
      else (t1, t2) in
  let merge tree pt1 pt2 pr =
    match arrange_trees tree (join pt1 pt2) with
    | p, Leaf -> Node (p, Leaf, pr, 0)
    | p, Node (qq1, qq2, qpr, qdp) ->
      Node (p, Node (qq1, qq2, qpr, qdp), pr, qdp + 1) in
  match (q1, q2) with
  | _, Leaf -> q1
  | Leaf, _ -> q2
  | Node (q1_l, q1_r, pr1, _), Node (q2_l, q2_r, pr2, _) ->
    if pr1 > pr2 then
      merge q2_l q1 q2_r pr2
    else
      merge q1_l q2 q1_r pr1


(** {add e q}: zwraca kolejkę powstałą z dołączenia elementu [e] do kolejki [q];
 *    zakładamy że typ danej kolejki pozwala na przetrzymywanie
 *    elementów typu [e]
 *
 *  Działanie: tworzy nowe drzewo z wierzchołkiem o priorytecie [e]
 *    i wykorzystuje procedurę {join} do złączenia drzew
 **)
let rec add (e:'a) (q:('a queue)) =
  match q with
  | Node _ -> join q (add e Leaf)
  | Leaf -> Node (Leaf, Leaf, e, 0)


(** {delete_min q} - procedura usuwająca oraz zwracająca element
 *    o najwyższym priorytecie;
 *    dla danej kolejki [q] zwraca:
 *      parę [(e, q')] <-> gdy kolejka [q] jest niepusta, wtedy:
 *        [e]  <-> element z danej kolejki [q] o najmniejszym priorytecie
 *        [q'] <-> nowa kolejka powstała po usunięciu [e] z kolejki [q]
 *      lub podnosi:
 *        wyjątek Empty <-> gdy kolejka [q] jest pusta
 *
 *  Działanie: usuwa i zwraca korzeń oraz łączy poddrzewa usuniętego wierzchołka
 *    za pomocą procedury join
 **)
let delete_min (q:('a queue)) =
  match q with
  | Node (q1, q2, e, _) -> (e, (join q1 q2))
  | Leaf -> raise Empty


(** {is_empty q} - procedura sprawdzająca, czy kolejka jest pusta;
 *    dla danej kolejki [q] zwraca:
 *      true  <-> kolejka (q) jest pusta
 *      false <-> kolejka (q) jest niepusta
 *
 *  Działanie: oparte na założeniach typu
 **)
let is_empty (q:('a queue)) =
  match q with
  | Node _ -> false
  | Leaf -> true


(* * *          * * *)
(* * * TESTZONE * * *) (*)
(* * *          * * *)

(* sprawdzaczka losowa - moja *)
let max_SIZ = 15    (* number of items in generated lists *)
let max_PRI = 7     (* max value of integer priority in a list *)
let max_ITR = 9999  (* number of iterations of a debug code *)

(* breaks down a queue and lists its content in output order *)
let return_list_of_queue q =
  let rec pom left =
    if is_empty left then [] else
      match delete_min left with
      | (l, p) -> l::(pom p) in
  pom q

(* generates a list of n random integers ranging from 0 to max_PRI *)
let gen_random_list n =
  let rec pom a i =
    if i < 1 then []
    else
      (Random.int max_PRI)::(pom a (i-1)) in
  begin
    Random.self_init();
    pom [] n
  end

(* generates a queue with a content of a given list *)
let queue_list arg =
  List.fold_left (fun acc el -> add el acc) empty arg

(* outputs contents of a given list *)
let print_list arg =
  let rec maker a =
    match a with
    | [] -> Printf.printf "())\n"
    | h::[] -> Printf.printf "%d)\n" h
    | h::t ->
      begin
        Printf.printf "%d, " h;
        maker t
      end in
  begin
    Printf.printf "(";
    maker arg
  end

let _ =
  for i = 0 to max_ITR do begin
    let test_list = gen_random_list max_SIZ in
    let test_sorted = List.sort (fun a b -> compare a b) test_list and
      test_queued = return_list_of_queue (queue_list test_list) in
    if test_sorted = test_queued then Printf.printf "OK %d\n" i
    else begin
      Printf.printf "ERROR!\n";
      Printf.printf "  expected: ";
      print_list test_sorted;
      Printf.printf "       got: ";
      print_list test_queued
    end
  end done

(* Sprawdzaczka warunków by Krzysiek *)
exception Father_is_a_leaf;;
exception Error_of_heap;;

let rec check_heap q =
  let node_condition f s =
    match f with
    | Node (_, _, fv, _) ->
      (match s with
       | Node (_, _, sv, _) -> fv <= sv
       | Leaf -> true)
    | Leaf -> raise Father_is_a_leaf (* swoją drogą, jest to niemożliwe *)
  in
  match q with
  | Node (l, r, v, rLen) ->
    check_heap l; check_heap r;
    if not (node_condition q l && node_condition q r) then
      raise Error_of_heap
  | Leaf -> ()
;;

exception Error_of_diff_lengths;;

let check_left q =
  let rec min_length t =
    match t with
    | Node (l, r, _, rLen) ->
      let l_minLen = min_length l
      and r_minLen = min_length r in
      let minLen = 1 + min l_minLen r_minLen in
      if minLen <> rLen then
        raise Error_of_diff_lengths
      else
        minLen
    | Leaf -> -1
  in
  let tmp = min_length q
  in if tmp = tmp then ()
;;

let check_tree q = check_heap q; check_left q;;

let print q =
  let rec tprint q lvl =
    for i = 0 to lvl do
      Printf.printf "  "
    done;
    match q with
    | Node (t1, t2, v, rLen) ->
      Printf.printf "Node v = %d, rLen = %d\n" v rLen;
      tprint t1 (lvl + 1); tprint t2 (lvl + 1)
    | Leaf -> Printf.printf "Leaf\n"
  in
  tprint q 0
;;

let printe q = print q; Printf.printf "\n"; flush stdout;;
let printse s q = Printf.printf "%s:\n" s; printe q;;
let printse_and_check s q = printse s q; check_tree q;;

let print_list l =
  let rec tmp_f l =
    match l with
    | [] -> ()
    | h::t -> Printf.printf "%d " h; tmp_f t
  in
  Printf.printf "[ "; tmp_f l; Printf.printf "]"
;;

let printe_list l = print_list l; Printf.printf "\n"; flush stdout;;

let delete_n_min n q =
  let rec tmp_f n q a =
    if n > 0 then
      let (v, q2) = delete_min q in
      tmp_f (n - 1) q2 (v::a)
    else
      (a, q)
  in
  tmp_f n q [];
;;

(* * *          * * *)
(* * * TESTZONE * * *) *)
(* * *          * * *)
