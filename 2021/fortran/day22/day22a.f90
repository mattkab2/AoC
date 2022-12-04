PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4),PARAMETER :: LO=-50,HI=50,MX=79129
  LOGICAL,ALLOCATABLE :: reactor(:,:,:)
  INTEGER(4) :: ierr
  INTEGER(8) :: xLo,xHi,yLo,yHi,zLo,zHi
  LOGICAL :: pwr

  ALLOCATE(reactor(LO:HI,LO:HI,LO:HI))
  reactor = .FALSE.
  OPEN(unit=98,file="input.dat")
  DO WHILE (.TRUE.)
    READ(98,*,IOSTAT=ierr) pwr,xLo,xHi,yLo,yHi,zLo,zHi
    IF (ierr/=0) EXIT
    xLo=MAX(xLo,-50)
    xHi=MIN(xHi,50)
    yLo=MAX(yLo,-50)
    yHi=MIN(yHi,50)
    zLo=MAX(zLo,-50)
    zHi=MIN(zHi,50)
    reactor(xLo:xHi,yLo:yHi,zLo:zHi) = pwr
  ENDDO
  WRITE(*,*) COUNT(reactor)

ENDPROGRAM Day1
