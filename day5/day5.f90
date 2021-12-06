PROGRAM Day5
  IMPLICIT NONE
  INTEGER(4) :: nLines,i,j,k,n,ret
  INTEGER(4),ALLOCATABLE :: vals(:,:)
  LOGICAL :: dupFlag,inFlag

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(vals(4,nLines))
  READ(98,*) vals
  ret = 0
  n = MAXVAL(vals)
  DO i=0,n
    DO j=0,n
      dupFlag = .FALSE.
      DO k=1,nLines
        IF ((i==vals(1,k) .AND. j==vals(2,k)) .OR. (i==vals(3,k) .AND. j==vals(4,k))) THEN
          inFlag = .TRUE.
        ELSEIF (vals(1,k)==vals(3,k)) THEN
          inFlag = ((i==vals(1,k)) .AND. (j>=MIN(vals(2,k),vals(4,k))) .AND. (j<=MAX(vals(2,k),vals(4,k))))
        ELSEIF (vals(2,k)==vals(4,k)) THEN
          inFlag = ((j==vals(2,k)) .AND. (i>=MIN(vals(1,k),vals(3,k))) .AND. (i<=MAX(vals(1,k),vals(3,k))))
        ELSEIF (ABS(vals(1,k)-vals(3,k)) == ABS(vals(2,k)-vals(4,k))) THEN
          inFlag = ((SIGN(1,i-vals(1,k))==SIGN(1,vals(3,k)-vals(1,k))) .AND. (SIGN(1,j-vals(2,k))==SIGN(1,vals(4,k)-vals(2,k))))
          inFlag = (inFlag .AND. (ABS(i-vals(1,k))==ABS(j-vals(2,k))))
          inFlag = (inFlag .AND. (ABS(i-vals(1,k))<=ABS(vals(1,k)-vals(3,k))))
          inFlag = (inFlag .AND. (ABS(j-vals(2,k))<=ABS(vals(2,k)-vals(4,k))))
        ELSE
          WRITE(*,*) "ERR"
          CYCLE
        ENDIF
        IF (inFlag .AND. dupFlag) THEN
          ret = ret+1
          EXIT
        ENDIF
        IF (inFlag .AND. .NOT. dupFlag) dupFlag = .TRUE. 
      ENDDO
    ENDDO
  ENDDO
  WRITE(*,*) ret
ENDPROGRAM Day5
