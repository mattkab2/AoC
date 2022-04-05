MODULE Utils
  IMPLICIT NONE
  INTEGER(4),PARAMETER :: I_INP=0,I_ADD=1,I_MUL=2,I_DIV=3,I_MOD=4,I_EQL=5
  TYPE,PUBLIC :: Instruction
    INTEGER(4) :: instr,arg1,arg2
    LOGICAL :: literal
    CONTAINS
      PROCEDURE,PASS :: init => i_init
  ENDTYPE
  TYPE,PUBLIC :: ALU
    INTEGER(4) :: nLines,iPtr,input(14)=(/1,3,5,7,9,2,4,6,8,9,9,9,9,9/)
    INTEGER(8) :: reg(4)
    TYPE(Instruction),ALLOCATABLE :: prog(:)
    CONTAINS
      PROCEDURE,PASS :: init => a_init
      PROCEDURE,PASS :: exec => a_exec
      PROCEDURE,PASS :: reset => a_reset
      PROCEDURE,PASS :: clear => a_clear
  ENDTYPE
  CONTAINS
    SUBROUTINE a_init(this,nLines,unitNo)
      CLASS(ALU),INTENT(INOUT) :: this
      INTEGER(4),INTENT(IN) :: nLines,unitNo
      CHARACTER(LEN=3) :: instr,r2
      CHARACTER(LEN=1) :: r1
      INTEGER(4) :: i

      this%nLines = nLines
      ALLOCATE(this%prog(nLines))
      DO i=1,nLines
        READ(unitNo,'(A3)',advance='no') instr
        IF (instr=='inp') THEN
          READ(unitNo,*) r1
          r2 = ""
        ELSE
          READ(unitNo,*) r1,r2
        ENDIF
        CALL this%prog(i)%init(instr,r1,r2)
      ENDDO
      this%reg = 0
      this%iPtr = 1
    ENDSUBROUTINE
    SUBROUTINE i_init(this,instr,r1,r2)
      CLASS(Instruction),INTENT(INOUT) :: this
      CHARACTER(LEN=3),INTENT(IN) :: instr,r2
      CHARACTER(LEN=1),INTENT(IN) :: r1
        SELECT CASE(instr)
        CASE('inp')
          this%instr = I_INP
        CASE('add')
          this%instr = I_ADD
        CASE('mul')
          this%instr = I_MUL
        CASE('div')
          this%instr = I_DIV
        CASE('mod')
          this%instr = I_MOD
        CASE('eql')
          this%instr = I_EQL
        ENDSELECT
        SELECT CASE(r1)
        CASE('x')
          this%arg1 = 1
        CASE('y')
          this%arg1 = 2
        CASE('z')
          this%arg1 = 3
        CASE('w')
          this%arg1 = 4
        ENDSELECT
        this%literal = .FALSE.
        IF (this%instr /= I_INP) THEN
          SELECT CASE(r2)
          CASE('x')
            this%arg2 = 1
          CASE('y')
            this%arg2 = 2
          CASE('z')
            this%arg2 = 3
          CASE('w')
            this%arg2 = 4
          CASE DEFAULT
            this%literal = .TRUE.
            READ(r2,*) this%arg2
          ENDSELECT
        ELSE
          this%arg2 = 0
        ENDIF
    ENDSUBROUTINE i_init
    FUNCTION a_exec(this) RESULT(ret)
      CLASS(ALU),INTENT(INOUT) :: this
      INTEGER(4) :: i
      LOGICAL :: ret
      DO i=1,this%nLines
        SELECT CASE(this%prog(i)%instr)
        CASE(I_INP)
          this%reg(this%prog(i)%arg1) = this%input(this%iPtr)
          this%iPtr = this%iPtr+1
        CASE(I_ADD)
          IF (this%prog(i)%literal) THEN
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) + this%prog(i)%arg2
          ELSE
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) + this%reg(this%prog(i)%arg2)
          ENDIF
        CASE(I_MUL)
          IF (this%prog(i)%literal) THEN
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) * this%prog(i)%arg2
          ELSE
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) * this%reg(this%prog(i)%arg2)
          ENDIF
        CASE(I_DIV)
          IF (this%prog(i)%literal) THEN
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) / this%prog(i)%arg2
          ELSE
            this%reg(this%prog(i)%arg1) = this%reg(this%prog(i)%arg1) / this%reg(this%prog(i)%arg2)
          ENDIF
        CASE(I_MOD)
          IF (this%prog(i)%literal) THEN
            this%reg(this%prog(i)%arg1) = MOD(this%reg(this%prog(i)%arg1),this%prog(i)%arg2)
          ELSE
            this%reg(this%prog(i)%arg1) = MOD(this%reg(this%prog(i)%arg1),this%reg(this%prog(i)%arg2))
          ENDIF
        CASE(I_EQL)
          IF (this%prog(i)%literal) THEN
            IF (this%reg(this%prog(i)%arg1)==this%prog(i)%arg2) THEN
              this%reg(this%prog(i)%arg1) = 1
            ELSE
              this%reg(this%prog(i)%arg1) = 0
            ENDIF
          ELSE
            IF (this%reg(this%prog(i)%arg1)==this%reg(this%prog(i)%arg2)) THEN
              this%reg(this%prog(i)%arg1) = 1
            ELSE
              this%reg(this%prog(i)%arg1) = 0
            ENDIF
          ENDIF
        ENDSELECT
      ENDDO
      ret = (this%reg(3)==0)
    ENDFUNCTION a_exec
    SUBROUTINE a_reset(this)
      CLASS(ALU),INTENT(INOUT) :: this
      this%reg=0
      this%iPtr=1
    ENDSUBROUTINE
    SUBROUTINE a_clear(this)
      CLASS(ALU),INTENT(INOUT) :: this
      DEALLOCATE(this%prog)
    ENDSUBROUTINE
ENDMODULE
PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nLines,i,j
  TYPE(ALU) :: subALU

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines

  CALL subALU%init(nLines,98)
  subALU%input = 9
  j=0
  DO WHILE (.NOT. subALU%exec())
    j = j+1
    CALL subALU%reset()
    subALU%input(14) = subALU%input(14)-1
    DO WHILE (ANY(subALU%input==0))
      DO i=14,2,-1
        IF (subALU%input(i)==0) THEN
          subALU%input(i)=9
          subALU%input(i-1) = subALU%input(i-1)-1
        ENDIF
      ENDDO
    ENDDO
    IF (MOD(j,1000)==0) WRITE(*,*) subALU%input
  ENDDO
  WRITE(*,*) subALU%input
ENDPROGRAM Day1
