PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: nLines,i
  INTEGER(4),ALLOCATABLE :: vals(:),tmp(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines

  ALLOCATE(vals(nLines))
  READ(98,*) vals

  ALLOCATE(tmp(nLines-2))
  DO i=1,SIZE(vals)-2
    tmp(i)=SUM(vals(i:i+2))
  ENDDO
  vals = vals(:SIZE(vals)-2) + vals(2:SIZE(vals)-1) + vals(3:)
  WRITE(*,*) COUNT(vals(2:)-vals(:SIZE(vals)-2) > 0)
  DEALLOCATE(vals)
ENDPROGRAM Day1
