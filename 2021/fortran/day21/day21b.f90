MODULE Utils
  IMPLICIT NONE
  ! Spaces are the given number - 1 because we use 0-9
  INTEGER(4),PARAMETER :: P1_START=3,P2_START=5
  INTEGER(8),PARAMETER,DIMENSION(9) :: DIE_FREQ=(/0,0,1,3,6,7,6,3,1/)
  CONTAINS
    PURE RECURSIVE SUBROUTINE play(p1_space,p2_space, &
        p1_score,p2_score,p1_wins,p2_wins,p1_turn,mult)
      INTEGER(4),INTENT(IN) :: p1_space,p2_space,p1_score,p2_score
      INTEGER(8),INTENT(IN) :: mult
      INTEGER(8),INTENT(INOUT) :: p1_wins,p2_wins
      LOGICAL,INTENT(IN) :: p1_turn
      INTEGER(4) :: spaceTmp,scoreTmp,k
      !INTEGER(8) :: winsTmp
      ! Distribution of rolls ranges from 3-9

      IF (p1_turn) THEN
        DO k=3,9
          spaceTmp = p1_space
          scoreTmp = p1_score
          CALL turn(spaceTmp,k,scoreTmp)

          IF (scoreTmp >= 21) THEN
            p1_wins = p1_wins+mult*DIE_FREQ(k)
          ELSE
            CALL play(spaceTmp,p2_space,scoreTmp,p2_score,p1_wins,p2_wins,.NOT. p1_turn, &
                mult*DIE_FREQ(k))
          ENDIF
        ENDDO
      ELSE
        DO k=3,9
          spaceTmp = p2_space
          scoreTmp = p2_score
          CALL turn(spaceTmp,k,scoreTmp)

          IF (scoreTmp >= 21) THEN
            p2_wins = p2_wins+mult*DIE_FREQ(k)
          ELSE
            CALL play(p1_space,spaceTmp,p1_score,scoreTmp,p1_wins,p2_wins,.NOT. p1_turn, &
                mult*DIE_FREQ(k))
          ENDIF
        ENDDO
      ENDIF
    ENDSUBROUTINE play
    PURE SUBROUTINE turn(space,roll,score)
      INTEGER(4),INTENT(INOUT) :: space,score
      INTEGER(4),INTENT(IN) :: roll
      space = MOD(space+roll,10)
      score = score + space + 1
    ENDSUBROUTINE turn
ENDMODULE

PROGRAM Day21
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: p1_space,p1_score,p2_space,p2_score
  INTEGER(8) :: p1_wins,p2_wins

  p1_space = P1_START
  p1_score = 0
  p1_wins = 0
  p2_space = P2_START
  p2_score = 0
  p2_wins = 0
  CALL play(p1_space,p2_space,p1_score,p2_score,p1_wins,p2_wins,.TRUE.,1_8)
  WRITE(*,*) MAX(p1_wins,p2_wins)
ENDPROGRAM Day21
