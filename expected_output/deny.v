// deny
vars: address other, uint256 wardOtherBefore, uint256 wardAfter, uint256 wardOtherAfter
spec: finished(Vat.deny(usr), (other != usr) |=> (((wardOtherBefore = old(wards(other))) && (wardAfter = (wards(usr))) && (wardOtherAfter = (wards(other)))) ==> ((wardAfter = 0) && (wardOtherAfter = wardOtherBefore))))