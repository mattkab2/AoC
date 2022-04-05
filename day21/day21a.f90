MODULE Utils
  ! Spaces are the given number - 1 because we use 0-9
  INTEGER(4),PARAMETER :: P1_START=3,P2_START=5
  TYPE,PUBLIC :: DiracDie
    INTEGER(8) :: nRolls=0
    INTEGER(4) :: nextRoll=1
    CONTAINS
      PROCEDURE,PASS :: roll => d_roll
  ENDTYPE
  CONTAINS
    FUNCTION d_roll(this) RESULT(ret)
      CLASS(DiracDie),INTENT(INOUT) :: this
      INTEGER(4) :: ret
      ret = this%nextRoll
      this%nextRoll = this%nextRoll+1
      IF (this%nextRoll>100) this%nextRoll = 1
      this%nRolls = this%nRolls+1
    ENDFUNCTION
ENDMODULE

PROGRAM Day21
  USE Utils
  IMPLICIT NONE
  TYPE(DiracDie) :: dice
  INTEGER(4) :: p1_space,p1_score,p2_space,p2_score

  p1_space = P1_START
  p1_score = 0
  p2_space = P2_START
  p2_score = 0
  DO WHILE (.TRUE.)
    p1_space = MOD(p1_space+dice%roll()+dice%roll()+dice%roll(),10)
    p1_score = p1_score + p1_space + 1
    IF (p1_score >= 1000) EXIT
    p2_space = MOD(p2_space+dice%roll()+dice%roll()+dice%roll(),10)
    p2_score = p2_score + p2_space + 1
    IF (p2_score >= 1000) EXIT
  ENDDO
  WRITE(*,*) MIN(p1_score,p2_score)*dice%nRolls
ENDPROGRAM Day21
