// file_revert_suff
vars: uint256 ward, uint256 live, bool revert1, bool revert2, bool revert3, bool revert4
spec: reverted(Vat.file(what, data), (((ward = (wards(sender))) && (live = (live())) && (revert1 = (value > 0)) && (revert2 = (ward != 1)) && (revert3 = (live != 1)) && (revert4 = (what != 0x4c696e6500000000000000000000000000000000000000000000000000000000))) ==> ((revert1) && (revert2) && (revert3) && (revert4))))