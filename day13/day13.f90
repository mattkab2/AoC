PROGRAM Day13
  IMPLICIT NONE
  INTEGER(4) :: nPts,nInstr,foldLine,i,k,n,m
  INTEGER(4),ALLOCATABLE :: idx(:),jdx(:)
  LOGICAL :: xDir
  LOGICAL,ALLOCATABLE :: folded(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nPts
  READ(98,*) nInstr
  ALLOCATE(idx(nPts),jdx(nPts))
  DO i=1,nPts
    READ(98,*) jdx(i),idx(i)
  ENDDO
  idx = idx+1
  jdx = jdx+1

  !nInstr=1
  DO k=1,nInstr
    READ(98,*) xDir,foldLine
    foldLine = foldLine+1
    IF (xDir) THEN
      DO i=1,nPts
        WHERE (jdx > foldLine) jdx = 2*foldLine - jdx
      ENDDO
    ELSE
      DO i=1,nPts
        WHERE (idx > foldLine) idx = 2*foldLine - idx
      ENDDO
    ENDIF
  ENDDO
  m=MAXVAL(jdx)
  n=MAXVAL(idx)
  ALLOCATE(folded(n,m))
  folded=.FALSE.
  DO i=1,nPts
    folded(idx(i),jdx(i)) = .TRUE.
  ENDDO
  WRITE(*,*) COUNT(folded)
  IF (nInstr>1) THEN
    DO i=1,n
      DO k=1,m
        IF (folded(i,k)) THEN
          WRITE(*,'(A1)',advance='no') '#'
        ELSE
          WRITE(*,'(A1)',advance='no') ' '
        ENDIF
      ENDDO
      WRITE(*,*) " "
    ENDDO
  ENDIF
ENDPROGRAM Day13
