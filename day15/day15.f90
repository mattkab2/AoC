PROGRAM Day1
  IMPLICIT NONE
  !INTEGER(4) :: i,j
  INTEGER(4) :: n,pos(2)
  INTEGER(4),ALLOCATABLE :: wgts(:,:),path(:,:)
  REAL(8) :: start,finish
  LOGICAL,ALLOCATABLE :: msk(:,:)

  CALL CPU_TIME(start)
  OPEN(unit=98,file="input.dat")
  READ(98,*) n
  CALL CPU_TIME(finish)
  WRITE(*,*) "Loaded data: ",(finish-start)*1e6,"us"

  ALLOCATE(wgts(n,n),path(n,n),msk(n,n))
  READ(98,*) wgts

  !ALLOCATE(wgts(5*n,5*n),path(5*n,5*n),msk(5*n,5*n))
  !DO i=1,n
  !  READ(98,*) wgts(i,1:n)
  !  DO j=1,4
  !    wgts(i,j*n+1:(j+1)*n) = 1+MOD(wgts(i,(j-1)*n+1:j*n),9)
  !  ENDDO
  !ENDDO
  !DO j=1,4
  !  wgts(j*n+1:(j+1)*n,:) = 1 + MOD(wgts((j-1)*n+1:j*n,:),9)
  !ENDDO
  !n = 5*n

  CALL CPU_TIME(start)
  path=HUGE(n)
  path(1,1)=0
  msk = .TRUE.
  DO WHILE (msk(n,n))
    pos = MINLOC(path,msk)
    ! Mark visited
    msk(pos(1),pos(2)) = .FALSE.
    ! Update neighbors
    IF (pos(1)>1) path(pos(1)-1,pos(2))=MIN(path(pos(1)-1,pos(2)),path(pos(1),pos(2))+wgts(pos(1)-1,pos(2)))
    IF (pos(1)<n) path(pos(1)+1,pos(2))=MIN(path(pos(1)+1,pos(2)),path(pos(1),pos(2))+wgts(pos(1)+1,pos(2)))
    IF (pos(2)>1) path(pos(1),pos(2)-1)=MIN(path(pos(1),pos(2)-1),path(pos(1),pos(2))+wgts(pos(1),pos(2)-1))
    IF (pos(2)<n) path(pos(1),pos(2)+1)=MIN(path(pos(1),pos(2)+1),path(pos(1),pos(2))+wgts(pos(1),pos(2)+1))
  ENDDO
  CALL CPU_TIME(finish)
  WRITE(*,*) path(n,n)
  WRITE(*,*) "Time:",(finish-start),"s"
ENDPROGRAM Day1
