PROGRAM Day7
  IMPLICIT NONE
  INTEGER(4) :: nSubs,i,tmp,score
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nSubs
  ALLOCATE(vals(nSubs))
  READ(98,*) vals

  score = 1e9
  DO i=1,MAXVAL(vals)
    !tmp = SUM(ABS(vals-i))
    tmp = SUM(ABS(vals-i)*(ABS(vals-i)+1)/2)
    IF (tmp < score) THEN
      score = tmp
    ENDIF
  ENDDO
  WRITE(*,*) score
ENDPROGRAM Day7
