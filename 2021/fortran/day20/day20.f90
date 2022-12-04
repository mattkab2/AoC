MODULE Utils
  INTEGER,PARAMETER :: APPLY=50
  CONTAINS
    PURE FUNCTION enhance(arr,map) RESULT(val)
      LOGICAL,INTENT(IN) :: arr(3,3)
      LOGICAL,INTENT(IN) :: map(512)
      LOGICAL :: val
      INTEGER(4) :: tmp
      tmp=1
      IF (arr(1,1)) tmp = tmp+256
      IF (arr(1,2)) tmp = tmp+128
      IF (arr(1,3)) tmp = tmp+64
      IF (arr(2,1)) tmp = tmp+32
      IF (arr(2,2)) tmp = tmp+16
      IF (arr(2,3)) tmp = tmp+8
      IF (arr(3,1)) tmp = tmp+4
      IF (arr(3,2)) tmp = tmp+2
      IF (arr(3,3)) tmp = tmp+1
      val = map(tmp)
    ENDFUNCTION enhance
    SUBROUTINE print(arr,lo,hi)
      LOGICAL,INTENT(IN) :: arr(:,:)
      INTEGER(4) :: lo,hi
      DO i=lo,hi
        DO j=lo,hi
          IF (arr(i,j)) THEN
            WRITE(*,'(A1)',advance='no') '#'
          ELSE
            WRITE(*,'(A1)',advance='no') '.'
          ENDIF
        ENDDO
        WRITE(*,*)
      ENDDO
      WRITE(*,*) 
    ENDSUBROUTINE print
ENDMODULE
PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: i,j,k,n
  LOGICAL :: bg,map(512)
  LOGICAL,ALLOCATABLE :: image(:,:),tmp(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) n
  READ(98,*) map
  n = n-2
  ALLOCATE(tmp(n,n))
  tmp = .FALSE.
  READ(98,*) tmp
  tmp = TRANSPOSE(tmp)
  ALLOCATE(image(n+4,n+4))
  bg = .FALSE.
  image = bg
  image(3:n+2,3:n+2) = tmp
  DEALLOCATE(tmp)

  CALL print(image,3,n+2)
  DO k=1,APPLY
    ALLOCATE(tmp(n+2,n+2))
    tmp = .FALSE.
    DO i=2,n+3
      DO j=2,n+3
        tmp(i-1,j-1) = enhance(image(i-1:i+1,j-1:j+1),map)
      ENDDO
    ENDDO
    ! Toggle background material
    !   works only for map(1) = T, map(-1) = F
    bg = .NOT. bg
    DEALLOCATE(image)
    ALLOCATE(image(n+6,n+6))
    image = bg
    image(3:n+4,3:n+4) = tmp(:,:)
    DEALLOCATE(tmp)
    n = n + 2
  ENDDO
  CALL print(image,3,n+2)
  WRITE(*,*) COUNT(image)

ENDPROGRAM Day1
