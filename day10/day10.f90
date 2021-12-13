PROGRAM Day10
  IMPLICIT NONE
  INTEGER(4) :: i,j,k,ierr,stackPtr,ret,stack(64),scores(4)
  INTEGER(8) :: completeScores(94)
  CHARACTER(LEN=128) :: input
  CHARACTER(LEN=1) :: opener(4),closer(4)
  LOGICAL,ALLOCATABLE :: msk(:)
  opener = (/'(','[','{','<'/)
  closer = (/')',']','}','>'/)
  scores = (/3,57,1197,25137/)

  ret = 0
  j=0
  completeScores=0
  OPEN(unit=98,file="input.txt")
  parseLine: DO WHILE (.TRUE.)
    READ(98,*,IOSTAT=ierr) input 
    IF (ierr/=0) EXIT
    j = j+1
    stackPtr = 0

    DO i=1,LEN_TRIM(input)
      k = FINDLOC(opener,input(i:i),DIM=1)
      ! If k/=0 stack push
      IF (k/=0) THEN
        stackPtr = stackPtr + 1
        stack(stackPtr) = k
        CYCLE
      ENDIF
      k = FINDLOC(closer,input(i:i),DIM=1)
      IF (k==0) EXIT parseLine
      ! Stack pop; if the top of the stack does not match, error
      IF (stack(stackPtr)==k) THEN
        stackPtr = stackPtr-1
      ELSE
        ret = ret+scores(k)
        CYCLE parseLine
      ENDIF
    ENDDO
    ! Compute autocomplete score
    DO k=stackPtr,1,-1
      completeScores(j) = 5*completeScores(j) + stack(k)
    ENDDO
  ENDDO parseLine
  WRITE(*,*) "Err Score: ",ret

  i = COUNT(completeScores>0)
  ALLOCATE(msk(i))
  msk = .TRUE.
  completeScores = PACK(completeScores,completeScores>0)
  DO WHILE (COUNT(completeScores(1:i)>0)>1)
    j = MAXLOC(completeScores(1:i),DIM=1,MASK=msk)
    msk(j)=.FALSE.
    completeScores(j)=0
    j = MINLOC(completeScores(1:i),DIM=1,MASK=msk)
    msk(j)=.FALSE.
    completeScores(j)=0
  ENDDO
  WRITE(*,*) "Autocomplete Score: ",completeScores(FINDLOC(msk,.TRUE.,DIM=1))
ENDPROGRAM Day10
