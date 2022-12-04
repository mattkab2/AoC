MODULE Utils
  TYPE,PUBLIC :: Cuboid
    INTEGER(4) :: bnds(3,2)
    !INTEGER(4) :: xLo,xHi,yLo,yHi,zLo,zHi
    LOGICAL :: isInit=.FALSE.,pwr
    CONTAINS
      PROCEDURE,PASS :: init => i_init
      PROCEDURE,PASS :: intersect => i_intersect
  ENDTYPE
  CONTAINS
    SUBROUTINE I_init(this,pwr,xlo,xhi,ylo,yhi,zlo,zhi)
      CLASS(Cuboid),INTENT(INOUT) :: this
      LOGICAL,INTENT(IN) :: pwr
      INTEGER(4),INTENT(IN) :: xlo,xhi,ylo,yhi,zlo,zhi
      this%pwr = pwr
      this%bnds(1,1) = xLo
      this%bnds(1,2) = xHi
      this%bnds(2,1) = yLo
      this%bnds(2,2) = zHi
      this%bnds(3,1) = zLo
      this%bnds(3,2) = zHi
      !this%xlo = xlo
      !this%ylo = ylo
      !this%zlo = zlo
      !this%xhi = xhi
      !this%yhi = yhi
      !this%zhi = zhi
      this%isInit = .TRUE.
    ENDSUBROUTINE I_init
    FUNCTION i_intersect(this,cube,pwr) RESULT(section)
      CLASS(Cuboid),INTENT(IN) :: this
      TYPE(Cuboid),INTENT(IN) :: cube
      LOGICAL,INTENT(IN) :: pwr
      TYPE(Cuboid) :: section
      IF (ANY(cube%bnds(:,2)<=this%%bnds(:,1)) .OR. ANY(cube%bnds(:,1)>=this%bnds(:,2))) RETURN
      CALL section%init(pwr,MAX(this%xLo,cube%xLo),MIN(this%xHi,cube%xHi), &
        MAX(this%yLo,cube%yLo),MIN(this%yHi,cube%yHi), &
        MAX(this%zLo,cube%zLo),MIN(this%zHi,cube%zHi))
    ENDFUNCTION i_intersect
ENDMODULE

PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  TYPE(Cuboid),ALLOCATABLE :: iList(:),overlap(:)
  INTEGER(4),PARAMETER :: LO=-50,HI=50,MX=79129
  INTEGER(4) :: i,j,nInstr
  INTEGER(4) :: xLo,xHi,yLo,yHi,zLo,zHi
  INTEGER(8) :: ret,tmp
  LOGICAL,ALLOCATABLE :: cubeMask(:,:,:)
  LOGICAL :: pwr

  OPEN(unit=98,file="input.dat")
  READ(98,*) nInstr
  ALLOCATE(iList(nInstr))
  WRITE(*,*) nInstr
  DO i=1,nInstr
    READ(98,*) pwr,xLo,xHi,yLo,yHi,zLo,zHi
    CALL iList(i)%init(pwr,xlo,xhi,ylo,yhi,zlo,zhi)
  ENDDO
  ret = 0
  DO i=1,nInstr
    tmp = 0
    IF (i>1) ALLOCATE(overlap(i-1))
    DO j=i-1,1,-1
      overlap(j) = iList(i)%intersect(iList(j),iList(j)%pwr)
    ENDDO
    ALLOCATE(cubeMask(iList(i)%xLo:iList(i)%xHi,iList(i)%yLo:iList(i)%yHi,iList(i)%zLo:iList(i)%zHi))
    cubeMask = .FALSE.
    DO j=1,i-1
      IF (.NOT. overlap(j)%isInit) CYCLE
      cubeMask(overlap(j)%xLo:overlap(j)%xHi, &
               overlap(j)%yLo:overlap(j)%yHi, &
               overlap(j)%zLo:overlap(j)%zHi) = overlap(j)%pwr
    ENDDO
    IF (iList(i)%pwr) THEN
      ret = ret + COUNT(.NOT. cubeMask)
    ELSE
      ! We are subtracting from ret
      ret = ret - COUNT(cubeMask)
    ENDIF
    DEALLOCATE(cubeMask)
    IF (i>1) DEALLOCATE(overlap)
  ENDDO
  WRITE(*,*) ret
ENDPROGRAM Day1
