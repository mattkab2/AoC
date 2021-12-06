PROGRAM Day4
  IMPLICIT NONE
  INTEGER(4) :: nBoards,j,i,k
  INTEGER(4) :: calls(100)
  INTEGER(4),ALLOCATABLE :: boards(:,:,:)
  LOGICAL,ALLOCATABLE :: called(:,:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nBoards
  READ(98,*) calls
  ALLOCATE(boards(nBoards,5,5),called(nBoards,5,5))
  DO i=1,nBoards
    READ(98,*)
    READ(98,*) boards(i,:,:)
  ENDDO
  called = .FALSE.

  bingo: DO i=1,100
    called = (called .OR. (boards==calls(i)))
    IF (i>=5) THEN
      DO j=1,nBoards
        DO k=1,5
          IF (ALL(called(j,k,:))) EXIT bingo
          IF (ALL(called(j,:,k))) EXIT bingo
        ENDDO
      ENDDO
    ENDIF
  ENDDO bingo
  WRITE(*,*) SUM(boards(j,:,:),.NOT. called(j,:,:))*calls(i)

ENDPROGRAM Day4
