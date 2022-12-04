MODULE Utils
  IMPLICIT NONE
  TYPE,PUBLIC :: Packet
    INTEGER(8) :: pID=-1,versID=-1,typeID=-1,bitLen=-1,pLen=-1,val=0
    LOGICAL :: lenIsNum=.FALSE.,isLiteral=.FALSE.
    TYPE(Packet),ALLOCATABLE :: subP(:)
    CONTAINS
      PROCEDURE,PASS :: init => p_init
      PROCEDURE,PASS :: eval => p_eval
  ENDTYPE
  CONTAINS
    FUNCTION binToInt(bits) RESULT(ret)
      LOGICAL,INTENT(IN) :: bits(:)
      INTEGER(8) :: i,ret
      ret=0
      DO i=LBOUND(bits,DIM=1),UBOUND(bits,DIM=1)-1
        IF (bits(i)) ret = ret+1
        ret = 2*ret
      ENDDO
      IF (bits(UBOUND(bits,DIM=1))) ret = ret+1
    ENDFUNCTION binToInt
    PURE FUNCTION hexToBin(c) RESULT(ret)
      CHARACTER,INTENT(IN) :: c
      LOGICAL :: ret(4)
      SELECT CASE(c)
      CASE('0'); ret=(/.FALSE., .FALSE., .FALSE., .FALSE./)
      CASE('1'); ret=(/.FALSE., .FALSE., .FALSE., .TRUE./)
      CASE('2'); ret=(/.FALSE., .FALSE., .TRUE. , .FALSE./)
      CASE('3'); ret=(/.FALSE., .FALSE., .TRUE. , .TRUE./)
      CASE('4'); ret=(/.FALSE., .TRUE. , .FALSE., .FALSE./)
      CASE('5'); ret=(/.FALSE., .TRUE. , .FALSE., .TRUE./)
      CASE('6'); ret=(/.FALSE., .TRUE. , .TRUE. , .FALSE./)
      CASE('7'); ret=(/.FALSE., .TRUE. , .TRUE. , .TRUE./)
      CASE('8'); ret=(/.TRUE. , .FALSE., .FALSE., .FALSE./)
      CASE('9'); ret=(/.TRUE. , .FALSE., .FALSE., .TRUE./)
      CASE('A'); ret=(/.TRUE. , .FALSE., .TRUE. , .FALSE./)
      CASE('B'); ret=(/.TRUE. , .FALSE., .TRUE. , .TRUE./)
      CASE('C'); ret=(/.TRUE. , .TRUE. , .FALSE., .FALSE./)
      CASE('D'); ret=(/.TRUE. , .TRUE. , .FALSE., .TRUE./)
      CASE('E'); ret=(/.TRUE. , .TRUE. , .TRUE. , .FALSE./)
      CASE DEFAULT; ret=(/.TRUE. , .TRUE. , .TRUE., .TRUE./)
      ENDSELECT
    ENDFUNCTION hexToBin
    SUBROUTINE p_init(this,pID,bitstring,packetStt)
      CLASS(Packet),INTENT(INOUT) :: this
      INTEGER(8),INTENT(INOUT) :: pID
      LOGICAL,INTENT(IN) :: bitstring(:)
      LOGICAL :: lenIsNum
      INTEGER(8),INTENT(IN) :: packetStt(:)
      INTEGER(8) :: i,bPtr,bOld

      this%pID = pID
      pID = pID+1
      bPtr = packetStt(this%pID)
      this%versID = binToInt(bitstring(bPtr:bPtr+2))
      this%typeID = binToInt(bitstring(bPtr+3:bPtr+5))
      this%isLiteral = (this%typeID==4)
      IF (this%isLiteral) THEN
        ! Read in literal
        bPtr = bPtr+6
        this%val = 0
        DO WHILE(bitstring(bPtr))
          this%val = this%val*16
          this%val = this%val+binToInt(bitstring(bPtr+1:bPtr+4))
          bPtr = bPtr+5
        ENDDO
        this%val = this%val*16
        this%val = this%val+binToInt(bitstring(bPtr+1:bPtr+4))
        RETURN
      ENDIF

      ! We have a collection of packets
      lenIsNum = bitstring(bPtr+6)
      IF (lenIsNum) THEN
        this%pLen = binToInt(bitstring(bPtr+7:bPtr+17))
        bPtr = bPtr+18
        bOld = bPtr
        ALLOCATE(this%subP(this%pLen))
        DO i=1,this%pLen
          CALL this%subP(i)%init(pID,bitstring,packetStt)
        ENDDO
        this%bitLen = bPtr-bOld+1
      ELSE
        this%bitLen = binToInt(bitstring(bPtr+7:bPtr+21))
        this%pLen = 1
        ALLOCATE(this%subP(COUNT((packetStt > packetStt(this%pID)) .AND. (packetStt < packetStt(this%pID+1)+this%bitLen))))
        DO WHILE ((packetStt(pID)<packetStt(this%pID+1)+this%bitLen) .AND. packetStt(pID)/=-1)
          CALL this%subP(this%pLen)%init(pID,bitstring,packetStt)
          this%pLen = this%pLen+1
        ENDDO
        this%pLen = MAX(this%pLen-1,1)
      ENDIF 
    ENDSUBROUTINE p_init
    RECURSIVE SUBROUTINE p_eval(this,ret)
      CLASS(Packet),INTENT(INOUT) :: this
      INTEGER(8),INTENT(OUT) :: ret
      INTEGER(8) :: i,tmp

      IF (ALLOCATED(this%subP)) THEN
        SELECT CASE(this%typeID)
        CASE(0)
          this%val = 0
          DO i=1,this%pLen
            CALL this%subP(i)%eval(tmp)
            this%val = this%val + tmp
          ENDDO
        CASE(1)
          this%val = 1
          DO i=1,this%pLen
            CALL this%subP(i)%eval(tmp)
            this%val = this%val * tmp
          ENDDO
        CASE(2)
          this%val = HUGE(this%val)
          DO i=1,this%pLen
            CALL this%subP(i)%eval(tmp)
            this%val = MIN(this%val,tmp)
          ENDDO
        CASE(3)
          this%val = -HUGE(this%val)
          DO i=1,this%pLen
            CALL this%subP(i)%eval(tmp)
            this%val = MAX(this%val,tmp)
          ENDDO
        CASE(5)
          CALL this%subP(1)%eval(tmp)
          CALL this%subP(2)%eval(tmp)
          IF (this%subP(1)%val > this%subP(2)%val) THEN
            this%val=1
          ELSE
            this%val=0
          ENDIF
        CASE(6)
          CALL this%subP(1)%eval(tmp)
          CALL this%subP(2)%eval(tmp)
          IF (this%subP(1)%val < this%subP(2)%val) THEN
            this%val=1
          ELSE
            this%val=0
          ENDIF
        CASE(7)
          CALL this%subP(1)%eval(tmp)
          CALL this%subP(2)%eval(tmp)
          IF (this%subP(1)%val == this%subP(2)%val) THEN
            this%val=1
          ELSE
            this%val=0
          ENDIF
        CASE DEFAULT
          STOP 123
        ENDSELECT
      ENDIF
      ret = this%val
    ENDSUBROUTINE p_eval
ENDMODULE
PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  INTEGER(8) :: nHex,i,tID,pID,bPtr,ret
  INTEGER(8),ALLOCATABLE :: packetStt(:)
  CHARACTER(LEN=1500) :: hexstring
  LOGICAL,ALLOCATABLE :: bitstring(:)
  TYPE(Packet) :: root

  OPEN(unit=98,file="input.txt")
  READ(98,*) hexstring
  nHex = LEN_TRIM(hexstring)
  WRITE(*,*) hexstring(1:nHex)
  ALLOCATE(bitstring(4*nHex))
  ALLOCATE(packetStt(4*nHex/7))
  packetStt=-1
  DO i=0,nHex-1
    bitstring(4*i+1:4*(i+1)) = hexToBin(hexstring(i+1:i+1))
  ENDDO
  bPtr=1
  pID=1
  ret=0
  DO WHILE(bPtr < SIZE(bitstring))
    packetStt(pID) = bPtr
    ret = ret + binToInt(bitstring(bPtr:bPtr+2))
    tID = binToInt(bitstring(bPtr+3:bPtr+5))
    IF (tID == 4) THEN
      ! We are looking at a literal
      bPtr = bPtr + 6
      DO WHILE (bitstring(bPtr))
        bPtr = bPtr + 5
      ENDDO
      bPtr = bPtr + 5
    ELSE
      IF (bitstring(bPtr+6)) THEN
        ! Packet length given
        bPtr = bPtr+7+11
      ELSE
        bPtr = bPtr+7+15
      ENDIF
    ENDIF
    pID = pID+1
  ENDDO
  WRITE(*,*) ret

  pID=1
  CALL root%init(pID,bitstring,packetStt)
  CALL root%eval(ret)
  WRITE(*,*) ret

  DEALLOCATE(bitstring,packetStt)

ENDPROGRAM Day1

