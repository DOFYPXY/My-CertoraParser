// slip
vars: bytes32 otherIlk, address otherUsr, uint256 gemBefore, uint256 gemOtherBefore, uint256 gemAfter, uint256 gemOtherAfter
spec: finished(Vat.slip(ilk, usr, wad), (otherIlk != ilk || otherUsr != usr) |=> (((gemBefore = old(gem(ilk,usr))) && (gemOtherBefore = old(gem(otherIlk,otherUsr))) && (gemAfter = (gem(ilk,usr))) && (gemOtherAfter = (gem(otherIlk,otherUsr)))) ==> ((to_mathint(gemAfter) = to_mathint(gemBefore) + to_mathint(wad)) && (gemOtherAfter = gemOtherBefore))))