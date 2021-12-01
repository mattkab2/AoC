
PROGRAM Day2
  USE,INTRINSIC :: iso_fortran_env, ONLY : iostat_end
  IMPLICIT NONE

  INTEGER(4) :: i,err,ret,tmp
  INTEGER(4) :: stt,stp
  CHARACTER(len=1) :: key
  CHARACTER(len=40) :: line

  OPEN(unit=98,file="input.dat")
  ret = 0
  DO
    READ(98,*,iostat=err) stt,stp,key,line
    SELECT CASE(err)
    CASE(0)
      tmp = 0
      DO i=1,40
        IF (line(i:i)==key) tmp = tmp+1
      ENDDO
      IF ((tmp >= stt) .AND. (tmp <= stp)) ret = ret+1
    CASE(iostat_end)
      EXIT
    CASE Default
      WRITE(*,*) "Error in reading file"
      STOP 1
    ENDSELECT
  ENDDO
  WRITE(*,*) ret

ENDPROGRAM Day2
