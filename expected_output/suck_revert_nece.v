// suck_revert_nece
vars: uint256 ward, uint256 sin, uint256 dai, uint256 vice, uint256 debt, bool revert1, bool revert2, bool revert3, bool revert4, bool revert5, bool revert6
spec: finished(Vat.suck(u, v, rad), (~(((ward = (wards(sender))) && (sin = (sin(u))) && (dai = (dai(v))) && (vice = (vice())) && (debt = (debt())) && (revert1 = (value > 0)) && (revert2 = (ward != 1)) && (revert3 = (sin + rad > max_uint256)) && (revert4 = (dai + rad > max_uint256)) && (revert5 = (vice + rad > max_uint256)) && (revert6 = (debt + rad > max_uint256))) ==> (revert1 || revert2 || revert3 || revert4 || revert5 || revert6))) |=> true)