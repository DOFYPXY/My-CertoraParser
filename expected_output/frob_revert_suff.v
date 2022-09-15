// frob_revert_suff
vars: uint256 live, bool wishU, bool wishV, bool wishW, uint256 Line, uint256 Art, uint256 rate, uint256 spot, uint256 line, uint256 dust, uint256 ink, uint256 art, uint256 gem, uint256 dai, uint256 debt, mathint inkFinal, mathint artFinal, mathint ArtFinal, mathint debtFinal, bool revert1, bool revert2, bool revert3, bool revert4, bool revert5, bool revert6, bool revert7, bool revert8, bool revert9, bool revert10, bool revert11, bool revert12, bool revert13, bool revert14, bool revert15, bool revert16, bool revert17, bool revert18, bool revert19, bool revert20
spec: reverted(Vat.frob(i, u, v, w, dink, dart), (((live = (live())) && (wishU = (u = sender || can(u,sender) = 1)) && (wishV = (v = sender || can(v,sender) = 1)) && (wishW = (w = sender || can(w,sender) = 1)) && (Line = (Line())) && (Art = (ilks(i).0)) && (rate = (ilks(i).1)) && (spot = (ilks(i).2)) && (line = (ilks(i).3)) && (dust = (ilks(i).4)) && (ink = (urns(i,u).0)) && (art = (urns(i,u).1)) && (gem = (gem(i,v))) && (dai = (dai(w))) && (debt = (debt())) && (inkFinal = (to_mathint(ink) + to_mathint(dink))) && (artFinal = (to_mathint(art) + to_mathint(dart))) && (ArtFinal = (to_mathint(Art) + to_mathint(dart))) && (debtFinal = (to_mathint(debt) + to_mathint(rate) * to_mathint(dart))) && (revert1 = (value > 0)) && (revert2 = (live != 1)) && (revert3 = (rate = 0)) && (revert4 = (to_mathint(ink) + to_mathint(dink) < 0 || to_mathint(ink) + to_mathint(dink) > max_uint256)) && (revert5 = (to_mathint(art) + to_mathint(dart) < 0 || to_mathint(art) + to_mathint(dart) > max_uint256)) && (revert6 = (to_mathint(Art) + to_mathint(dart) < 0 || to_mathint(Art) + to_mathint(dart) > max_uint256)) && (revert7 = (rate > max_int256())) && (revert8 = (to_mathint(rate) * to_mathint(dart) < min_int256() || to_mathint(rate) * to_mathint(dart) > max_int256())) && (revert9 = (rate * artFinal > max_uint256)) && (revert10 = (to_mathint(debt) + to_mathint(rate) * to_mathint(dart) < 0 || to_mathint(debt) + to_mathint(rate) * to_mathint(dart) > max_uint256)) && (revert11 = (ArtFinal * rate > max_uint256)) && (revert12 = (dart > 0 && (ArtFinal * rate > line || debtFinal > Line))) && (revert13 = (inkFinal * spot > max_uint256)) && (revert14 = ((dart > 0 || dink < 0) && rate * artFinal > inkFinal * spot)) && (revert15 = ((dart > 0 || dink < 0) && !wishU)) && (revert16 = (dink > 0 && !wishV)) && (revert17 = (dart < 0 && !wishW)) && (revert18 = (artFinal > 0 && rate * artFinal < dust)) && (revert19 = (to_mathint(gem) - to_mathint(dink) < 0 || to_mathint(gem) - to_mathint(dink) > max_uint256)) && (revert20 = (to_mathint(dai) + to_mathint(rate) * to_mathint(dart) < 0 || to_mathint(dai) + to_mathint(rate) * to_mathint(dart) > max_uint256))) ==> ((revert1) && (revert2) && (revert3) && (revert4) && (revert5) && (revert6) && (revert7) && (revert8) && (revert9) && (revert10) && (revert11) && (revert12) && (revert13) && (revert14) && (revert15) && (revert16) && (revert17) && (revert18) && (revert19) && (revert20))))