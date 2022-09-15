// slip_revert_suff
vars: uint256 ward, uint256 gem, bool revert1, bool revert2, bool revert3
spec: reverted(Vat.slip(ilk, usr, wad), (((ward = (wards(sender))) && (gem = (gem(ilk,usr))) && (revert1 = (value > 0)) && (revert2 = (ward != 1)) && (revert3 = (to_mathint(gem) + to_mathint(wad) < 0 || to_mathint(gem) + to_mathint(wad) > max_uint256))) ==> ((revert1) && (revert2) && (revert3))))