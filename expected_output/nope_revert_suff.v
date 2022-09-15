// nope_revert_suff
vars: bool revert1
spec: reverted(Vat.nope(usr), ((revert1 = (value > 0)) ==> (revert1)))