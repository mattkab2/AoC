
PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: nLines,i,ret
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines

  ALLOCATE(vals(nLines))
  DO i=1,nLines
    READ(98,*) vals(i)
  ENDDO

  DO i=1,nLines
    IF (ANY(vals(i+1:)==2020-vals(i))) THEN
      ret = vals(i)*(2020-vals(i))
      EXIT
    ENDIF
  ENDDO
  WRITE(*,*) ret
  DEALLOCATE(vals)

ENDPROGRAM Day1
