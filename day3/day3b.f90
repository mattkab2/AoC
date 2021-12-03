MODULE Utils
  IMPLICIT NONE
  CONTAINS
  SUBROUTINE filter(vals,nBits,invCrit)
    INTEGER(4),ALLOCATABLE,INTENT(INOUT) :: vals(:)
    INTEGER(4),INTENT(IN) :: nBits
    LOGICAL,INTENT(IN) :: invCrit
    INTEGER(4) :: i
    LOGICAL,ALLOCATABLE :: msk(:)
    LOGICAL :: crit,msk(:)
    ALLOCATE(msk(SIZE(vals)))
    DO i=nBits-1,0,-1
      crit = (COUNT(BTEST(vals,i)) >= COUNT(.NOT. BTEST(vals,i)))
      IF (invCrit) crit = .NOT. crit
      msk = (BTEST(vals,i) .EQV. crit)
      vals = PACK(vals,msk)
      IF (SIZE(vals)==1) RETURN
    ENDDO
  ENDSUBROUTINE filter
ENDMODULE Utils
PROGRAM Day3
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nLines,nBits,i,j
  INTEGER(4),ALLOCATABLE :: valsC(:),valsO(:),bits(:)
  LOGICAL,ALLOCATABLE :: msk(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nBits,nLines
  ALLOCATE(bits(nBits),msk(nLines))
  ALLOCATE(valsO(nLines),valsC(nLines))
  DO i=1,nLines
    READ(98,*) bits
    valsO(i) = 0
    DO j=nBits,1,-1
      IF (bits(j)==1) valsO(i) = valsO(i) + 2**(SIZE(bits)-j)
    ENDDO
    valsC(i) = valsO(i)
  ENDDO
  CALL filter(valsO,nBits,.FALSE.)
  CALL filter(valsC,nBits,.TRUE.)
  WRITE(*,*) valsC(1)*valsO(1)
ENDPROGRAM Day3
