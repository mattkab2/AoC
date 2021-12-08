PROGRAM Day8
  IMPLICIT NONE
  INTEGER(4) :: nLines,ret,i,j,tmp
  CHARACTER(LEN=7) :: junk(10)
  CHARACTER(LEN=7),ALLOCATABLE :: vals(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(vals(nLines,4))
  DO i=1,nLines
    READ(98,*) junk,vals(i,:)
  ENDDO
  ret = 0
  DO i=1,nLines
    DO j=1,4
      tmp = LEN_TRIM(vals(i,j))
      IF (tmp==2 .OR. tmp==4 .OR. tmp==3 .OR. tmp==7) ret = ret+1
    ENDDO
  ENDDO
  WRITE(*,*) ret
ENDPROGRAM Day8
