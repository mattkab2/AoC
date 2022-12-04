PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: nLines
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(vals(nLines))

  ! TODO Stuff here

ENDPROGRAM Day1
