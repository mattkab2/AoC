PROGRAM Day2
  IMPLICIT NONE
  INTEGER(4) :: iErr,aim,step,depth,horz
  CHARACTER(LEN=7) :: dir

  depth = 0
  horz = 0
  aim = 0
  OPEN(unit=98,file="input.dat")
  DO WHILE(.TRUE.)
    READ(98,*,iostat=iErr) dir,step
    IF (iErr /= 0) EXIT
    SELECT CASE(dir)
    CASE("up")
      aim = aim-step
    CASE("down")
      aim = aim+step
    CASE("forward")
      horz = horz + step
      depth = depth + aim*step
    CASE DEFAULT
      STOP 123
    ENDSELECT
  ENDDO
  WRITE(*,*) depth*horz
ENDPROGRAM Day2
