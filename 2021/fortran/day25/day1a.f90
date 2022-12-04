MODULE Utils
  IMPLICIT NONE
  CONTAINS
    SUBROUTINE evolveE(arr,blk,changed)
      LOGICAL,INTENT(INOUT) :: arr(:,:)
      LOGICAL,INTENT(IN) :: blk(:,:)
      LOGICAL,INTENT(OUT) :: changed
      INTEGER(4) :: i,j
      changed=.FALSE.
      i=0
      DO WHILE (i<SIZE(arr,DIM=1))
        i = i+1
        j = 0
        DO WHILE (j<SIZE(arr,DIM=2))
          j = j+1
          IF (.NOT. arr(i,j)) CYCLE
          IF (j==SIZE(arr,DIM=2)) THEN
            IF (.NOT. blk(i,1)) THEN
              arr(i,1) = .TRUE.
              arr(i,j) = .FALSE.
              changed = .TRUE.
              j = j+1
            ENDIF
          ELSE
            IF (.NOT. blk(i,j+1)) THEN
              arr(i,j+1) = .TRUE.
              arr(i,j) = .FALSE.
              changed = .TRUE.
              j = j+1
            ENDIF
          ENDIF
        ENDDO
      ENDDO
    ENDSUBROUTINE evolveE
    SUBROUTINE evolveS(arr,blk,changed)
      LOGICAL,INTENT(INOUT) :: arr(:,:)
      LOGICAL,INTENT(IN) :: blk(:,:)
      LOGICAL,INTENT(OUT) :: changed
      INTEGER(4) :: i,j
      changed=.FALSE.
      j = 0
      DO WHILE (j<SIZE(arr,DIM=2))
        j = j+1
        i = 0
        DO WHILE (i<SIZE(arr,DIM=1))
          i = i+1
          IF (.NOT. arr(i,j)) CYCLE
          IF (i==SIZE(arr,DIM=1)) THEN
            IF (.NOT. blk(1,j)) THEN
              arr(1,j) = .TRUE.
              arr(i,j) = .FALSE.
              changed = .TRUE.
              i = i+1
            ENDIF
          ELSE
            IF (.NOT. blk(i+1,j)) THEN
              arr(i+1,j) = .TRUE.
              arr(i,j) = .FALSE.
              changed = .TRUE.
              i = i+1
            ENDIF
          ENDIF
        ENDDO
      ENDDO
    ENDSUBROUTINE evolveS
    SUBROUTINE printFloor(arr)
      INTEGER(4),INTENT(IN) :: arr(:,:)
      INTEGER(4) :: i,j
      DO i=1,SIZE(arr,DIM=1)
        DO j=1,SIZE(arr,DIM=2)
          SELECT CASE(arr(i,j))
          CASE(0)
            WRITE(*,'(A1)',advance='no') '.'
          CASE(1)
            WRITE(*,'(A1)',advance='no') '>'
          CASE(-1)
            WRITE(*,'(A1)',advance='no') 'v'
          ENDSELECT
        ENDDO
        WRITE(*,*)
      ENDDO
    ENDSUBROUTINE printFloor
ENDMODULE

PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nx,ny,ret
  INTEGER(4),ALLOCATABLE :: flr(:,:)
  LOGICAL,ALLOCATABLE :: east(:,:), south(:,:)
  LOGICAL :: c1,c2

  OPEN(unit=98,file="input.dat")
  READ(98,*) ny,nx
  ALLOCATE(flr(ny,nx))
  READ(98,*) flr
  flr = TRANSPOSE(flr)

  east = (flr==1)
  south = (flr==-1)

  ret=0
  c1 = .TRUE.
  c2 = .TRUE.
  DO WHILE (c1 .OR. c2)
    CALL evolveE(east,east .OR. south,c1)
    CALL evolveS(south,east .OR. south,c2)
    ret = ret+1
  ENDDO
  WHERE (east)
    flr=1
  ELSEWHERE (south)
    flr=-1
  ELSEWHERE
    flr=0
  ENDWHERE
  CALL printFloor(flr)
  WRITE(*,*) ret
ENDPROGRAM Day1
