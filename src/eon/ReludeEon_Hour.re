open Relude.Globals;

type t =
  | Hour(int);

module Eq: BsAbstract.Interface.EQ with type t = t = {
  type nonrec t = t;
  let eq = (Hour(a), Hour(b)) => Int.eq(a, b);
};

module Ord: BsAbstract.Interface.ORD with type t = t = {
  include Eq;
  let compare = (Hour(a), Hour(b)) => Int.compare(a, b);
};

module Bounded: BsAbstract.Interface.BOUNDED with type t = t = {
  include Ord;
  let bottom = Hour(0);
  let top = Hour(23);
};

// private, but useful internally
module BoundExtras = ReludeEon_Bounded.MakeExtras(Bounded);
module IntCompat = {
  type nonrec t = t;
  type other = int;
  let toOther = (Hour(v)) => v;
  let fromOther = v => Hour(v);
};

module WrappedExtras =
  ReludeEon_Bounded.MakeRingLikeExtras(
    Bounded,
    BsAbstract.Int.Euclidean_Ring,
    Int.Ord,
    IntCompat,
  );

let makeWrapped = i => WrappedExtras.wrappedFromRing(i);
let makeClamped = i => BoundExtras.clamp(Hour(i));
let make = i => BoundExtras.ensure(Hour(i));

let addHours = WrappedExtras.addWrapped;
let getHours = IntCompat.toOther;

let eq = Eq.eq;
let compare = Ord.compare;
include Relude_Extensions_Ord.Make(Ord);