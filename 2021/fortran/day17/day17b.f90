MODULE Utils
  IMPLICIT NONE
  INTEGER(4),PARAMETER :: xMin=244, xMax=303, yMin=-91, yMax=-54
  !INTEGER(4),PARAMETER :: xMin=20, xMax=30, yMin=-10, yMax=-5
  CONTAINS
    PURE FUNCTION evalPath(xvIn,yvIn) RESULT(success)
      INTEGER(4),INTENT(IN) :: xvIn,yvIn
      LOGICAL :: success
      INTEGER(4) :: x,y,xv,yv
      LOGICAL :: inBounds
      x=0;xv=xvIn
      y=0;yv=yvIn
      inBounds=.TRUE.
      success = .TRUE.
      DO WHILE(inBounds)
        x = x+xv
        y = y+yv
        inBounds = (x <= xMax .AND. y >= yMin)
        xv = MAX(xv-1,0)
        yv = yv-1
        IF (inBounds .AND. x >= xMin .AND. y <= yMax) RETURN
      ENDDO
      success=.FALSE.
    ENDFUNCTION evalPath
ENDMODULE Utils
PROGRAM Day17
  USE Utils
  IMPLICIT NONE
  INTEGER(8) :: ret=0
  INTEGER(4) :: xv,yv

  DO xv=10,xMax
    DO yv=yMin,-yMin
      IF (evalPath(xv,yv)) ret = ret+1
    ENDDO
  ENDDO
      
  WRITE(*,*) ret
ENDPROGRAM Day17
