// deny_revert_suff
vars: uint256 ward, uint256 live, bool revert1, bool revert2, bool revert3
spec: reverted(Vat.deny(usr), (((ward = (wards(sender))) && (live = (live())) && (revert1 = (value > 0)) && (revert2 = (ward != 1)) && (revert3 = (live != 1))) ==> ((revert1) && (revert2) && (revert3))))