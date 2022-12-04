MODULE Utils
  IMPLICIT NONE
  CONTAINS
  FUNCTION StrContains(str1,str2) RESULT(ret)
    CHARACTER(LEN=7),INTENT(IN) :: str1,str2
    INTEGER(4) :: i
    LOGICAL :: ret
    ret = .FALSE.
    DO i=1,LEN_TRIM(str2)
      IF (INDEX(str1,str2(i:i))==0) RETURN
    ENDDO
    ret = .TRUE.
  ENDFUNCTION StrContains
  FUNCTION SortString(str) RESULT(ret)
    CHARACTER(LEN=7),INTENT(IN) :: str
    CHARACTER :: strcpy(7)
    CHARACTER(LEN=7) :: ret
    INTEGER(4) :: i,j
    DO i=1,LEN_TRIM(str)
      strcpy(i) = str(i:i)
    ENDDO
    strcpy(i:) = 'z'
    ret = ""
    DO i=1,LEN_TRIM(str)
      j=MINLOC(strcpy,DIM=1)
      ret(i:i)=strcpy(j)
      strcpy(j:j)='z'
    ENDDO
  ENDFUNCTION SortString
ENDMODULE Utils
PROGRAM Day1
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nLines,ret,i,j
  CHARACTER(LEN=7) :: key(10)
  CHARACTER(LEN=7),ALLOCATABLE :: patterns(:,:),output(:,:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nLines
  ALLOCATE(patterns(nLines,10))
  ALLOCATE(output(nLines,4))
  DO i=1,nLines
    READ(98,*) patterns(i,1:10),output(i,1:4)
  ENDDO

  ret = 0
  DO i=1,nLines
    ! Solve the puzzle. First identify 1,4,7
    key=""
    DO j=1,10
      SELECT CASE(LEN_TRIM(patterns(i,j)))
      CASE(2)
        key(1) = SortString(patterns(i,j))
      CASE(3)
        key(7) = SortString(patterns(i,j))
      CASE(4)
        key(4) = SortString(patterns(i,j))
      CASE(7)
        key(8) = SortString(patterns(i,j))
      ENDSELECT
    ENDDO
    ! Now we can search for 6 (length 6, not contains 1)
    !   3 (length 5, contains 1), 9 (length 6, contains 4)
    DO j=1,10
      SELECT CASE(LEN_TRIM(patterns(i,j)))
      CASE(5)
        IF (StrContains(patterns(i,j),key(1))) key(3) = SortString(patterns(i,j))
      CASE(6)
        IF (.NOT. StrContains(patterns(i,j),key(1))) key(6) = SortString(patterns(i,j))
        IF (StrContains(patterns(i,j),key(4))) key(9) = SortString(patterns(i,j))
      ENDSELECT
    ENDDO
    ! This also gives us 0, since it is the only len(6) one left
    ! Now we search for 5 (len 5, contained in 6)
    DO j=1,10
      SELECT CASE(LEN_TRIM(patterns(i,j)))
      CASE(5)
        IF (StrContains(key(6),patterns(i,j))) key(5) = SortString(patterns(i,j))
      CASE(6)
        IF (FINDLOC(key,SortString(patterns(i,j)),DIM=1)==0) key(10) = SortString(patterns(i,j))
      ENDSELECT
    ENDDO
    ! The last one not included will be 2
    DO j=1,10
      IF (FINDLOC(key,SortString(patterns(i,j)),DIM=1)==0) key(2) = SortString(patterns(i,j))
    ENDDO
    ! Compute value for this row
    DO j=3,0,-1
      IF (FINDLOC(key,SortString(output(i,4-j)),DIM=1)==0) THEN
        WRITE(*,*) "Err at row: ",i
        WRITE(*,*) "Key array: ",key
        WRITE(*,*) "Searching Failed for:"
        WRITE(*,*) SortString(output(i,4-j))
        STOP 123
      ENDIF
      ret = ret + (10**j)*MOD(FINDLOC(key,SortString(output(i,4-j)),DIM=1),10)
    ENDDO
  ENDDO
  WRITE(*,*) ret
ENDPROGRAM Day1
