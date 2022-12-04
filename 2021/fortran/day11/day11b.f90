PROGRAM Day11
  IMPLICIT NONE
  INTEGER(4) :: n,m,t,i,j
  INTEGER(4),ALLOCATABLE :: vals(:,:)
  LOGICAL,ALLOCATABLE :: msk(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) n,m
  ALLOCATE(vals(n,m))
  ALLOCATE(msk(n,m))
  READ(98,*) vals

  t=0
  DO WHILE (.NOT. ALL(vals==0))
    t = t+1
    vals = vals+1
    DO WHILE(ANY(vals==10))
      DO i=1,n
        DO j=1,m
          IF (vals(i,j)==10) THEN
            msk = .FALSE.
            msk(MAX(1,i-1):MIN(i+1,n),MAX(1,j-1):MIN(j+1,m)) = .TRUE.
            WHERE(msk .AND. vals<10)  vals = vals+1
            vals(i,j) = vals(i,j)+1
          ENDIF
        ENDDO
      ENDDO
    ENDDO
    WHERE(vals>9) vals=0
  ENDDO
  WRITE(*,*) t
ENDPROGRAM Day11
