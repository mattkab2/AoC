PROGRAM Day3
  IMPLICIT NONE
  INTEGER(4) :: nLines,nBits,gRate,i
  INTEGER(4),ALLOCATABLE :: vals(:,:)
  LOGICAL,ALLOCATABLE :: outBin(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nBits,nLines
  ALLOCATE(vals(nBits,nLines))
  ALLOCATE(outBin(nBits))
  READ(98,*) vals

  outBin = (SUM(vals,DIM=2) > nLines/2) ! Even #lines in input
  gRate = 0
  DO i=nBits,1,-1
    IF (outBin(i)) gRate = gRate + 2**(nBits-i)
  ENDDO
  WRITE(*,*) gRate*(2**nBits-gRate-1)

ENDPROGRAM Day3
