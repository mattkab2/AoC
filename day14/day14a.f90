MODULE Utils
  IMPLICIT NONE
  TYPE,PUBLIC :: LinkedList
    TYPE(LinkedList),POINTER :: next => NULL()
    CHARACTER(LEN=1) :: elem=' '
    CONTAINS
      PROCEDURE,PASS :: append => ll_append
      PROCEDURE,PASS :: insert => ll_insert
      PROCEDURE,PASS :: clear => ll_clear
  ENDTYPE
  CONTAINS
    PURE RECURSIVE SUBROUTINE ll_append(this,c)
      CLASS(LinkedList),INTENT(INOUT) :: this
      CHARACTER(LEN=1),INTENT(IN) :: c
      TYPE(LinkedList),POINTER :: tail
      IF (.NOT. ASSOCIATED(this%next)) THEN
        ALLOCATE(tail)
        tail%elem = c
        this%next => tail
        RETURN
      ENDIF
      CALL this%next%append(c)
    ENDSUBROUTINE ll_append

    PURE SUBROUTINE ll_insert(this,c)
      CLASS(LinkedList),INTENT(INOUT) :: this
      CHARACTER(LEN=1),INTENT(IN) :: c
      TYPE(LinkedList),POINTER :: mid
      ALLOCATE(mid)
      mid%elem = c
      mid%next => this%next
      this%next => mid
    ENDSUBROUTINE ll_insert

    PURE SUBROUTINE ll_clear(this)
      CLASS(LinkedList),INTENT(INOUT) :: this
      TYPE(LinkedList),POINTER :: curr
      DO WHILE (ASSOCIATED(this%next))
        curr => this%next
        this%next => this%next%next
        DEALLOCATE(curr)
      ENDDO
    ENDSUBROUTINE ll_clear
ENDMODULE Utils

PROGRAM Day14
  USE Utils
  IMPLICIT NONE
  TYPE(LinkedList),POINTER :: head,curr
  INTEGER(4) :: nRules,i,j
  INTEGER(8) :: counts(26)
  CHARACTER(LEN=20) :: init
  CHARACTER(LEN=2),ALLOCATABLE :: ruleInp(:)
  CHARACTER(LEN=1),ALLOCATABLE :: ruleOut(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nRules
  READ(98,*) init
  ALLOCATE(ruleInp(nRules),ruleOut(nRules))
  DO i=1,nRules
    READ(98,*) ruleInp(i),ruleOut(i)
  ENDDO
  ! Initialize LL
  ALLOCATE(head)
  curr => head
  DO i=1,LEN_TRIM(init)-1
    curr%elem=init(i:i)
    ALLOCATE(curr%next)
    curr => curr%next
  ENDDO
  curr%elem=init(LEN_TRIM(init):LEN_TRIM(init))
  curr%next=>NULL()

  curr => head
  DO WHILE (ASSOCIATED(curr%next))
    WRITE(*,'(A1)',advance='no') curr%elem
    curr => curr%next
  ENDDO
  WRITE(*,'(A1)') curr%elem
  ! Begin applying rules
  curr => head
  DO i=1,10
    DO WHILE (ASSOCIATED(curr%next))
      j = FINDLOC(ruleInp,curr%elem//curr%next%elem,DIM=1)
      IF (j/=0) THEN
        CALL curr%insert(ruleOut(j))
        curr => curr%next
      ENDIF
      curr => curr%next
    ENDDO
    curr => head
  ENDDO
  curr => head
  counts=0
  DO WHILE (ASSOCIATED(curr%next))
    counts(ICHAR(curr%elem)-64) = counts(ICHAR(curr%elem)-64)+1
    curr => curr%next
  ENDDO
  counts(ICHAR(curr%elem)-64) = counts(ICHAR(curr%elem)-64)+1
  WRITE(*,*) MAXVAL(counts,counts/=0)-MINVAL(counts,counts/=0)
  NULLIFY(curr)
  CALL head%clear()
  DEALLOCATE(head,ruleInp,ruleOut)
ENDPROGRAM Day14
