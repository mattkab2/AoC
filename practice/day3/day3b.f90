MODULE Utils
  IMPLICIT NONE
  CONTAINS
  SUBROUTINE getTrees(map,mx,my,n)
    INTEGER(4),INTENT(IN) :: mx,my
    INTEGER(4),ALLOCATABLE,INTENT(IN) :: map(:,:)
    INTEGER(4),INTENT(OUT) :: n
    INTEGER(4) :: i,j

    i = 1
    j = 1
    n = 0 
    DO WHILE (j<=SIZE(map,2))
      n = n + map(i,j)
      j = j+my
      i = i+mx
      IF (i>SIZE(map,1)) i = MOD(i-1,SIZE(map,1))+1
    ENDDO
  ENDSUBROUTINE getTrees
ENDMODULE Utils

PROGRAM Day2
  USE Utils
  IMPLICIT NONE

  INTEGER(4) :: nx,ny,ret,tmp
  INTEGER(4),ALLOCATABLE :: map(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nx,ny
  ALLOCATE(map(nx,ny))

  READ(98,*) map
  ret = 1
  CALL getTrees(map,1,1,tmp)
  WRITE(*,*) tmp
  ret = ret*tmp
  CALL getTrees(map,3,1,tmp)
  WRITE(*,*) tmp
  ret = ret*tmp
  CALL getTrees(map,5,1,tmp)
  WRITE(*,*) tmp
  ret = ret*tmp
  CALL getTrees(map,7,1,tmp)
  WRITE(*,*) tmp
  ret = ret*tmp
  CALL getTrees(map,1,2,tmp)
  WRITE(*,*) tmp
  ret = ret*tmp
  WRITE(*,*) ret

  DEALLOCATE(map)
ENDPROGRAM Day2

