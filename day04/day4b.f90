PROGRAM Day4
  IMPLICIT NONE
  INTEGER(4) :: nBoards,j,i,k,nWins
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

  nWins=0
  DO i=1,100
    called = (called .OR. (boards==calls(i)))
    DO j=1,nBoards
      IF (boards(j,1,1)==-1) CYCLE
      DO k=1,5
        IF (ALL(called(j,k,:)) .OR. ALL(called(j,:,k))) THEN
          nWins = nWins + 1
          boards(j,1,1)= -1
          EXIT
        ENDIF
      ENDDO
    ENDDO
    IF (nWins == nBoards-1) EXIT
  ENDDO
  j = MAXLOC(boards(:,1,1),DIM=1)
  bingo: DO i=i,100
    called(j,:,:) = (called(j,:,:) .OR. (boards(j,:,:)==calls(i)))
    DO k=1,5
      IF (ALL(called(j,k,:))) EXIT bingo
      IF (ALL(called(j,:,k))) EXIT bingo
    ENDDO
  ENDDO bingo
  WRITE(*,*) SUM(boards(j,:,:),.NOT. called(j,:,:))*calls(i)
ENDPROGRAM Day4
