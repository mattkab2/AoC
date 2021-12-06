PROGRAM Day6
  IMPLICIT NONE
  INTEGER(8) :: hist(9)
  INTEGER(4) :: i,nFish
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nFish
  ALLOCATE(vals(nFish))
  READ(98,*) vals

  hist=0
  DO i=1,nFish
    hist(vals(i)+1) = hist(vals(i)+1)+1
  ENDDO
  DO i=1,256
    hist = CSHIFT(hist,1)
    hist(7) = hist(7) + hist(9)
  ENDDO
  WRITE(*,*) SUM(hist)
ENDPROGRAM Day6
