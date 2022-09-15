// cage_revert_nece
vars: uint256 ward, bool revert1, bool revert2
spec: finished(Vat.cage(), (~(((ward = (wards(sender))) && (revert1 = (value > 0)) && (revert2 = (ward != 1))) ==> (revert1 || revert2))) |=> true)