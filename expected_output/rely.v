// rely
vars: address other, uint256 wardOtherBefore, uint256 wardAfter, uint256 wardOtherAfter
spec: finished(Vat.rely(usr), (other != usr) |=> (((wardOtherBefore = old(wards(other))) && (wardAfter = (wards(usr))) && (wardOtherAfter = (wards(other)))) ==> ((wardAfter = 1) && (wardOtherAfter = wardOtherBefore))))