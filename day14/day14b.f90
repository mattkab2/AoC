PROGRAM Day14
  IMPLICIT NONE
  INTEGER(4) :: nRules,i,j
  INTEGER(8) :: pairHist(26,26),histDelta(26,26),charHist(26)
  INTEGER(4),ALLOCATABLE :: ruleIn1(:),ruleIn2(:),ruleOut(:)
  CHARACTER(LEN=20) :: init
  CHARACTER(LEN=2) :: ri
  CHARACTER(LEN=1) :: ro
  REAL(8) :: start,finish

  OPEN(unit=98,file="input.dat")
  READ(98,*) nRules
  READ(98,*) init
  ALLOCATE(ruleIn1(nRules),ruleIn2(nRules),ruleOut(nRules))
  DO i=1,nRules
    READ(98,*) ri,ro
    ruleIn1(i) = ICHAR(ri(1:1))-64
    ruleIn2(i) = ICHAR(ri(2:2))-64
    ruleOut(i) = ICHAR(ro(1:1))-64
  ENDDO
  ! Initialize pairhist
  CALL CPU_TIME(start)
  pairHist=0
  DO i=1,LEN_TRIM(init)-1
    pairHist(ICHAR(init(i:i))-64,ICHAR(init(i+1:i+1))-64)=pairHist(ICHAR(init(i:i))-64,ICHAR(init(i+1:i+1))-64)+1
  ENDDO
  ! Evolve the histogram
  DO j=1,10
    histDelta=0
    DO i=1,nRules
      histDelta(ruleIn1(i),ruleIn2(i)) = histDelta(ruleIn1(i),ruleIn2(i)) - pairHist(ruleIn1(i),ruleIn2(i))
      histDelta(ruleIn1(i),ruleOut(i)) = histDelta(ruleIn1(i),ruleOut(i)) + pairHist(ruleIn1(i),ruleIn2(i))
      histDelta(ruleOut(i),ruleIn2(i)) = histDelta(ruleOut(i),ruleIn2(i)) + pairHist(ruleIn1(i),ruleIn2(i))
    ENDDO
    pairHist = pairHist + histDelta
  ENDDO
  ! Compute single elements from pairs
  charHist = MAX(SUM(pairHist,DIM=1),SUM(pairHist,DIM=2))
  IF (init(1:1)==init(LEN_TRIM(init):LEN_TRIM(init))) charHist(ICHAR(init(1:1))-64) = charHist(ICHAR(init(1:1))-64)+1
  CALL CPU_TIME(finish)
  WRITE(*,*) MAXVAL(charHist)-MINVAL(charHist,charHist/=0)
  WRITE(*,*) 1e6*(finish-start),"us"
  DO j=11,40
    histDelta=0
    DO i=1,nRules
      histDelta(ruleIn1(i),ruleIn2(i)) = histDelta(ruleIn1(i),ruleIn2(i)) - pairHist(ruleIn1(i),ruleIn2(i))
      histDelta(ruleIn1(i),ruleOut(i)) = histDelta(ruleIn1(i),ruleOut(i)) + pairHist(ruleIn1(i),ruleIn2(i))
      histDelta(ruleOut(i),ruleIn2(i)) = histDelta(ruleOut(i),ruleIn2(i)) + pairHist(ruleIn1(i),ruleIn2(i))
    ENDDO
    pairHist = pairHist + histDelta
  ENDDO
  ! Compute single elements from pairs
  charHist = MAX(SUM(pairHist,DIM=1),SUM(pairHist,DIM=2))
  IF (init(1:1)==init(LEN_TRIM(init):LEN_TRIM(init))) charHist(ICHAR(init(1:1))-64) = charHist(ICHAR(init(1:1))-64)+1
  CALL CPU_TIME(finish)
  WRITE(*,*) MAXVAL(charHist)-MINVAL(charHist,charHist/=0)
  WRITE(*,*) 1e6*(finish-start),"us"

  DEALLOCATE(ruleIn1,ruleIn2,ruleOut)
ENDPROGRAM Day14
