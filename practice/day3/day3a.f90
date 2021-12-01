
PROGRAM Day2
  USE,INTRINSIC :: iso_fortran_env, ONLY : iostat_end
  IMPLICIT NONE

  INTEGER(4) :: nx,ny,i,j,ret
  INTEGER(4),ALLOCATABLE :: map(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nx,ny
  ALLOCATE(map(nx,ny))

  READ(98,*) map
  i = 1
  j = 1
  ret = 0 
  DO WHILE (j<=ny)
    ret = ret + map(i,j)
    j = j+1
    i = i+3
    IF (i>nx) i = MOD(i-1,nx)+1
  ENDDO
  WRITE(*,*) ret

  DEALLOCATE(map)
ENDPROGRAM Day2
