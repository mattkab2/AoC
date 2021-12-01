
PROGRAM Day2
  USE,INTRINSIC :: iso_fortran_env, ONLY : iostat_end
  IMPLICIT NONE

  INTEGER(4) :: i,err,ret
  INTEGER(4) :: stt,stp
  CHARACTER(len=1) :: key
  CHARACTER(len=40) :: line

  OPEN(unit=98,file="input.dat")
  ret = 0
  DO
    READ(98,*,iostat=err) stt,stp,key,line
    SELECT CASE(err)
    CASE(0)
      IF ((line(stt:stt)==key) .XOR. (line(stp:stp)==key)) ret = ret+1 
    CASE(iostat_end)
      EXIT
    CASE Default
      WRITE(*,*) "Error in reading file"
      STOP 1
    ENDSELECT
  ENDDO
  WRITE(*,*) ret

ENDPROGRAM Day2
