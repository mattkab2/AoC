
PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: nLines,i,j,ret
  INTEGER(4),ALLOCATABLE :: vals(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines

  ALLOCATE(vals(nLines))
  DO i=1,nLines
    READ(98,*) vals(i)
  ENDDO

  DO i=1,nLines
    DO j=i,nLines
      IF (ANY(vals(j+1:)==2020-vals(i)-vals(j))) THEN
        ret = vals(i)*vals(j)*(2020-vals(i)-vals(j))
        EXIT
      ENDIF
    ENDDO
  ENDDO
  WRITE(*,*) ret
  DEALLOCATE(vals)

ENDPROGRAM Day1
