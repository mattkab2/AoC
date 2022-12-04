MODULE Utils
  IMPLICIT NONE
  TYPE,PUBLIC :: Snail
    LOGICAL :: isScalar=.FALSE.
    TYPE(Snail),POINTER :: left=>NULL(),right=>NULL(),parent => NULL()
    INTEGER(8) :: val
    CONTAINS
      PROCEDURE,PASS :: simplify => s_simplify
      PROCEDURE,PASS :: magnitude => s_magnitude
      PROCEDURE,PASS :: explode => s_explode
      PROCEDURE,PASS :: split => s_split
      PROCEDURE,PASS :: fromString => s_init
      PROCEDURE,PASS :: print => s_print
      PROCEDURE,PASS :: clear => s_clear
      PROCEDURE,PASS :: copy => s_copy
  ENDTYPE
  CONTAINS
    FUNCTION add(a,b) RESULT(c)
      TYPE(Snail),TARGET,INTENT(INOUT) :: a,b
      TYPE(Snail),POINTER:: c
      ALLOCATE(c)
      c%isScalar=.FALSE.
      c%left => a
      a%parent => c
      c%right => b
      b%parent => c
    ENDFUNCTION add
    SUBROUTINE s_simplify(this)
      CLASS(Snail),INTENT(INOUT) :: this
      LOGICAL :: altered
      altered = .TRUE.
      DO WHILE (altered)
        DO WHILE (altered)
          altered = .FALSE.
          CALL rExplode(this,altered,0)
        ENDDO
        altered = .FALSE.
        CALL rSplit(this,altered)
      ENDDO
    ENDSUBROUTINE s_simplify
    RECURSIVE SUBROUTINE rExplode(root,altered,depth)
      TYPE(Snail),INTENT(INOUT) :: root
      LOGICAL,INTENT(INOUT) :: altered
      INTEGER(4),INTENT(IN) :: depth
     
      IF (root%isScalar) RETURN
      IF (depth==4) THEN
        altered = .TRUE.
        CALL root%explode()
        RETURN
      ENDIF
      CALL rExplode(root%left,altered,depth+1)
      IF (altered) RETURN
      CALL rExplode(root%right,altered,depth+1)
    ENDSUBROUTINE rExplode
    RECURSIVE SUBROUTINE rSplit(root,altered)
      TYPE(Snail),INTENT(INOUT) :: root
      LOGICAL,INTENT(INOUT) :: altered
      
      IF (root%isScalar) THEN
        IF (root%val>=10) THEN
          altered = .TRUE.
          CALL root%split()
        ENDIF
        RETURN
      ENDIF
      CALL rSplit(root%left,altered)
      IF (altered) RETURN
      CALL rSplit(root%right,altered)
    ENDSUBROUTINE rSplit
    RECURSIVE FUNCTION s_magnitude(this) RESULT(ret)
      CLASS(Snail),INTENT(IN) :: this
      INTEGER(8) :: ret
      
      IF (this%isScalar) THEN
        ret=this%val
        RETURN
      ENDIF
      ret = 3*this%left%magnitude() + 2*this%right%magnitude()
    ENDFUNCTION s_magnitude
    SUBROUTINE s_explode(this)
      CLASS(Snail),TARGET,INTENT(INOUT) :: this
      TYPE(Snail),POINTER :: tmp,tmp2
      tmp => this%parent
      tmp2 => this
      DO WHILE (ASSOCIATED(tmp) .AND. ASSOCIATED(tmp%left,tmp2))
        tmp => tmp%parent
        tmp2 => tmp2%parent
      ENDDO
      IF (ASSOCIATED(tmp)) THEN
        tmp => tmp%left
        DO WHILE (.NOT. tmp%isScalar)
          tmp => tmp%right
        ENDDO
        tmp%val = tmp%val + this%left%val
      ENDIF
      tmp => this%parent
      tmp2 => this
      DO WHILE (ASSOCIATED(tmp) .AND. ASSOCIATED(tmp%right,tmp2))
        tmp => tmp%parent
        tmp2 => tmp2%parent
      ENDDO
      IF (ASSOCIATED(tmp)) THEN
        tmp => tmp%right
        DO WHILE (.NOT. tmp%isScalar)
          tmp => tmp%left
        ENDDO
        tmp%val = tmp%val + this%right%val
      ENDIF
      CALL this%left%clear()
      DEALLOCATE(this%left)
      CALL this%right%clear()
      DEALLOCATE(this%right)
      this%isScalar = .TRUE.
      this%val=0
    ENDSUBROUTINE s_explode
    SUBROUTINE s_split(this)
      CLASS(Snail),TARGET,INTENT(INOUT) :: this
      this%isScalar = .FALSE.
      ALLOCATE(this%left)
      this%left%isScalar = .TRUE.
      this%left%val = this%val/2
      this%left%parent => this
      ALLOCATE(this%right)
      this%right%isSCalar = .TRUE.
      this%right%val = this%val - this%left%val
      this%right%parent => this
    ENDSUBROUTINE s_split
    SUBROUTINE s_init(this,string,sPtr)
      CLASS(Snail),TARGET,INTENT(INOUT) :: this
      CHARACTER(LEN=80),INTENT(IN) :: string
      INTEGER(4),INTENT(INOUT) :: sPtr

      DO WHILE(.TRUE.)
        sPtr = sPtr+1
        SELECT CASE(string(sPtr:sPtr))
        CASE('[')
          this%isScalar = .FALSE.
          ALLOCATE(this%left)
          CALL this%left%fromString(string,sPtr)
          this%left%parent => this
        CASE(',')
          ALLOCATE(this%right)
          CALL this%right%fromString(string,sPtr)
          this%right%parent => this
        CASE(']')
          RETURN
        CASE(' ')
          RETURN
        CASE DEFAULT
          ! Create scalar
          READ(string(sPtr:sPtr),*) this%val
          this%isScalar = .TRUE.
          RETURN
        ENDSELECT
      ENDDO
    ENDSUBROUTINE s_init
    SUBROUTINE s_print(this)
      CLASS(Snail),INTENT(IN) :: this
     
      IF (this%isScalar) THEN
        WRITE(*,'(i2)',advance='no') this%val
        RETURN
      ENDIF
      WRITE(*,'(a1)',advance='no') '['
      CALL this%left%print()
      WRITE(*,'(a1)',advance='no') ','
      CALL this%right%print()
      WRITE(*,'(a1)',advance='no') ']'
    ENDSUBROUTINE s_print
    SUBROUTINE s_clear(this)
      CLASS(Snail),INTENT(INOUT) :: this
      NULLIFY(this%parent)
      IF (this%isScalar) RETURN
      CALL this%left%clear()
      DEALLOCATE(this%left)
      CALL this%right%clear()
      DEALLOCATE(this%right)
    ENDSUBROUTINE s_clear
    RECURSIVE FUNCTION s_copy(this) RESULT(cpy)
      CLASS(Snail),INTENT(INOUT) :: this
      TYPE(Snail),POINTER :: cpy

      ALLOCATE(cpy)
      cpy%isScalar = this%isScalar
      cpy%val = this%val
      IF (.NOT. this%isScalar) THEN
        cpy%left => this%left%copy()
        cpy%left%parent => cpy
        cpy%right => this%right%copy()
        cpy%right%parent => cpy
      ENDIF
    ENDFUNCTION
ENDMODULE

PROGRAM Day18
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nLines,sPtr=0,i,j
  INTEGER(8) :: ret
  CHARACTER(LEN=80) :: line
  TYPE(Snail),ALLOCATABLE :: sn(:)
  TYPE(Snail),POINTER :: root

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(sn(nLines))
  DO i=1,nLines
    READ(98,'(A80)') line
    sPtr = 0
    CALL sn(i)%fromString(line,sPtr)
  ENDDO
  root => add(sn(1)%copy(),sn(2)%copy())
  CALL root%simplify()
  DO i=3,nLines
    root => add(root,sn(i)%copy())
    CALL root%simplify()
  ENDDO
  WRITE(*,*) root%magnitude()
  CALL root%clear()
  DEALLOCATE(root)

  ret = 0
  DO i=1,nLines
    DO j=1,nLines
      IF (i==j) CYCLE
      root => add(sn(i)%copy(),sn(j)%copy())
      CALL root%simplify()
      ret = MAX(ret,root%magnitude())
      CALL root%clear()
      DEALLOCATE(root)
    ENDDO
  ENDDO
  WRITE(*,*) ret
  DO i=1,nLines
    CALL sn(i)%clear()
  ENDDO
  DEALLOCATE(sn)

ENDPROGRAM Day18
