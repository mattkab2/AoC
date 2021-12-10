PROGRAM Day2
  IMPLICIT NONE
  INTEGER(4) :: iErr,step,depth,horz
  CHARACTER(LEN=7) :: dir

  depth = 0
  horz = 0
  OPEN(unit=98,file="input.dat")
  DO WHILE(.TRUE.)
    READ(98,*,iostat=iErr) dir,step
    IF (iErr /= 0) EXIT
    SELECT CASE(dir)
    CASE("up")
      depth = depth - step
    CASE("down")
      depth = depth + step
    CASE("forward")
      horz = horz + step
    CASE DEFAULT
      STOP 123
    ENDSELECT
  ENDDO
  WRITE(*,*) depth*horz
ENDPROGRAM Day2
