// hope_revert_nece
vars: bool revert1
spec: finished(Vat.hope(usr), (~((revert1 = (value > 0)) ==> (revert1))) |=> true)