(** Autor:      Michał Balcerzak  (mb385130) *)
(** Recenzent:  Piotr Styczyński  (ps322756) *)

(** Implementacja Arytmetyki niedokladnych wartości *)


type wartosc =                                                                  (*   para (x, y)                            *)
  | Set of float*float                                                          (* przedział od x do y (Zał. (x<=y))        *)
  | Antiset of float*float                                                      (* R minus przedział od y do x (Zał (x>y))  *
                                                                                 * alternatywnie [x; inf] \sum [-inf; y]    *)


(** konstruktory **)

(* wartosc_dokladnosc x p = x +/- p% *)
(* war.pocz.: p > 0                  *)
let wartosc_dokladnosc (x:float) (p:float) =
  let procencik = (x *. p) /. 100.
  in let dolna_granica = (min (x-.procencik) (x+.procencik))
  and gorna_granica = (max (x-.procencik) (x+.procencik))
  in Set (dolna_granica, gorna_granica)

(* wartosc_od_do x y = [x;y]         *)
(* war.pocz.: x <= y                 *)
let wartosc_od_do (x:float) (y:float) =
  if (x>y) then Antiset (x, y)                                                  (* for testing purposes only *)
  else Set (x, y)

(* wartosc_dokladna x = [x;x]        *)
let wartosc_dokladna (x:float) =
  Set (x, x)


(** metody zwracające informacje o własnościach wartości **)

(* in_wartosc w x = x \in w *)
let in_wartosc (w:wartosc) (x:float) =
  match w with
  | Set (a, b) -> ((a<=x) && (x<=b))
  | Antiset (a, b) -> ((x<=b) || (a<=x))

(* min_wartosc w = najmniejsza możliwa wartość w,   *)
(* lub neg_infinity jeśli brak dolnego ograniczenia.*)
let min_wartosc (w:wartosc) =
  match w with
  | Set (x, _) -> x
  | Antiset (_, _) -> neg_infinity

(* max_wartosc w = największa możliwa wartość w,    *)
(* lub infinity jeśli brak górnego ograniczenia.    *)
let max_wartosc (w:wartosc) =
  match w with
  | Set (_, x) -> x
  | Antiset (_, _) -> infinity

(* środek przedziału od min_wartosc do max_wartosc, *)
(* lub nan jeśli min i max_wartosc nie są określone.*)
let sr_wartosc (w:wartosc) =
  match w with
  | Antiset (_, _) -> nan
  | Set (x, y) ->
    let aver = (x +. y)/.2.
    in aver


(** funkcje pomocnicze **)

let czy_nieskonczony_set o =
  match o with
  | Set (a, b) ->
    if (a=neg_infinity || b=infinity) then true else false
  | _ -> false

let czy_zero_w_antysecie o =
  in_wartosc o (0.+.epsilon_float) && in_wartosc o (0.-.epsilon_float)          (* wtedy współrzędne antiseta są jednoznakowe *)

let czy_pusty_set o =
  match o with
  | Set (x, y) ->
    if classify_float x=FP_nan || classify_float y=FP_nan then true else false
  | Antiset (x, y) ->
    if classify_float x=FP_nan || classify_float y=FP_nan then true else false

(* sprawdza, czy wartość spełnia założenia typu i dokonuje ewentualnych korekt *)
let normalizer (a:wartosc) =
  match a with
  | Set (x, y) ->
    if x<=y then a
    else if (x>y) then Antiset (x, y)
    else Set (nan, nan)
  | Antiset (x, y) ->
    if czy_pusty_set a then Set (nan, nan)
    else if x>y then a
    else Set (neg_infinity, infinity)

(* zwraca max albo min z listy elementów *)
let functionator (f) (a) =
  let rec pom localmax leftlist =
    match leftlist with
    | [] -> localmax
    | h::t ->
      if (classify_float h=FP_nan) then pom localmax t
      else pom (f localmax h) t
  in let poczatek = (0.-.(f neg_infinity infinity))
  in if (pom poczatek a)=poczatek then 0.
  else  (pom poczatek a)


(** Operacje arytmetyczne na niedokładnych wartościach. **)

let plus (a:wartosc) (b:wartosc) =
  let rec pom j k =
    match j, k with
    | Set (p, q), Set (x, y) -> Set ((p+.x), (q+.y))
    | Antiset (_, _), Antiset (_, _) -> Set (neg_infinity, infinity)
    | Set (p, q), Antiset (x, y) -> Antiset ((p+.x), (q+.y))
    | _, _ -> pom k j
  in normalizer (pom a b)

let minus (a:wartosc) (b:wartosc) =
  match b with
  | Set (x, y) -> plus a (Set ((-.y), (-.x)))
  | Antiset (x, y) -> plus a (Antiset ((-.y), (-.x)))

let razy (a:wartosc) (b:wartosc) =
  let rec pom j k =
    match j, k with
    | Set (p, q), Set (x, y) ->
      if czy_pusty_set j || czy_pusty_set k then Set (nan, nan)
      else
        let values = [p*.x; p*.y; q*.x; q*.y]
        in Set ((functionator (min) values), (functionator (max) values))
    | Antiset (p, q), Antiset (x, y) ->
      if (in_wartosc a 0.)||(in_wartosc b 0.) then
        Set (neg_infinity, infinity)
      else                                                                      (* skoro nie ma 0 to x,p > 0 i y,q < 0 *)
        Antiset ((min (p*.x) (q*.y)), (max (q*.x) (p*.y)))
    | Antiset (p, q), Set (x, y) ->
      if czy_pusty_set k then Set (nan, nan)
      else if k = Set (0., 0.) then Set (0., 0.)
      else if in_wartosc k 0. = true then Set (neg_infinity, infinity)          (* skoro 0 nie w secie to x, y są jednego znaku *)
      else if czy_zero_w_antysecie j && czy_nieskonczony_set k then Set (neg_infinity, infinity)

      else if y=infinity then Antiset ((p*.x), (q*.x))
      else if x=neg_infinity then Antiset ((q*.y), (p*.y))

      else if x>0. then Antiset ((min (p*.x) (p*.y)), (max (q*.x) (q*.y)))
      else Antiset ((min (q*.x) (q*.y)), (max (p*.x) (p*.y)))
    | _, _ -> pom k j
  in normalizer (pom a b)

let podzielic (a:wartosc) (b:wartosc) =
  match b with
  | Set (0., 0.) -> Set (nan, nan)
  | Set (x, 0.) -> razy a (Set (neg_infinity, (1./.x)))
  | Set (0., y) -> razy a (Set ((1./.y), infinity))
  | Set (x, y) ->
    if (in_wartosc b 0.)=true then razy a (Antiset (((1./.y), (1./.x))))
    else razy a (Set ((1./.y), (1./.x)))

  | Antiset (x, 0.) -> razy a (Set (neg_infinity, (1./.x)))
  | Antiset (0., y) -> razy a (Set ((1./.y), infinity))
  | Antiset (x, y) ->
    if (in_wartosc b 0.)=true then razy a (Antiset ( (1./.y), (1./.x)))
    else razy a (Set ((1./.y), (1./.x)))

(*
    (* test zone *)

    (* funkcja wypis by ps322756 *)

    let wypis (a:wartosc) =
      let epsilon = epsilon_float in
      let (>==) a b = ((a-.epsilon*.10.0)>=b) || ((a+.epsilon*.10.0)>=b) in
      let (<==) a b = ((a-.epsilon*.10.0)<=b) || ((a+.epsilon*.10.0)<=b) in
      let (===) a b = (a >== b) && (a <== b) in
      let normliczbe a = if a === 0.0 then 0.0 else a in
      match a with
      | Set (x, y) ->
        Printf.printf " <%8.3f   ;%8.3f>  " (normliczbe x) (normliczbe y)
      | Antiset (x, y) ->
        Printf.printf " (%8.3f  >;<%7.3f)  " (normliczbe y) (normliczbe x)




    (* SPRAWDZARKA as made famous by MBalc385130  *)



    let antyprzedzial = (podzielic (wartosc_dokladna 1.) (wartosc_od_do (-.1.) 1.))
    let antyprzedzial2 = podzielic antyprzedzial (wartosc_dokladna 2.)
    let antyprzedzial5 = razy antyprzedzial2 (wartosc_dokladna 5.)
    let wszystkieliczby = podzielic (wartosc_od_do (-1000.) 1000.) (wartosc_od_do (-1000.) 1000.)

    let tab_arguments =
      let a = wartosc_od_do (-1.) 1.               (* <-1, 1> *)
      in let b = wartosc_dokladna (-1.)            (* <1, 1> *)
      in let c = podzielic b a                     (* (-inf -1> U <1 inf) *)
      in let d = plus c a                          (* (-inf, inf) *)
      in let e = wartosc_dokladna 0.               (* <0, 0> *)
      in let f = razy c e                          (* <0, 0> *)
      in let g = razy d e                          (* <0, 0> *)
      in let h = wartosc_dokladnosc (-10.) 50.     (* <-15, -5> *)
      in let i = podzielic h e                     (* nan, przedzial pusty*)
      in let j = wartosc_od_do (-6.) 5.            (* <-6, 5> *)
      in let k = razy j j                          (* <-30, 36> *)
      in let l = plus a b                          (* <-2, 0> *)
      in let m = razy b l                          (* <0, -2> *)
      in let n = podzielic l l                     (* <0, inf) *)
      in let o = podzielic l m                     (* (-inf, 0) *)
      in let p = razy o a                          (* (-inf, inf) *)
      in let q = plus n o                          (* (-inf, inf) *)
      in let r = minus n n                         (* (-inf, inf) *)
      in let s = wartosc_dokladnosc (-0.0001) 100. (* <-0.0002, 0> *)
      in let t = razy n s                          (* (-inf, 0) *)
      in [
        wartosc_dokladnosc 1. 100.                      ;                           (* S: +0 +2 *)
        wartosc_dokladnosc 2.5 20.                      ;                           (* S: +2 +3 *)
        wartosc_dokladnosc (-.1.) 100.                  ;                           (* S: -2 +0 *)
        wartosc_dokladnosc (-.2.5) 20.                  ;                           (* S: -3 -2 *)
        wartosc_dokladnosc (-.0.5) 500.                 ;                           (* S: -3 +2 *)
        wartosc_dokladna 0.                             ;                           (* S: +0 +0 *)
        wartosc_dokladna 5.                             ;                           (* S: +5 +5 *)
        wartosc_dokladna (-.7.)                         ;                           (* S: -7 -7 *)

        plus (wartosc_dokladna 1.) antyprzedzial        ;                           (* A: +0 +2 *)
        plus (wartosc_dokladna 2.5) antyprzedzial2      ;                           (* A: +2 +3 *)
        plus (wartosc_dokladna (-.1.)) antyprzedzial    ;                           (* A: -2 +0 *)
        plus (wartosc_dokladna (-.4.5)) antyprzedzial5  ;                           (* A: -7 -2 *)
        plus (wartosc_dokladna (-.0.5)) antyprzedzial5  ;                           (* A: -3 +2 *)

        a                                               ;                           (* <-1, 1> *)
        b                                               ;                           (* <1, 1> *)
        c                                               ;                           (* (-inf -1> U <1 inf) *)
        d                                               ;                           (* (-inf, inf) *)
        e                                               ;                           (* <0, 0> *)
        f                                               ;                           (* <0, 0> *)
        g                                               ;                           (* <0, 0> *)
        h                                               ;                           (* <-15, -5> *)
        i                                               ;                           (* nan, przedzial pusty*)
        j                                               ;                           (* <-6, 5> *)
        k                                               ;                           (* <-30, 36> *)
        l                                               ;                           (* <-2, 0> *)
        m                                               ;                           (* <0, -2> *)
        n                                               ;                           (* <0, inf) *)
        o                                               ;                           (* (-inf, 0) *)
        p                                               ;                           (* (-inf, inf) *)
        q                                               ;                           (* (-inf, inf) *)
        r                                               ;                           (* (-inf, inf) *)
        s                                               ;                           (* <-0.0002, 0> *)
        t                                                                           (* (-inf, 0) *)
      ]

    let helpere f s =                                                               (* daje output dla wszystkich kombinacji elementów z listy tab_arguments dla funkcji dwuargumentowej f *)
      let rec pom i made =
        let rec pom2 j left now =
          match left with
          | [] -> ()
          | h::t -> (
              (*Printf.printf "if (min_wartosc(%s (List.nth tab_arguments %d) (List.nth tab_arguments %d)))=%f" s i j 0.;*)
              wypis now;
              Printf.printf "%s " s;
              wypis h;
              Printf.printf "equals ";
              wypis (f now h);
              Printf.printf "\n";
              pom2 (j+1) t now
            )
        in match made with
        | [] -> ()
        | h::t -> (
            pom2 0 tab_arguments h;
            pom (i+1) t
          )
      in pom 0 tab_arguments


    let _ =
      let var1 = List.nth tab_arguments 0
      and var2 = List.nth tab_arguments 1
      and var3 = List.nth tab_arguments 2
      and var4 = List.nth tab_arguments 3
      and var5 = List.nth tab_arguments 4
      and var6 = List.nth tab_arguments 5
      and var7 = List.nth tab_arguments 6
      and var8 = List.nth tab_arguments 7

      and rav1 = List.nth tab_arguments 8
      and rav2 = List.nth tab_arguments 9
      and rav3 = List.nth tab_arguments 10
      and rav4 = List.nth tab_arguments 11
      and rav5 = List.nth tab_arguments 12

      and out500 = wartosc_dokladnosc 2. 100.
      and out516 = wartosc_od_do 4. 9.
      and out532 = wartosc_od_do 0. 4.
      and out548 = wartosc_od_do 4. 9.
      and out564 = wartosc_od_do (-.6.) 9.
      and out580 = wartosc_dokladna 0.
      and out596 = wartosc_dokladna 25.
      and out612 = wartosc_dokladna 49.

      and out628 = wszystkieliczby
      and out644 = wszystkieliczby
      and out660 = wszystkieliczby
      and out676 = wszystkieliczby
      and out692 = minus (razy antyprzedzial5 (wartosc_dokladna 2.)) (wartosc_dokladna 1.)



      in

      helpere plus "{+(PLUS)+}";
      helpere minus "{-(MINUS)-}";
      helpere razy "{*(RAZY)*}";
      helpere podzielic "{/(DIVID)/}";                                              (* outy można porównać jakimś diff-toolem, gdy mamy napisaną odpowiednią funkcję wypis *)

      assert ((in_wartosc var1 0.)=true);
      assert ((in_wartosc var2 0.)=false);
      assert ((in_wartosc var3 0.)=true);
      assert ((in_wartosc var4 0.)=false);
      assert ((in_wartosc var5 0.)=true);
      assert ((in_wartosc var6 0.)=true);
      assert ((in_wartosc var7 0.)=false);
      assert ((in_wartosc var8 0.)=false);

      assert ((in_wartosc rav5 0.)=false);
      assert ((in_wartosc rav4 0.)=true);
      assert ((in_wartosc rav3 0.)=true);
      assert ((in_wartosc rav2 0.)=true);
      assert ((in_wartosc rav1 0.)=true);
      assert (var4 = wartosc_od_do (-.3.) (-.2.));



      assert (razy var1 var1 = out500);
      assert (razy var2 var2 = out516);
      assert (razy var3 var3 = out532);
      assert (razy var4 var4 = out548);
      assert (razy var5 var5 = out564);
      assert (razy var6 var6 = out580);
      assert (razy var7 var7 = out596);
      assert (razy var8 var8 = out612);

      assert (razy rav1 rav1 = out628);
      assert (razy rav2 rav2 = out644);
      assert (razy rav3 rav3 = out660);
      assert (razy rav4 rav4 = out676);
      assert (razy rav5 rav5 = out692);

      assert (wartosc_od_do 0. 0. = wartosc_dokladna 0.);
      assert ((razy (wartosc_dokladna 0.) (podzielic (wartosc_od_do 1. 2.) (wartosc_od_do (-.1.) (1.)))) = (wartosc_dokladna 0.));
      assert (razy (wartosc_dokladna 0.) (wartosc_od_do (1.) (2.)) = wartosc_dokladna 0.);
      assert ((razy (wartosc_od_do (-.7.) (-.3.))(wartosc_od_do (-.7.) (-.3.)))=wartosc_od_do 9. 49.);
      assert ((razy (wartosc_od_do (-.7.) (-.3.))(wartosc_od_do (-.3.) (2.)))=wartosc_od_do (-.14.) (21.))

    (* testpack by Cyryll *)

    let a = wartosc_od_do (-1.) 1.            (* <-1, 1> *)
    let b = wartosc_dokladna (-1.)            (* <1, 1> *)
    let c = podzielic b a                     (* (-inf -1> U <1 inf) *)
    let d = plus c a                          (* (-inf, inf) *)
    let e = wartosc_dokladna 0.               (* <0, 0> *)
    let f = razy c e                          (* <0, 0> *)
    let g = razy d e                          (* <0, 0> *)
    let h = wartosc_dokladnosc (-10.) 50.     (* <-15, -5> *)
    let i = podzielic h e                     (* nan, przedzial pusty*)
    let j = wartosc_od_do (-6.) 5.            (* <-6, 5> *)
    let k = razy j j                          (* <-30, 36> *)
    let l = plus a b                          (* <-2, 0> *)
    let m = razy b l                          (* <0, -2> *)
    let n = podzielic l l                     (* <0, inf) *)
    let o = podzielic l m                     (* (-inf, 0) *)
    let p = razy o a                          (* (-inf, inf) *)
    let q = plus n o                          (* (-inf, inf) *)
    let r = minus n n                         (* (-inf, inf) *)
    let s = wartosc_dokladnosc (-0.0001) 100. (* <-0.0002, 0> *)
    let t = razy n s                          (* (-inf, 0) *)
    ;;

    assert ((min_wartosc c, max_wartosc c) = (neg_infinity, infinity));
    (* assert ((sr_wartosc c) == nan) *) (*OCaml ma kilka różnych rodzajów nan, nawet jak funkcja zwraca nan to ten test się krzeczy, 30 minut debugowania*)
    assert (compare (sr_wartosc c) nan = 0); (*compare a b = {1 gdy a>b, 0 gdy a=b, -1 gdy a<b}, w przeciwienstwie do "==" "compare" ogarnia porownywanie nan*)
    assert (in_wartosc c 0. = false);
    assert ((in_wartosc c (-1.)) && (in_wartosc c (-100000.)) && (in_wartosc c 1.) && (in_wartosc c 100000.));
    assert ((in_wartosc d 0.) && (in_wartosc d (-1.)) && (in_wartosc d (-100000.)) && (in_wartosc d 1.) && (in_wartosc d 100000.));
    assert ((min_wartosc f, max_wartosc f, sr_wartosc f) = (0., 0., 0.));
    assert ((min_wartosc g, max_wartosc g, sr_wartosc g) = (0., 0., 0.));
    assert ((min_wartosc h, max_wartosc h, sr_wartosc h) = (-15., -5., -10.));
    assert ((compare nan (min_wartosc i), compare nan (sr_wartosc i), compare nan (max_wartosc i)) = (0, 0, 0));
    assert ((min_wartosc k, max_wartosc k, sr_wartosc k) = (-30., 36., 3.));
    assert ((min_wartosc n, max_wartosc n, sr_wartosc n) = (0., infinity, infinity));
    assert ((min_wartosc o, max_wartosc o, sr_wartosc o) = (neg_infinity, 0., neg_infinity));
    assert ((min_wartosc p, max_wartosc p, compare (sr_wartosc p) nan) = (neg_infinity, infinity, 0));
    assert ((min_wartosc q, max_wartosc q, compare (sr_wartosc q) nan) = (neg_infinity, infinity, 0));
    assert ((min_wartosc r, max_wartosc r, compare (sr_wartosc r) nan) = (neg_infinity, infinity, 0));
    assert ((min_wartosc t, max_wartosc t, sr_wartosc t) = (neg_infinity, 0., neg_infinity));
*)
