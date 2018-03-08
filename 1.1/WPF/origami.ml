(**********************************************************************)
(*                    Implementacja zadania Origami                   *)
(*      (Hyperrealistic Virtual Paper Sheet Folding Simulator 2!)     *)
(**********************************************************************)
(*        Autor:       Michał Balcerzak            (mb385130)         *)
(*        Recenzent:   Jacek Olczyk                (jo385896)         *)
(**********************************************************************)


(*** DEFINICJE TYPÓW ***)

(** Punkt na płaszczyźnie *)
type point = float * float

(** Poskładana kartka: funkcja zwracająca, ile razy kartkę przebije
      szpilka wbita w danym punkcie *)
type kartka = point -> int


(*** FUNKCJE POMOCNICZE ***)

(* zwraca liczbę float - kwadrat odległości między punktami [p1] i [p2] *)
let square_distance (x1, y1) (x2, y2) =
  let square a = a *. a in
  square (x1 -. x2) +. square (y1 -. y2)

(* sprawdza, czy punkt [p] leży po prawej stronie wektora [a]->[b];
    zwraca true, gdy tak jest *)
let po_prawej ((a_x, a_y), (b_x, b_y)) (p_x, p_y) =
  let g_x = b_x -. a_x and g_y = b_y -. a_y
  and h_x = p_x -. a_x and h_y = p_y -. a_y in
  let iloczyn = g_x *. h_y -. g_y *. h_x
  in iloczyn < -.epsilon_float

(* porównuje dwa punkty i zwraca true, gdy się pokrywają *)
let cmp (p_x, p_y) (q_x, q_y) =
  abs_float (p_x -. q_x) +. abs_float (p_y -. q_y) <= epsilon_float

(* dla wektorów [a]!=[b] przedstawiających prostą i wektora [p] - punktu
     zwraca punkt zawierający współrzędne symetrycznego odbicia podanego punktu
     względem prostej *)
let symetria ((a_x, a_y), (b_x, b_y)) (p_x, p_y) =
  let licz = (p_x -. a_x) *. (b_x -. a_x) +. (p_y -. a_y) *. (b_y -. a_y)
  and mian = (a_x -. b_x) *. (a_x -. b_x) +. (a_y -. b_y) *. (a_y -. b_y) in
  let u = licz /. mian in
  let q_x = ((b_x -. a_x) *. u +. a_x) *. 2.
  and q_y = ((b_y -. a_y) *. u +. a_y) *. 2.
  in (q_x -. p_x, q_y -. p_y)

(*** KONSTRUKTORY ***)

(** [prostokat p1 p2] zwraca kartkę, reprezentującą domknięty
      prostokąt o bokach równoległych do osi układu współrzędnych i lewym
      dolnym rogu [p1] a prawym górnym [p2]. Punkt [p1] musi więc być
      nieostro na lewo i w dół od punktu [p2]. Gdy w kartkę tę wbije się
      szpilkę wewnątrz (lub na krawędziach) prostokąta, kartka zostanie
      przebita 1 raz, w pozostałych przypadkach 0 razy
    [between a b c] sprawdza czy zachodzi a<=b<=c i zwraca true, gdy tak jest *)
let prostokat (p1 : point) (p2 : point) =
  let between p1 arg p2 fn =
    (fn p1 <= fn arg) && (fn arg <= fn p2) in
  fun (arg : point) ->
    if (between p1 arg p2 fst) && (between p1 arg p2 snd) then 1
    else 0

(** [kolko p r] zwraca kartkę, reprezentującą kółko domknięte o środku
      w punkcie [p] i promieniu [r] *)
let kolko (p : point) (r : float) =
  fun (arg : point) ->
    if square_distance p arg <= r *. r then 1
    else 0


(*** FUNKCJE WYŻSZYCH RZĘDÓW ***)

(** [zloz p1 p2 k] składa kartkę [k] wzdłuż prostej przechodzącej
      przez punkty [p1] i [p2] (muszą to być różne punkty). Papier jest
      składany w ten sposób, że z prawej strony prostej (patrząc w kierunku
      od [p1] do [p2]) jest przekładany na lewą. Wynikiem funkcji jest
      złożona kartka. Jej przebicie po prawej stronie prostej powinno więc
      zwrócić 0. Przebicie dokładnie na prostej powinno zwrócić tyle samo,
      co przebicie kartki przed złożeniem. Po stronie lewej - tyle co przed
      złożeniem plus przebicie rozłożonej kartki w punkcie, który nałożył
      się na punkt przebicia. *)
let zloz (p1 : point) (p2 : point) (k : kartka) =
  fun p ->
    if po_prawej (p1, p2) p then 0 else
      let odb = symetria (p1, p2) p in
      if cmp odb p then k p
      else k p + k odb


(** [skladaj [(p1_1,p2_1);...;(p1_n,p2_n)] k =
      = zloz p1_n p2_n (zloz ... (zloz p1_1 p2_1 k)...)], czyli wynikiem jest
      złożenie kartki [k] kolejno wzdłuż wszystkich prostych z listy *)
let skladaj (l : (point * point) list) (k : kartka) =
  List.fold_left (fun kar (p1, p2) -> zloz p1 p2 kar) k l



(* * * *  TESTZONE  * * * *)
(* )
   open Origami;;
   let op=[((0.0,1.0),(9.0,7.0));((10.0,10.0),(9.0,3.0));((9.0,10.0),(10.0,7.0));((1.0,2.0),(1.0,7.0));((10.0,3.0),(2.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test0=skladaj op kartka;;
   assert (test0 (6.0,0.0)=0);;
   let op=[((7.0,6.0),(7.0,10.0));((10.0,10.0),(5.0,3.0));((2.0,7.0),(10.0,10.0));((7.0,4.0),(6.0,2.0));((10.0,3.0),(5.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test1=skladaj op kartka;;
   assert (test1 (9.0,8.0)=0);;
   let op=[((9.0,6.0),(3.0,1.0));((5.0,7.0),(9.0,2.0));((2.0,5.0),(8.0,3.0));((2.0,10.0),(4.0,5.0));((0.0,4.0),(6.0,9.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test2=skladaj op kartka;;
   assert (test2 (2.0,6.0)=0);;
   let op=[((7.0,4.0),(6.0,6.0));((8.0,4.0),(2.0,7.0));((4.0,0.0),(4.0,8.0));((3.0,0.0),(0.0,9.0));((3.0,1.0),(0.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test3=skladaj op kartka;;
   assert (test3 (1.0,8.0)=0);;
   let op=[((0.0,2.0),(2.0,10.0));((7.0,8.0),(3.0,8.0));((1.0,3.0),(10.0,7.0));((8.0,7.0),(10.0,8.0));((1.0,6.0),(4.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test4=skladaj op kartka;;
   assert (test4 (6.0,9.0)=0);;
   let op=[((9.0,5.0),(10.0,10.0));((10.0,8.0),(0.0,2.0));((2.0,10.0),(0.0,6.0));((5.0,0.0),(0.0,9.0));((10.0,10.0),(7.0,3.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test5=skladaj op kartka;;
   assert (test5 (8.0,0.0)=0);;
   let op=[((0.0,8.0),(3.0,0.0));((2.0,3.0),(9.0,5.0));((0.0,7.0),(5.0,1.0));((6.0,8.0),(8.0,3.0));((0.0,9.0),(2.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test6=skladaj op kartka;;
   assert (test6 (0.0,2.0)=0);;
   let op=[((4.0,7.0),(7.0,7.0));((6.0,10.0),(3.0,9.0));((2.0,2.0),(3.0,5.0));((1.0,3.0),(2.0,8.0));((7.0,0.0),(10.0,2.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test7=skladaj op kartka;;
   assert (test7 (10.0,8.0)=0);;
   let op=[((3.0,7.0),(8.0,10.0));((6.0,10.0),(5.0,4.0));((9.0,5.0),(4.0,7.0));((6.0,9.0),(9.0,3.0));((10.0,4.0),(3.0,0.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test8=skladaj op kartka;;
   assert (test8 (4.0,7.0)=0);;
   let op=[((2.0,2.0),(1.0,7.0));((9.0,10.0),(0.0,6.0));((2.0,10.0),(8.0,5.0));((10.0,7.0),(5.0,10.0));((1.0,10.0),(9.0,2.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test9=skladaj op kartka;;
   assert (test9 (4.0,1.0)=0);;
   let op=[((1.0,5.0),(2.0,7.0));((2.0,8.0),(3.0,5.0));((0.0,7.0),(2.0,9.0));((5.0,0.0),(9.0,2.0));((8.0,2.0),(3.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test10=skladaj op kartka;;
   assert (test10 (1.0,5.0)=0);;
   let op=[((4.0,9.0),(2.0,3.0));((4.0,8.0),(3.0,10.0));((1.0,1.0),(7.0,7.0));((1.0,2.0),(8.0,6.0));((0.0,7.0),(8.0,6.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test11=skladaj op kartka;;
   assert (test11 (6.0,0.0)=0);;
   let op=[((8.0,9.0),(4.0,6.0));((6.0,5.0),(9.0,9.0));((6.0,3.0),(8.0,5.0));((3.0,10.0),(3.0,6.0));((0.0,7.0),(2.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test12=skladaj op kartka;;
   assert (test12 (3.0,9.0)=0);;
   let op=[((4.0,10.0),(2.0,9.0));((0.0,2.0),(8.0,10.0));((1.0,6.0),(6.0,5.0));((1.0,4.0),(10.0,5.0));((2.0,8.0),(5.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test13=skladaj op kartka;;
   assert (test13 (1.0,3.0)=0);;
   let op=[((10.0,6.0),(0.0,9.0));((10.0,6.0),(0.0,6.0));((10.0,10.0),(5.0,1.0));((8.0,5.0),(10.0,7.0));((1.0,5.0),(6.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test14=skladaj op kartka;;
   assert (test14 (10.0,3.0)=0);;
   let op=[((10.0,6.0),(7.0,5.0));((7.0,10.0),(9.0,8.0));((2.0,7.0),(5.0,9.0));((8.0,7.0),(4.0,8.0));((9.0,6.0),(7.0,2.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test15=skladaj op kartka;;
   assert (test15 (6.0,8.0)=0);;
   let op=[((1.0,7.0),(0.0,10.0));((3.0,4.0),(0.0,2.0));((10.0,9.0),(7.0,2.0));((3.0,7.0),(1.0,1.0));((5.0,1.0),(9.0,8.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test16=skladaj op kartka;;
   assert (test16 (1.0,1.0)=1);;
   let op=[((1.0,3.0),(4.0,10.0));((1.0,8.0),(7.0,9.0));((0.0,9.0),(4.0,10.0));((1.0,7.0),(7.0,6.0));((3.0,6.0),(10.0,1.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test17=skladaj op kartka;;
   assert (test17 (0.0,10.0)=8);;
   let op=[((8.0,4.0),(6.0,7.0));((0.0,1.0),(0.0,10.0));((7.0,9.0),(2.0,2.0));((0.0,4.0),(2.0,8.0));((10.0,8.0),(3.0,2.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test18=skladaj op kartka;;
   assert (test18 (4.0,3.0)=0);;
   let op=[((3.0,7.0),(5.0,5.0));((5.0,7.0),(0.0,1.0));((6.0,4.0),(9.0,6.0));((6.0,10.0),(6.0,4.0));((7.0,10.0),(7.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test19=skladaj op kartka;;
   assert (test19 (1.0,0.0)=0);;
   let op=[((3.0,5.0),(8.0,9.0));((5.0,5.0),(10.0,1.0));((1.0,0.0),(0.0,7.0));((10.0,8.0),(4.0,3.0));((5.0,5.0),(4.0,0.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test20=skladaj op kartka;;
   assert (test20 (8.0,9.0)=0);;
   let op=[((4.0,10.0),(6.0,6.0));((5.0,7.0),(0.0,1.0));((7.0,1.0),(6.0,6.0));((3.0,5.0),(1.0,0.0));((6.0,4.0),(4.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test21=skladaj op kartka;;
   assert (test21 (7.0,2.0)=0);;
   let op=[((9.0,8.0),(6.0,5.0));((0.0,0.0),(4.0,1.0));((0.0,6.0),(2.0,2.0));((0.0,0.0),(9.0,7.0));((10.0,8.0),(3.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test22=skladaj op kartka;;
   assert (test22 (8.0,10.0)=0);;
   let op=[((8.0,9.0),(4.0,2.0));((7.0,6.0),(7.0,2.0));((3.0,4.0),(4.0,8.0));((6.0,8.0),(0.0,8.0));((10.0,5.0),(1.0,9.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test23=skladaj op kartka;;
   assert (test23 (4.0,5.0)=0);;
   let op=[((1.0,1.0),(6.0,5.0));((4.0,0.0),(2.0,8.0));((9.0,1.0),(6.0,1.0));((10.0,0.0),(4.0,10.0));((4.0,9.0),(5.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test24=skladaj op kartka;;
   assert (test24 (9.0,6.0)=0);;
   let op=[((2.0,4.0),(5.0,7.0));((5.0,7.0),(6.0,5.0));((5.0,7.0),(7.0,9.0));((3.0,1.0),(7.0,3.0));((10.0,6.0),(4.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test25=skladaj op kartka;;
   assert (test25 (0.0,9.0)=0);;
   let op=[((6.0,3.0),(8.0,2.0));((9.0,7.0),(10.0,2.0));((6.0,6.0),(1.0,5.0));((0.0,5.0),(2.0,4.0));((10.0,10.0),(8.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test26=skladaj op kartka;;
   assert (test26 (6.0,10.0)=0);;
   let op=[((8.0,9.0),(8.0,2.0));((3.0,4.0),(10.0,5.0));((8.0,4.0),(2.0,1.0));((2.0,2.0),(10.0,9.0));((4.0,3.0),(1.0,2.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test27=skladaj op kartka;;
   assert (test27 (2.0,4.0)=0);;
   let op=[((5.0,4.0),(1.0,1.0));((9.0,10.0),(4.0,6.0));((2.0,1.0),(6.0,8.0));((4.0,3.0),(7.0,9.0));((8.0,8.0),(10.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test28=skladaj op kartka;;
   assert (test28 (2.0,6.0)=4);;
   let op=[((9.0,10.0),(7.0,1.0));((0.0,10.0),(3.0,6.0));((8.0,8.0),(3.0,9.0));((0.0,2.0),(8.0,8.0));((4.0,3.0),(4.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test29=skladaj op kartka;;
   assert (test29 (9.0,7.0)=0);;
   let op=[((4.0,7.0),(9.0,10.0));((10.0,7.0),(10.0,2.0));((8.0,1.0),(2.0,6.0));((5.0,5.0),(7.0,10.0));((4.0,3.0),(9.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test30=skladaj op kartka;;
   assert (test30 (4.0,5.0)=0);;
   let op=[((5.0,5.0),(2.0,1.0));((0.0,2.0),(7.0,4.0));((5.0,3.0),(10.0,5.0));((4.0,2.0),(3.0,4.0));((6.0,4.0),(10.0,5.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test31=skladaj op kartka;;
   assert (test31 (3.0,2.0)=0);;
   let op=[((10.0,8.0),(2.0,10.0));((8.0,6.0),(8.0,9.0));((9.0,6.0),(3.0,8.0));((10.0,5.0),(4.0,3.0));((0.0,9.0),(7.0,4.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test32=skladaj op kartka;;
   assert (test32 (4.0,10.0)=0);;
   let op=[((5.0,6.0),(6.0,3.0));((8.0,2.0),(4.0,8.0));((4.0,9.0),(2.0,3.0));((7.0,2.0),(0.0,3.0));((10.0,4.0),(4.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test33=skladaj op kartka;;
   assert (test33 (4.0,6.0)=0);;
   let op=[((9.0,8.0),(8.0,5.0));((4.0,10.0),(3.0,0.0));((9.0,9.0),(9.0,0.0));((3.0,6.0),(8.0,5.0));((10.0,6.0),(9.0,2.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test34=skladaj op kartka;;
   assert (test34 (7.0,10.0)=0);;
   let op=[((6.0,2.0),(9.0,2.0));((4.0,9.0),(7.0,10.0));((5.0,1.0),(8.0,2.0));((2.0,2.0),(10.0,0.0));((1.0,5.0),(0.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test35=skladaj op kartka;;
   assert (test35 (5.0,7.0)=0);;
   let op=[((0.0,0.0),(5.0,1.0));((0.0,10.0),(8.0,7.0));((10.0,6.0),(2.0,6.0));((4.0,5.0),(7.0,5.0));((2.0,8.0),(7.0,0.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test36=skladaj op kartka;;
   assert (test36 (7.0,9.0)=1);;
   let op=[((4.0,1.0),(8.0,2.0));((1.0,0.0),(1.0,10.0));((3.0,5.0),(9.0,8.0));((10.0,6.0),(0.0,0.0));((0.0,3.0),(2.0,8.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test37=skladaj op kartka;;
   assert (test37 (5.0,4.0)=0);;
   let op=[((3.0,5.0),(5.0,6.0));((7.0,4.0),(3.0,1.0));((9.0,4.0),(0.0,1.0));((3.0,2.0),(4.0,8.0));((8.0,1.0),(5.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test38=skladaj op kartka;;
   assert (test38 (1.0,5.0)=0);;
   let op=[((5.0,0.0),(8.0,4.0));((2.0,2.0),(3.0,6.0));((6.0,2.0),(5.0,9.0));((5.0,3.0),(4.0,7.0));((5.0,7.0),(1.0,9.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test39=skladaj op kartka;;
   assert (test39 (5.0,9.0)=0);;
   let op=[((6.0,2.0),(2.0,6.0));((7.0,10.0),(3.0,7.0));((5.0,10.0),(10.0,7.0));((1.0,2.0),(9.0,10.0));((9.0,2.0),(10.0,0.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test40=skladaj op kartka;;
   assert (test40 (1.0,2.0)=0);;
   let op=[((1.0,5.0),(9.0,10.0));((2.0,2.0),(8.0,9.0));((1.0,2.0),(2.0,7.0));((8.0,5.0),(7.0,10.0));((6.0,10.0),(5.0,4.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test41=skladaj op kartka;;
   assert (test41 (5.0,1.0)=0);;
   let op=[((1.0,3.0),(8.0,10.0));((10.0,5.0),(1.0,8.0));((0.0,7.0),(1.0,1.0));((5.0,2.0),(10.0,10.0));((6.0,5.0),(0.0,0.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test42=skladaj op kartka;;
   assert (test42 (0.0,1.0)=0);;
   let op=[((5.0,1.0),(7.0,7.0));((2.0,4.0),(9.0,3.0));((4.0,6.0),(0.0,2.0));((7.0,5.0),(10.0,3.0));((0.0,3.0),(8.0,4.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test43=skladaj op kartka;;
   assert (test43 (4.0,4.0)=0);;
   let op=[((1.0,10.0),(10.0,6.0));((6.0,9.0),(9.0,3.0));((1.0,9.0),(7.0,0.0));((0.0,0.0),(9.0,9.0));((0.0,0.0),(10.0,10.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test44=skladaj op kartka;;
   assert (test44 (1.0,2.0)=0);;
   let op=[((7.0,3.0),(5.0,10.0));((5.0,5.0),(8.0,0.0));((3.0,9.0),(6.0,9.0));((0.0,7.0),(2.0,2.0));((2.0,4.0),(4.0,3.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test45=skladaj op kartka;;
   assert (test45 (5.0,9.0)=4);;
   let op=[((4.0,9.0),(9.0,7.0));((0.0,3.0),(7.0,9.0));((3.0,7.0),(1.0,0.0));((7.0,8.0),(0.0,8.0));((2.0,2.0),(5.0,5.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test46=skladaj op kartka;;
   assert (test46 (4.0,3.0)=0);;
   let op=[((1.0,3.0),(6.0,3.0));((9.0,0.0),(3.0,0.0));((6.0,9.0),(2.0,2.0));((5.0,4.0),(9.0,3.0));((2.0,2.0),(9.0,8.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test47=skladaj op kartka;;
   assert (test47 (1.0,9.0)=0);;
   let op=[((6.0,1.0),(5.0,10.0));((2.0,8.0),(7.0,10.0));((9.0,9.0),(3.0,3.0));((6.0,0.0),(0.0,8.0));((1.0,1.0),(2.0,8.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test48=skladaj op kartka;;
   assert (test48 (2.0,4.0)=0);;
   let op=[((5.0,8.0),(0.0,4.0));((8.0,2.0),(10.0,9.0));((10.0,1.0),(5.0,0.0));((1.0,7.0),(9.0,10.0));((9.0,7.0),(2.0,10.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test49=skladaj op kartka;;
   assert (test49 (8.0,0.0)=0);;
   let op=[((5.0,4.0),(8.0,5.0));((4.0,2.0),(3.0,0.0));((1.0,5.0),(4.0,2.0));((2.0,8.0),(10.0,4.0));((10.0,0.0),(7.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test50=skladaj op kartka;;
   assert (test50 (1.0,10.0)=0);;
   let op=[((2.0,4.0),(2.0,7.0));((3.0,3.0),(7.0,1.0));((1.0,10.0),(4.0,3.0));((9.0,2.0),(3.0,5.0));((1.0,2.0),(3.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test51=skladaj op kartka;;
   assert (test51 (2.0,10.0)=0);;
   let op=[((5.0,9.0),(2.0,1.0));((4.0,2.0),(0.0,9.0));((4.0,7.0),(4.0,4.0));((4.0,7.0),(0.0,4.0));((7.0,10.0),(2.0,0.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test52=skladaj op kartka;;
   assert (test52 (10.0,3.0)=2);;
   let op=[((2.0,1.0),(2.0,9.0));((4.0,5.0),(9.0,10.0));((3.0,0.0),(2.0,3.0));((10.0,4.0),(4.0,2.0));((7.0,10.0),(7.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test53=skladaj op kartka;;
   assert (test53 (7.0,1.0)=0);;
   let op=[((8.0,9.0),(1.0,4.0));((7.0,7.0),(10.0,10.0));((10.0,7.0),(6.0,4.0));((5.0,6.0),(4.0,8.0));((0.0,3.0),(3.0,10.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test54=skladaj op kartka;;
   assert (test54 (1.0,7.0)=0);;
   let op=[((7.0,6.0),(8.0,8.0));((5.0,6.0),(2.0,5.0));((6.0,1.0),(2.0,0.0));((6.0,10.0),(0.0,9.0));((9.0,6.0),(8.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test55=skladaj op kartka;;
   assert (test55 (3.0,7.0)=0);;
   let op=[((5.0,1.0),(3.0,10.0));((3.0,5.0),(10.0,9.0));((8.0,5.0),(9.0,2.0));((4.0,8.0),(10.0,0.0));((4.0,0.0),(2.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test56=skladaj op kartka;;
   assert (test56 (4.0,4.0)=0);;
   let op=[((1.0,8.0),(3.0,10.0));((10.0,7.0),(3.0,8.0));((3.0,0.0),(7.0,7.0));((9.0,10.0),(4.0,10.0));((3.0,0.0),(9.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test57=skladaj op kartka;;
   assert (test57 (2.0,0.0)=0);;
   let op=[((7.0,0.0),(0.0,3.0));((7.0,1.0),(10.0,3.0));((8.0,7.0),(4.0,3.0));((10.0,4.0),(9.0,0.0));((1.0,8.0),(3.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test58=skladaj op kartka;;
   assert (test58 (6.0,1.0)=0);;
   let op=[((3.0,10.0),(10.0,2.0));((4.0,7.0),(10.0,6.0));((7.0,0.0),(2.0,8.0));((2.0,10.0),(3.0,1.0));((9.0,0.0),(9.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test59=skladaj op kartka;;
   assert (test59 (7.0,9.0)=1);;
   let op=[((10.0,1.0),(6.0,6.0));((8.0,2.0),(0.0,5.0));((6.0,6.0),(10.0,4.0));((3.0,5.0),(2.0,9.0));((1.0,7.0),(6.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test60=skladaj op kartka;;
   assert (test60 (7.0,7.0)=0);;
   let op=[((2.0,4.0),(3.0,0.0));((3.0,3.0),(6.0,5.0));((9.0,5.0),(9.0,8.0));((10.0,6.0),(10.0,9.0));((0.0,4.0),(2.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test61=skladaj op kartka;;
   assert (test61 (10.0,6.0)=0);;
   let op=[((7.0,9.0),(3.0,4.0));((0.0,1.0),(3.0,5.0));((1.0,9.0),(6.0,7.0));((2.0,2.0),(7.0,2.0));((9.0,4.0),(5.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test62=skladaj op kartka;;
   assert (test62 (10.0,5.0)=0);;
   let op=[((2.0,6.0),(9.0,6.0));((2.0,9.0),(6.0,3.0));((10.0,6.0),(0.0,4.0));((1.0,5.0),(4.0,5.0));((1.0,10.0),(0.0,1.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test63=skladaj op kartka;;
   assert (test63 (6.0,9.0)=4);;
   let op=[((5.0,0.0),(3.0,10.0));((9.0,3.0),(5.0,4.0));((10.0,4.0),(6.0,7.0));((2.0,2.0),(5.0,3.0));((2.0,4.0),(6.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test64=skladaj op kartka;;
   assert (test64 (5.0,5.0)=0);;
   let op=[((9.0,3.0),(3.0,2.0));((8.0,8.0),(6.0,7.0));((5.0,0.0),(0.0,6.0));((7.0,1.0),(5.0,10.0));((7.0,2.0),(3.0,8.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test65=skladaj op kartka;;
   assert (test65 (1.0,7.0)=0);;
   let op=[((6.0,8.0),(4.0,7.0));((7.0,2.0),(8.0,8.0));((5.0,1.0),(9.0,9.0));((5.0,4.0),(0.0,2.0));((0.0,7.0),(6.0,1.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test66=skladaj op kartka;;
   assert (test66 (8.0,9.0)=0);;
   let op=[((6.0,7.0),(0.0,4.0));((10.0,1.0),(2.0,10.0));((8.0,1.0),(9.0,9.0));((4.0,2.0),(2.0,10.0));((8.0,9.0),(6.0,1.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test67=skladaj op kartka;;
   assert (test67 (2.0,1.0)=0);;
   let op=[((6.0,7.0),(9.0,9.0));((5.0,7.0),(8.0,2.0));((6.0,3.0),(2.0,6.0));((1.0,1.0),(1.0,9.0));((6.0,10.0),(4.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test68=skladaj op kartka;;
   assert (test68 (10.0,0.0)=2);;
   let op=[((2.0,10.0),(9.0,6.0));((0.0,5.0),(9.0,4.0));((2.0,6.0),(6.0,6.0));((7.0,6.0),(8.0,4.0));((8.0,9.0),(4.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test69=skladaj op kartka;;
   assert (test69 (2.0,4.0)=0);;
   let op=[((6.0,1.0),(1.0,9.0));((8.0,0.0),(2.0,4.0));((10.0,7.0),(7.0,4.0));((4.0,9.0),(2.0,0.0));((2.0,5.0),(10.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test70=skladaj op kartka;;
   assert (test70 (7.0,10.0)=0);;
   let op=[((1.0,4.0),(7.0,9.0));((3.0,9.0),(2.0,7.0));((7.0,6.0),(5.0,7.0));((0.0,3.0),(5.0,6.0));((10.0,9.0),(9.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test71=skladaj op kartka;;
   assert (test71 (0.0,0.0)=0);;
   let op=[((5.0,10.0),(2.0,0.0));((9.0,8.0),(6.0,3.0));((6.0,6.0),(8.0,10.0));((5.0,2.0),(10.0,3.0));((10.0,4.0),(1.0,6.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test72=skladaj op kartka;;
   assert (test72 (0.0,8.0)=0);;
   let op=[((6.0,0.0),(8.0,1.0));((9.0,0.0),(5.0,7.0));((7.0,9.0),(7.0,0.0));((3.0,5.0),(8.0,10.0));((8.0,10.0),(7.0,4.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test73=skladaj op kartka;;
   assert (test73 (3.0,1.0)=0);;
   let op=[((9.0,9.0),(9.0,1.0));((5.0,5.0),(7.0,1.0));((1.0,0.0),(4.0,1.0));((10.0,9.0),(8.0,0.0));((1.0,3.0),(10.0,7.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test74=skladaj op kartka;;
   assert (test74 (9.0,4.0)=0);;
   let op=[((5.0,3.0),(1.0,5.0));((7.0,10.0),(10.0,1.0));((5.0,0.0),(7.0,2.0));((4.0,2.0),(6.0,4.0));((2.0,7.0),(9.0,8.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test75=skladaj op kartka;;
   assert (test75 (8.0,7.0)=0);;
   let op=[((8.0,5.0),(10.0,9.0));((2.0,1.0),(2.0,10.0));((0.0,1.0),(1.0,8.0));((1.0,1.0),(6.0,6.0));((3.0,8.0),(3.0,3.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test76=skladaj op kartka;;
   assert (test76 (4.0,3.0)=0);;
   let op=[((10.0,8.0),(0.0,4.0));((0.0,0.0),(1.0,10.0));((3.0,9.0),(4.0,7.0));((2.0,1.0),(9.0,9.0));((2.0,10.0),(6.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test77=skladaj op kartka;;
   assert (test77 (10.0,10.0)=0);;
   let op=[((0.0,5.0),(9.0,8.0));((6.0,10.0),(8.0,0.0));((3.0,8.0),(0.0,7.0));((0.0,7.0),(9.0,6.0));((10.0,3.0),(1.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test78=skladaj op kartka;;
   assert (test78 (4.0,10.0)=0);;
   let op=[((4.0,4.0),(5.0,9.0));((4.0,5.0),(1.0,9.0));((3.0,6.0),(1.0,5.0));((2.0,7.0),(9.0,6.0));((2.0,2.0),(7.0,6.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test79=skladaj op kartka;;
   assert (test79 (10.0,0.0)=0);;
   let op=[((2.0,10.0),(4.0,3.0));((5.0,5.0),(9.0,1.0));((2.0,5.0),(0.0,9.0));((5.0,6.0),(1.0,2.0));((4.0,3.0),(1.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test80=skladaj op kartka;;
   assert (test80 (6.0,4.0)=0);;
   let op=[((5.0,7.0),(2.0,3.0));((10.0,9.0),(1.0,0.0));((0.0,9.0),(5.0,2.0));((6.0,4.0),(10.0,5.0));((5.0,1.0),(3.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test81=skladaj op kartka;;
   assert (test81 (0.0,4.0)=0);;
   let op=[((6.0,10.0),(5.0,4.0));((9.0,1.0),(3.0,9.0));((1.0,3.0),(9.0,2.0));((7.0,4.0),(5.0,1.0));((9.0,5.0),(1.0,1.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test82=skladaj op kartka;;
   assert (test82 (3.0,0.0)=0);;
   let op=[((1.0,0.0),(6.0,8.0));((2.0,0.0),(2.0,3.0));((2.0,4.0),(1.0,1.0));((5.0,0.0),(1.0,3.0));((1.0,0.0),(0.0,6.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test83=skladaj op kartka;;
   assert (test83 (2.0,8.0)=0);;
   let op=[((1.0,8.0),(0.0,5.0));((3.0,1.0),(1.0,6.0));((8.0,4.0),(5.0,7.0));((10.0,8.0),(3.0,1.0));((0.0,6.0),(7.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test84=skladaj op kartka;;
   assert (test84 (3.0,9.0)=0);;
   let op=[((4.0,6.0),(2.0,0.0));((0.0,7.0),(3.0,5.0));((9.0,6.0),(2.0,6.0));((8.0,6.0),(2.0,5.0));((2.0,8.0),(6.0,9.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test85=skladaj op kartka;;
   assert (test85 (8.0,7.0)=0);;
   let op=[((8.0,3.0),(9.0,1.0));((2.0,5.0),(7.0,4.0));((2.0,1.0),(0.0,7.0));((9.0,10.0),(10.0,6.0));((5.0,7.0),(9.0,3.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test86=skladaj op kartka;;
   assert (test86 (9.0,2.0)=0);;
   let op=[((6.0,4.0),(3.0,10.0));((10.0,5.0),(7.0,4.0));((1.0,10.0),(4.0,10.0));((5.0,3.0),(10.0,9.0));((1.0,8.0),(7.0,4.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test87=skladaj op kartka;;
   assert (test87 (0.0,10.0)=0);;
   let op=[((1.0,8.0),(5.0,10.0));((2.0,7.0),(9.0,3.0));((0.0,10.0),(7.0,0.0));((10.0,5.0),(6.0,9.0));((0.0,1.0),(8.0,2.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test88=skladaj op kartka;;
   assert (test88 (1.0,9.0)=1);;
   let op=[((10.0,9.0),(4.0,0.0));((6.0,2.0),(2.0,0.0));((8.0,2.0),(0.0,2.0));((7.0,3.0),(8.0,5.0));((10.0,4.0),(5.0,5.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test89=skladaj op kartka;;
   assert (test89 (5.0,8.0)=0);;
   let op=[((6.0,5.0),(3.0,1.0));((0.0,10.0),(10.0,2.0));((3.0,7.0),(6.0,7.0));((7.0,2.0),(2.0,3.0));((7.0,9.0),(6.0,1.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test90=skladaj op kartka;;
   assert (test90 (8.0,7.0)=0);;
   let op=[((7.0,5.0),(0.0,6.0));((2.0,0.0),(9.0,6.0));((5.0,7.0),(1.0,10.0));((4.0,6.0),(2.0,7.0));((6.0,4.0),(7.0,7.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test91=skladaj op kartka;;
   assert (test91 (5.0,7.0)=0);;
   let op=[((2.0,10.0),(6.0,0.0));((4.0,6.0),(0.0,0.0));((4.0,7.0),(3.0,4.0));((2.0,1.0),(0.0,10.0));((4.0,7.0),(9.0,7.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test92=skladaj op kartka;;
   assert (test92 (10.0,0.0)=0);;
   let op=[((5.0,6.0),(1.0,6.0));((4.0,4.0),(9.0,6.0));((5.0,7.0),(0.0,8.0));((7.0,7.0),(8.0,9.0));((6.0,9.0),(4.0,2.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test93=skladaj op kartka;;
   assert (test93 (5.0,1.0)=0);;
   let op=[((1.0,4.0),(6.0,4.0));((7.0,1.0),(8.0,3.0));((2.0,1.0),(5.0,3.0));((0.0,1.0),(0.0,10.0));((0.0,7.0),(7.0,8.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test94=skladaj op kartka;;
   assert (test94 (3.0,8.0)=0);;
   let op=[((3.0,10.0),(7.0,3.0));((6.0,8.0),(1.0,9.0));((9.0,8.0),(7.0,1.0));((10.0,5.0),(8.0,9.0));((6.0,0.0),(3.0,0.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test95=skladaj op kartka;;
   assert (test95 (0.0,9.0)=0);;
   let op=[((4.0,2.0),(4.0,7.0));((5.0,7.0),(3.0,0.0));((10.0,1.0),(0.0,2.0));((10.0,10.0),(2.0,0.0));((1.0,7.0),(0.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test96=skladaj op kartka;;
   assert (test96 (5.0,7.0)=0);;
   let op=[((0.0,4.0),(6.0,10.0));((2.0,4.0),(10.0,0.0));((0.0,3.0),(5.0,9.0));((4.0,0.0),(3.0,8.0));((4.0,3.0),(6.0,5.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test97=skladaj op kartka;;
   assert (test97 (1.0,4.0)=0);;
   let op=[((4.0,1.0),(7.0,2.0));((1.0,0.0),(5.0,0.0));((1.0,1.0),(8.0,2.0));((2.0,8.0),(7.0,4.0));((3.0,3.0),(2.0,6.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test98=skladaj op kartka;;
   assert (test98 (7.0,0.0)=0);;
   let op=[((0.0,5.0),(5.0,5.0));((2.0,1.0),(1.0,9.0));((2.0,5.0),(7.0,1.0));((6.0,7.0),(5.0,5.0));((8.0,0.0),(9.0,4.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test99=skladaj op kartka;;
   assert (test99 (4.0,2.0)=0);;



   let op=[((10.0,8.0),(2.0,4.0));((0.0,8.0),(6.0,1.0));((7.0,0.0),(9.0,2.0));((8.0,3.0),(0.0,8.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test0=skladaj op kartka;;
   assert (test0 (4.0,0.0)=0);;
   let op=[((8.0,6.0),(10.0,7.0));((7.0,2.0),(3.0,2.0));((9.0,0.0),(1.0,9.0));((2.0,5.0),(9.0,10.0))];;
   let kartka=prostokat (0.,0.) (10.,10.) ;;
   let test1=skladaj op kartka;;
   assert (test1 (9.0,7.0)=0);;
   let op=[((8.0,2.0),(2.0,5.0));((5.0,9.0),(5.0,1.0));((8.0,5.0),(7.0,8.0));((8.0,4.0),(4.0,0.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test2=skladaj op kartka;;
   assert (test2 (3.0,5.0)=0);;
   let op=[((1.0,3.0),(0.0,8.0));((1.0,5.0),(7.0,7.0));((8.0,7.0),(9.0,5.0));((2.0,4.0),(10.0,10.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test3=skladaj op kartka;;
   assert (test3 (3.0,8.0)=0);;
   let op=[((3.0,7.0),(1.0,1.0));((3.0,10.0),(2.0,3.0));((1.0,8.0),(4.0,7.0));((7.0,3.0),(3.0,8.0))];;
   let kartka=kolko (5.,5.) 4. ;;
   let test4=skladaj op kartka;;
   assert (test4 (3.0,7.0)=4);;



   let test a b msg = if a=b then print_endline "ok"
   else (print_int a; print_string "<>"; print_int b; print_string " test: "; print_endline msg);;


   let p1 = prostokat (0., 0.) (10., 10.)
   let k1 = kolko (5., 5.) 5.
   let l1 = [((0., 0.), (10., 10.));
   ((5., 0.), (10., 5.));
   ((10., 0.), (0., 10.));
   ((2.5, 0.), (2.5, 10.))];;
   let l2 = [((8., 0.), (10., 2.));
   ((6., 0.), (10., 4.));
   ((4., 0.), (10., 6.));
   ((2., 0.), (10., 8.));
   ((0., 0.), (10., 10.));
   ((0., 2.), (8., 10.));
   ((0., 4.), (6., 10.));
   ((0., 6.), (4., 10.));
   ((0., 8.), (2., 10.))];;

   let construct a b n =
   let rec pom (x1, y1) (x2, y2) n a =
    let sx = (x1 +. x2) /. 2. and sy = (y1 +. y2) /. 2. in
    if n=0 then ((sx, sy),a) else
      let (w,wyn) = pom (x1, y1) (sx, sy) (n - 1) a in
   (w,((sx, y1), (sx, y2))::(((sx, sy), (x1, sy))::wyn)) in
   pom a b n [];;

   let p2 = skladaj l1 p1
   let p3 = skladaj l2 p1
   let k2 = skladaj l1 k1;;

   test (p2 (7., 3.)) 0 "0.1: p2";;
   test (p2 (5., 8.)) 0 "0.2: p2";;
   test (p2 (3., 5.)) 0 "0.3: p2";;
   test (p2 (5., 5.)) 0 "0.4: p2";;
   test (p2 (0., 0.)) 2 "1: p2";;
   test (p2 (0., 10.)) 2  "2: p2";;
   test (p2 (2.5, 2.5)) 2 "3: p2";;
   test (p2 (2.5, 7.5)) 2 "4: p2";;
   test (p2 (2.5, 5.)) 4 "5: p2";;
   test (p2 (0., 5.)) 5 "6: p2";;
   test (p2 (1., 2.)) 4 "7: p2";;
   test (p2 (1., 5.)) 8 "8: p2";;
   test (p2 (1., 8.)) 4 "9: p2";;

   test (k2 (7., 3.)) 0 "0.1: k2";;
   test (k2 (5., 8.)) 0 "0.2: k2";;
   test (k2 (3., 5.)) 0 "0.3: k2";;
   test (k2 (5., 5.)) 0 "0.4: k2";;
   test (k2 (2.5, 2.5)) 2 "1: k2";;
   test (k2 (2.5, 7.5)) 2 "2: k2";;
   test (k2 (2.5, 5.)) 4 "3: k2";;
   test (k2 (0., 5.)) 5 "4: k2";;
   test (k2 (1., 3.)) 4 "5: k2";;
   test (k2 (1., 5.)) 8 "6: k2";;
   test (k2 (1., 7.)) 4 "7: k2";;

   test (p3 ((-4.), 6.)) 2 "1: p3";;
   test (p3 ((-3.), 5.)) 1 "2: p3";;
   test (p3 ((-3.), 7.)) 2 "3: p3";;
   test (p3 ((-2.), 6.)) 3 "4: p3";;
   test (p3 ((-2.5), 6.5)) 4 "5: p3";;
   test (p3 ((-2.), 8.)) 4 "6: p3";;
   test (p3 ((-1.), 7.)) 3 "7: p3";;
   test (p3 ((-1.5), 7.5)) 6 "8: p3";;
   test (p3 (0., 8.)) 5 "9: p3";;
   test (p3 ((-1.), 9.)) 4 "10: p3";;
   test (p3 ((-0.5), 8.5)) 8 "11: p3";;
   test (p3 (0., 10.)) 6 "12: p3";;
   test (p3 (1., 9.)) 5 "13: p3";;
   test (p3 (0.5, 9.5)) 10 "14: p3";;

   let (point, l) = construct (0., 0.) (10., 10.) 1;;
   let p = skladaj l p1;;
   test (p point) 4 "big 1";;

   let (point, l) = construct (0., 0.) (10., 10.) 2;;
   let p = skladaj l p1;;
   test (p point) 16 "big 2";;

   let (point, l) = construct (0., 0.) (10., 10.) 4;;
   let p = skladaj l p1;;
   test (p point)  (1 lsl 8) "big 3";;

   let (point, l) = construct (0., 0.) (10., 10.) 10;;
   let p = skladaj l p1;;
   test (p point) (1 lsl 20) "big 4";;


   let (point, l) = construct (0., 0.) (10., 10.) 15;;
   let p = skladaj l p1;;
   test (p point) (1 lsl 30) "big 4";;


   ( *)
