// fold_revert_suff
vars: uint256 ward, uint256 live, uint256 Art, uint256 rate, uint256 spot, uint256 line, uint256 dust, uint256 dai, uint256 debt, mathint rad, bool revert1, bool revert2, bool revert3, bool revert4, bool revert5, bool revert6, bool revert7, bool revert8
spec: reverted(Vat.fold(i, u, rate_), (((ward = (wards(sender))) && (live = (live())) && (Art = (ilks(i).0)) && (rate = (ilks(i).1)) && (spot = (ilks(i).2)) && (line = (ilks(i).3)) && (dust = (ilks(i).4)) && (dai = (dai(u))) && (debt = (debt())) && (rad = (to_mathint(Art) * to_mathint(rate_))) && (revert1 = (value > 0)) && (revert2 = (ward != 1)) && (revert3 = (live != 1)) && (revert4 = (to_mathint(rate) + to_mathint(rate_) < 0 || to_mathint(rate) + to_mathint(rate_) > max_uint256)) && (revert5 = (Art > max_int256())) && (revert6 = (to_mathint(Art) * to_mathint(rate_) < min_int256() || to_mathint(Art) * to_mathint(rate_) > max_int256())) && (revert7 = (to_mathint(dai) + rad < 0 || to_mathint(dai) + rad > max_uint256)) && (revert8 = (to_mathint(debt) + rad < 0 || to_mathint(debt) + rad > max_uint256))) ==> ((revert1) && (revert2) && (revert3) && (revert4) && (revert5) && (revert6) && (revert7) && (revert8))))