// hope_revert_suff
vars: bool revert1
spec: reverted(Vat.hope(usr), ((revert1 = (value > 0)) ==> (revert1)))