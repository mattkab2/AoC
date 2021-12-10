MODULE Utils
  IMPLICIT NONE
  CONTAINS
  PURE RECURSIVE SUBROUTINE rCount(i,j,vals,msk,cnt)
    INTEGER(4),INTENT(IN) :: i,j,vals(:,:)
    LOGICAL,INTENT(INOUT) :: msk(:,:)
    INTEGER(4),INTENT(INOUT) :: cnt
    IF (i<1 .OR. i>SIZE(msk,DIM=1) .OR. j<1 .OR. j>SIZE(msk,DIM=2)) RETURN
    IF (msk(i,j)) RETURN
    cnt = cnt + 1
    msk(i,j) = .TRUE.
    CALL rCount(i+1,j,vals,msk,cnt)
    CALL rCount(i-1,j,vals,msk,cnt)
    CALL rCount(i,j+1,vals,msk,cnt)
    CALL rCount(i,j-1,vals,msk,cnt)
  ENDSUBROUTINE rCount
ENDMODULE Utils
PROGRAM Day9
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: n,m,ret,i,j,k
  INTEGER(4),ALLOCATABLE :: vals(:,:),basins(:,:)
  LOGICAL,ALLOCATABLE :: msk(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) n,m
  ALLOCATE(vals(n,m),msk(n,m))
  READ(98,*) vals

  msk = .TRUE.
  msk(1:n-1,1:m) = (msk(1:n-1,1:m) .AND. (vals(1:n-1,1:m) < vals(2:n,1:m)))
  msk(2:n,1:m) = (msk(2:n,1:m) .AND. (vals(2:n,1:m) < vals(1:n-1,1:m)))
  msk(1:n,1:m-1) = (msk(1:n,1:m-1) .AND. (vals(1:n,1:m-1) < vals(1:n,2:m)))
  msk(1:n,2:m) = (msk(1:n,2:m) .AND. (vals(1:n,2:m) < vals(1:n,1:m-1)))
  WRITE(*,*) SUM(vals,msk)+COUNT(msk)

  ALLOCATE(basins(COUNT(msk),3))
  k=1
  DO i=1,n
    DO j=1,m
      IF (.NOT. msk(i,j)) CYCLE
      basins(k,1) = i
      basins(k,2) = j
      basins(k,3) = 0
      k = k+1
    ENDDO
  ENDDO
  msk = (vals==9)
  DO k=1,SIZE(basins,DIM=1)
    CALL rCount(basins(k,1),basins(k,2),vals,msk,basins(k,3))
  ENDDO
  k = MAXLOC(basins(:,3),DIM=1)
  ret = basins(k,3)
  basins(k,3) = 0
  k = MAXLOC(basins(:,3),DIM=1)
  ret = ret * basins(k,3)
  basins(k,3) = 0
  k = MAXLOC(basins(:,3),DIM=1)
  ret = ret * basins(k,3)
  WRITE(*,*) ret
ENDPROGRAM Day9
