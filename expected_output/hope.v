// hope
vars: address otherFrom, address otherTo, uint256 canOtherBefore, uint256 canAfter, uint256 canOtherAfter
spec: finished(Vat.hope(usr), (otherFrom != sender || otherTo != usr) |=> (((canOtherBefore = old(can(otherFrom,otherTo))) && (canAfter = (can(sender,usr))) && (canOtherAfter = (can(otherFrom,otherTo)))) ==> ((canAfter = 1) && (canOtherAfter = canOtherBefore))))