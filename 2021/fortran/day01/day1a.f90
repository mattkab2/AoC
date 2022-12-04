PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: nLines
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(vals(nLines))
  READ(98,*) vals
  WRITE(*,*) COUNT(vals(2:)-vals(:SIZE(vals)-2) > 0)
  DEALLOCATE(vals)

ENDPROGRAM Day1
