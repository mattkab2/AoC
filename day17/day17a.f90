PROGRAM Day1
  IMPLICIT NONE
  INTEGER(4) :: xMin,xMax,yMin,yMax,yV,ret

  xMin=244
  xMax=303
  yMin=-91
  yMax=-54

  ! Max y-vel which passes through box is yMin
  ! The highest is obtained by a sum:
  ret = 0
  yV = yMin
  DO WHILE (yV /= 0)
    yV = yV+1
    ret = ret-yV
  ENDDO
  WRITE(*,*) ret
ENDPROGRAM Day1
